#include "main.h"
SimpleDHT22 dht22(PINDHT22);
//for buffer
int arr[MAX_CMD_COUNT];
char command[MAX_STR+1];
CRGB leds[NUM_LEDS];
CRGB leds_temp[NUM_LEDS];
CRGB leds_link[NUM_LEDS];

CRGB color1 = CRGB(0, 0, 50);
CRGB color2 = CRGB::Black;
CRGB color_maj_und = CRGB(128, 0, 128);//purple
CRGB color_min_und = CRGB(0, 0, 128);//blue
CRGB color_comfortable = CRGB(0, 128, 0);//green
CRGB color_min_ovr = CRGB(200, 120, 0);//orange
CRGB color_maj_ovr = CRGB(200, 0, 0);//red
//input processing variables
byte command_size = 0;
byte command_count = 0;
char inByte = 0;

//timekeeping variables
DS3231_Simple Clock;
unsigned int curr_time = 0, prev_time3 = 0, prev_time_udp = 0, prev_time_udp_checker = 0, prev_time_dht_short = 0, prev_time_dht_long = 0;
bool link_status = false;

unsigned int  blink_delay3 = 500;

//keeping current temp and humidity in global
int8_t cur_temp = 0;
int8_t cur_humidity = 0;
int8_t max_temp = INT8_MIN;
int8_t min_temp = INT8_MAX;
int8_t max_humidity = INT8_MIN;
int8_t min_humidity = INT8_MAX;
int8_t current_threshold = 2;

// The IP address will be dependent on your local network:
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 177);
IPAddress ip_remote(192, 168, 1, 111);   // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged";        // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;


void setup() {
	Ethernet.init(10);
	Ethernet.begin(mac, ip);

	Serial.begin(BAUD_RATE);
	 // wait for serial port to connect. Needed for native USB port only
	while (!Serial) {;}
	// Check for Ethernet hardware present
  Udp.begin(localPort);


	Clock.begin();
	FastLED.addLeds<NEOPIXEL, RGB_DATA_PIN>(leds, NUM_LEDS);
	FastLED.addLeds<NEOPIXEL, RGB_DATA_PIN_TEMP>(leds_temp, NUM_LEDS);
	FastLED.addLeds<NEOPIXEL, RGB_DATA_PIN_LINK>(leds_link, NUM_LEDS);
	leds[0] = color1;
	leds_temp[0] = color_comfortable;
	FastLED.show();
	command[0] = '\0';

	pinMode(13, OUTPUT);

	//power on led
	pinMode(DUAL_LED_PIN1, OUTPUT);
	pinMode(DUAL_LED_PIN2, OUTPUT);
	digitalWrite(DUAL_LED_PIN1, HIGH);
	digitalWrite(DUAL_LED_PIN2, LOW);

	Serial.println(F("Enter commands or 'HELP'"));
}

void loop() {
	curr_time = millis();
	read_temp_hum_loop();

	//blink the LEDs, the functions account for the delay
	blink_RGB();

	//calls set flags and execute
	take_input();
	take_input_udp();
}
