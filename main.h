#include <Arduino.h>



#ifndef MY_GLOBALS_H


#define MAX_STR 12
#define MAX_CMD_COUNT 3
#define DUAL_LED_PIN1 4
#define DUAL_LED_PIN2 5
#define MIN_DELAY 50
#define CUR_VERSION 2.0
#define BAUD_RATE 9600

//pin and setup for the DHT
#define PINDHT22 2

extern void take_input();
extern void execute_commands();
extern void read_temp_hum_loop();
extern void print_EEPROM_data(int x);
extern int8_t to_farenheit(int8_t x);
extern void change_dual_led(byte x);
extern void blink_LED();
extern void blink_d13();
extern bool is_str_number(char command[], int &ret);
extern void set_command_flag(char command[], int arr[]);

//when adding a menu item, add in 4 places, enum, help, set_command_flag(), execute_commands()
typedef enum {M_LED, M_SET, M_STATUS, M_VERSION,
	M_HELP, M_LEDS, M_GREEN, M_RED, M_DUAL,
M_ON, M_OFF, M_BLINK, M_D13, M_WRITE, M_READ, M_RTC,
M_DHT, M_CURRENT, M_SAVED, M_EXTREME, M_ADD, M_INVALID = -32768} MENU_ITEMS;
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
extern unsigned int curr_time, prev_time1, prev_time2, prev_time_dht_short, prev_time_dht_long;

extern unsigned int blink_delay, dht_read_short_delay;
extern unsigned int dht_read_long_delay;

//toggles for blinking options
extern bool blinkD13toggle, blinkLEDtoggle, dual_blink;

//for keeping track fo the current color for blinking
extern byte current_color, blink_color;

//keeping current temp and humidity in global
extern int8_t cur_temp;
extern int8_t cur_humidity;
extern int8_t max_temp;
extern int8_t min_temp;
extern int8_t max_humidity;
extern int8_t min_humidity;


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
};




#endif
