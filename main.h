#include <Arduino.h>
#include <EEPROM.h>
#include <DS3231_Simple.h>
#include <Wire.h>
#include <SimpleDHT.h>
#include <FastLED.h>

#include <Ethernet.h>
#include <EthernetUdp.h>


#ifndef MY_GLOBALS_H


#define MAX_STR 12
#define MAX_CMD_COUNT 5
#define DUAL_LED_PIN1 4
#define DUAL_LED_PIN2 5
#define MIN_DELAY 50
#define CUR_VERSION 2.3
#define BAUD_RATE 9600
#define UDP_LISTEN_DELAY 10

//pin and setup for the DHT
#define PINDHT22 2
#define NUM_LEDS 1
#define RGB_DATA_PIN 6

extern CRGB leds[NUM_LEDS];
extern CRGB color1, color2;

extern void take_input();
extern void take_input_udp();
extern void execute_commands(byte is_udp);
extern void read_temp_hum_loop();
extern void print_EEPROM_data(int x);
extern void reset_EEPROM_data();
extern int8_t to_farenheit(int8_t x);
extern void blink_LED(byte &x);
extern void blink_RGB();
extern bool is_str_number(char command[], int &ret);
extern void set_command_flag(char command[], int arr[]);

//when adding a menu item, add in 4 places, enum, help, set_command_flag(), execute_commands()
typedef enum {M_LED = -128, M_SET, M_STATUS, M_VERSION,
	M_HELP, M_LEDS, M_GREEN, M_RED, M_DUAL,
/*M_ON,*/ M_OFF, M_BLINK/*, M_D13*/, M_WRITE, M_READ, M_RTC,
M_DHT, M_CURRENT, M_SAVED, M_EXTREME, M_ADD, M_RGB, M_RESET, M_INVALID = -32768} MENU_ITEMS;
extern MENU_ITEMS menu_items;

extern SimpleDHT22 dht22;
//for buffer
extern int arr[MAX_CMD_COUNT];
extern char command[MAX_STR+1];
//input processing variables
extern byte command_size;
extern byte command_count;
extern char inByte;

//timekeeping variables
extern DS3231_Simple Clock;
extern unsigned int curr_time, prev_time2, prev_time3, prev_time_udp, prev_time_dht_short, prev_time_dht_long;

extern unsigned int blink_delay2, blink_delay3, dht_read_short_delay;
extern unsigned int dht_read_long_delay;

//toggles for blinking options
// extern bool blinkD13toggle;

//for keeping track fo the current color for blinking
extern byte dual_led_binary;

//keeping current temp and humidity in global
extern int8_t cur_temp;
extern int8_t cur_humidity;
extern int8_t max_temp;
extern int8_t min_temp;
extern int8_t max_humidity;
extern int8_t min_humidity;

extern int8_t temp_threshold_1, temp_threshold_2, temp_threshold_3, temp_threshold_4, current_threshold;



// The IP address will be dependent on your local network:
extern byte mac[];
extern IPAddress ip;
extern unsigned int localPort;
extern IPAddress ip_remote;
extern unsigned int remotePort;// local port to listen on
// buffers for receiving and sending data
extern char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; // buffer to hold incoming packet,
extern char ReplyBuffer[];// a string to send back
// An EthernetUDP instance to let us send and receive packets over UDP
extern EthernetUDP Udp;


class Data_To_Store {
public:
	DateTime date_time;
	int8_t temp;
	int8_t humidity;
};

class Eeprom_indexes {
private:
	int start_i;
	int end_i;
	int curr_i;
	int stored_data_count;
	int actual_start_i;
	bool is_underflow;
public:
	int get_start_i();
	int get_end_i();
	int get_curr_i();
	int get_stored_data_count();
	Eeprom_indexes(int new_start_i, int new_end_i, int new_curr_i, int new_stored_data_count);
	void store_data(Data_To_Store data_to_store);
	void print_data(int x);
	void init();
};




#endif
