#include "main.h"


void Data_To_Store::write_everything(byte shift_to_left, byte size_in_bits, byte num) {

  // flag for makig sure the size is smaller than the size in bits specified
  byte flag = 1 << size_in_bits;
  flag--;
  num = (num & flag);
  unsigned long num_copy = num;
  num_copy = num_copy << shift_to_left;
  //storing the num in the date time
  date_time_temp = date_time_temp | num_copy;
}

byte Data_To_Store::read_everything(byte shift_to_left, byte size_in_bits) {
  //bitwise operations to get the data from the unsigned long
  unsigned long retval = date_time_temp;
  byte flag = (1 << size_in_bits);
  flag--;
  retval = retval >> shift_to_left;
  retval = retval & flag;
  return retval;
}
void Data_To_Store::set_hum(int8_t h) {humidity = h;}
int8_t Data_To_Store::get_hum() {return humidity;}
void Data_To_Store::set_temp(int8_t t) {temp = t;}
int8_t Data_To_Store::get_temp() {return temp;}



byte Data_To_Store::get_year(){return read_everything(26, 6);}
byte Data_To_Store::get_month(){return read_everything(22, 4);}
byte Data_To_Store::get_day(){return read_everything(17, 5);}
byte Data_To_Store::get_hour(){return read_everything(12, 5);}
byte Data_To_Store::get_minute(){return read_everything(6, 6);}
byte Data_To_Store::get_second(){return read_everything(0, 6);}
void Data_To_Store::set_year(byte num){write_everything(26, 6, num);}
void Data_To_Store::set_month(byte num){write_everything(22, 4, num);}
void Data_To_Store::set_day(byte num){write_everything(17, 5, num);}
void Data_To_Store::set_hour(byte num){write_everything(12, 5, num);}
void Data_To_Store::set_minute(byte num){write_everything(6, 6, num);}
void Data_To_Store::set_second(byte num){write_everything(0, 6, num);}






int Eeprom_indexes::get_start_i(){return start_i;}
int Eeprom_indexes::get_end_i(){return end_i;}
int Eeprom_indexes::get_curr_i(){return curr_i;}
int Eeprom_indexes::get_stored_data_count(){return stored_data_count;}


Eeprom_indexes::Eeprom_indexes(int new_start_i, int new_end_i) {
  start_i = new_start_i;
  end_i = new_end_i;
  short cur, num;
  //get current index and the count of data points
  EEPROM.get(new_start_i, cur);
  EEPROM.get(new_start_i+sizeof(short), num);
  curr_i = cur;
  stored_data_count = num;

  is_underflow = false;
  actual_start_i = (new_start_i + (2*sizeof(short)));
  if (curr_i < actual_start_i) curr_i = actual_start_i;
}

void Eeprom_indexes::store_data(Data_To_Store data_to_store) {
  // Serial.println("debug for loop runs for delayed temperature recording inn eeprom");
  EEPROM.put(curr_i, data_to_store);
  if (!is_underflow) {
    stored_data_count++;
  }
  curr_i += sizeof(Data_To_Store);

  if (curr_i > (end_i - sizeof(Data_To_Store))) {
    curr_i = actual_start_i;
    is_underflow = true;
  }
  EEPROM.put(start_i, curr_i);
  EEPROM.put(start_i+sizeof(int), stored_data_count);
}

void Eeprom_indexes::init() {
  //initialize the class
  curr_i = actual_start_i;
  stored_data_count = 0;
  is_underflow = false;
}


Data_To_Store Eeprom_indexes::get_ith_data(int x) {
  if (stored_data_count == 0) return Data_To_Store();
  if (x >= stored_data_count) x = stored_data_count - 1;
  if (x < 0) x = 0;

  Data_To_Store ret;
  int read_i = curr_i;
  read_i -= ((x+1)*sizeof(Data_To_Store));
  if (read_i < actual_start_i) {
    read_i += sizeof(Data_To_Store)*(int)((end_i - actual_start_i) / sizeof(Data_To_Store));
  }
  EEPROM.get(read_i, ret);
  return ret;
}

void Eeprom_indexes::print_data(int x, int8_t is_udp) {
  int read_i = curr_i;
  if (stored_data_count == 0) {
    Serial.println("No data");
    return;
  }
  Serial.print(stored_data_count);
  Serial.println(" data points.");
  Serial.print("Current index is ");
  Serial.println(curr_i);



  if (x > stored_data_count) x = stored_data_count;
  for (int i = 0; i < x; i++) {
    //getting the numbers from the bitwise
    byte ret_year = get_ith_data(i).get_year();
    byte ret_month = get_ith_data(i).get_month();
    byte ret_day = get_ith_data(i).get_day();
    byte ret_hour = get_ith_data(i).get_hour();
    byte ret_minute = get_ith_data(i).get_minute();
    byte ret_second = get_ith_data(i).get_second();
    int8_t ret_temp = get_ith_data(i).get_temp();
    byte ret_hum = get_ith_data(i).get_hum();
    //


    Serial.print(i+1);
    Serial.print("\t");
    Serial.print(ret_year);
    Serial.print("/");
    Serial.print(ret_month);
    Serial.print("/");
    Serial.print(ret_day);
    Serial.print(" ");
    Serial.print(ret_hour);
    Serial.print(":");
    Serial.print(ret_minute);
    Serial.print(":");
    Serial.print(ret_second);
    Serial.print(" ");
    Serial.print(ret_temp);
    Serial.print("C (");
    Serial.print(to_farenheit(ret_temp));
    Serial.print("F), ");
    Serial.print(ret_hum);
    Serial.println("%");

    if (is_udp) {
      char buff[8];
      Udp.beginPacket(ip_remote, remotePort);
      itoa(ret_year, buff, 10);
      Udp.write(buff);
      Udp.write("/");
      itoa(ret_month, buff, 10);
      Udp.write(buff);
      Udp.write("/");
      itoa(ret_day, buff, 10);
      Udp.write(buff);
      Udp.write(" ");
      itoa(ret_hour, buff, 10);
      Udp.write(buff);
      Udp.write(":");
      itoa(ret_minute, buff, 10);
      Udp.write(buff);
      Udp.write(":");
      itoa(ret_second, buff, 10);
      Udp.write(buff);
      Udp.write(" ");
      itoa(ret_temp, buff, 10);
      Udp.write(buff);
      Udp.write("C (");
      itoa(to_farenheit(ret_temp), buff, 10);
      Udp.write(buff);
      Udp.write("F), ");
      itoa(ret_hum, buff, 10);
      Udp.write(buff);
      Udp.write("%");
      udp_packets_out_counter++;
      Udp.endPacket();
    }

  }

}
