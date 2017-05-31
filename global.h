#ifndef GLOBAL_H
#define GLOBAL_H

#define FIRMWARE 	"v3.23 2017-05-13"
String fwVers = "v9.99";

#include "password.h"

IPAddress ip9    	(192, 168, 179, 200);
IPAddress esp_ip   	(0, 0, 0, 0);
IPAddress dns   	(192, 168, 179, 1  );
IPAddress gateway 	(192, 168, 179, 1  );
IPAddress subnet  	(255, 255, 255, 0  );
IPAddress ip0   	(0, 0, 0, 0);
IPAddress dns0    	(0, 0, 0, 0);
IPAddress gateway0  (0, 0, 0, 0);
String esp_ip_str = "noipyet";

IPAddress ipMulti = IPAddress(192,168,178,255);
unsigned int portMulti = 45131;

// We need to include Wire.h for I2C communication
#include <Wire.h>
#define OLED_SDA	5 	// D1 = GPIO  5
#define OLED_SCL	4 	// D2 = GPIO  4
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
SSD1306  display(0x3c, OLED_SDA, OLED_SCL);
boolean OLED_outputOn = false;

#include <ChainableLED.h> // chainable serial MOSFET Module P9813
#define P9813_NUM_LEDS  1 // ChainableLED.h - only one RGB will be used
#define	P9813_CLK		9  // 4 	// D2 = GPIO  4
#define P9813_DATA		10 // 5 	// D1 = GPIO  5
//ChainableLED(byte clk_pin, byte data_pin, byte number_of_leds)
ChainableLED p9813(P9813_CLK, P9813_DATA, P9813_NUM_LEDS);


#include <E131.h>
E131 e131;

#include <ESPDMX.h>
DMXESPSerial dmx; // output on GPIO2 = D4 = TxD1

// GPIO15 = GND (always)
// GPIO00 = GND when flashing
// D0 = GPIO16 will cause conflict - Belongs to the RTC module, not the general GPIO module,

// static const uint8_t LED_BUILTIN = 16;
uint8_t pLED_RED 	= 14;	// D5 = GPIO 14
uint8_t pLED_GREEN 	= 13;	// D7 = GPIO 13
uint8_t pLED_BLUE 	= 12;	// D6 = GPIO 12
uint8_t pLED_RED2 	= 15;	// D8 = GPIO 15 // not for ESP12E !!!

// 			Set 	Element
#define LED_PIN_SETS	4
uint8_t LED_PIN[LED_PIN_SETS][4] =
						{// R  	G  	B 	R2
						{	14,	13,	12,	15 }, // NodeMCU with MOSFET
						{	13,	12,	14,	15 }, // ESP12E mini board
						{	12,	13,	14,	15 },
						{	13,	14,	12,	15 } // ESP12E mini board
						};

// library file ESPDMX.cpp
// int sendPin = 2;		// GPIO2 = D4 = TxD1 - default on ESP8266
#define LED_STATUS 	2 	// D4 = GPIO  2
// #define LED_STATUS 	9 	// SDs = GPIO  9
#define DMX_STATUS 	10 	// D12 = GPIO  10
uint8_t LEDs[] = {pLED_RED,pLED_GREEN,pLED_BLUE};
uint8_t LEDcnt = sizeof(LEDs);

// DMX Sender
uint8_t DMXsequence = 1;
#define Button1 0	// D3  = GPIO  0
#define Button2 D10	// D10 = GPIO  xx
#define Button3 D9	// D9  = GPIO  xx

int			DMXGammaPerc 	= 100;		// correct all channels with this factor
uint8_t 	DMXMin 	= 0;
uint8_t 	DMXMax 	= 255;

String DMX_Actor[] = {
		"Front",	// 0
		"Back",		// 1
		"Spot1",	// 2
		"Spot2",	// 3
		"Laser",	// 4
		"Kim",		// 5
		"Lale",		// 6
		"Swantje",	// 7
		"Liz",		// 8
		"No5",		// 9
		"No6",		// 10
		"No7",		// 11
		"No8"		// 12
};
String DMX_Module[] = {
		"Wheel",
		"Post",
		"Body",
		"Legs"
};


void DMX_Init(void);
void OLED_init(void);
void Led_Init(void);
void Led_Test(void);
void Led_Off(void);
void Battery_Init(void);
void NodeServer_ping_Multicast	(void);
void NodeServer_text_Multicast	(String sMessage);
void esp_reset(void);
void e131_send_rgb_all		(const uint8_t dRed,const uint8_t dGreen,const uint8_t dBlue);
void e131_send_rgb_set		(const uint8_t DmxSet, const uint8_t dRed,const uint8_t dGreen,const uint8_t dBlue);
void e131_send_rgb_set_ext	(const uint8_t DmxSet, const uint8_t dRed,const uint8_t dGreen,const uint8_t dBlue);
void e131_test_rgb(void);

/* Debugging */
#define DEBUGGING(...) Serial.println( __VA_ARGS__ )
#define DEBUGGING_L(...) Serial.print( __VA_ARGS__ )

/* DXM */
extern uint16_t universe = 2,  universeTX = 2;
#define DMX_SET     12
extern uint8_t 	dmxCh			= 0;
extern uint8_t 	dmxStartChannel	= 0;
extern uint16_t dmxGammaPerc 	= 100;	// correct all channels with this factor
extern uint8_t 	dmxMin 			= 0;
extern uint8_t 	dmxMax 			= 255;
unsigned long 	uiDxmPowerOffTime = 2000;

boolean 	dmxTxLoop 	= false;
uint8_t		dmxTxRed 	= 50;
uint8_t		dmxTxGreen 	= 0;
uint8_t		dmxTxBlue	= 0;
uint8_t		dmxTxRed2 	= 0;

uint16_t 	uiDMXRed 	= 0;	// raw DMX data
uint16_t 	uiDMXGreen  = 0;	// raw DMX data
uint16_t 	uiDMXBlue 	= 0;	// raw DMX data
uint16_t 	uiDMXRed2 	= 0;	// raw DMX data

uint8_t 	iLedRed 	= 0; // temp data
uint8_t 	iLedGreen 	= 0; // temp data
uint8_t		iLedBlue 	= 0; // temp data
uint8_t 	iLedRed2 	= 0; // temp data

int 		DMX_RED 	= 0;
int 		DMX_GREEN 	= 1;
int 		DMX_BLUE 	= 2;
int 		DMX_RED2 	= 3;

int 		iRetVal 	= false;
boolean 	udpActive 	= false;
boolean 	e131Active 	= false;
boolean 	testmode 	= false;
boolean		testDmxRGB	= false;
boolean		testButton	= false;
boolean		node_json 	= true;

String 		rssi_qual 	= "really unknown";
int 		rssi_bars 	= 0;
int32_t 	rssi_value = 0;
int 		rssi_percent= 0;

String 		esp_status 	= "really unknown";
uint8_t     esp_level	=0;

extern int 	iLED = 0;
extern int 	iBrightness = 0;
extern int 	incr = 1;
boolean 	bStartup = true;		// On firststart = true, set ports

// ---------------------------------------------------------- battery monitor
// #include <Battery.h>
#define ANALOG_PIN 			A0 		// read, input
#define DIVIDER_RATIO 		4.3		// Bat+ --- 3k3 --- A0 --- 1k --- GND
#define ANALOG_REF_mV 		3333	// NodeLUA - A0 up tp 3.3V
#define DIVIDER_RATIO_E 	11.0  	// Bat+ --- 10k --- A0 --- 1k --- GND
#define ANALOG_REF_mV_E 	1070	// ESP12E - A0 up tp 1.0V

#define LIPO_mV_FULL    8340  	//                           Lipo: 1S full 4,2; empty 3,3V
#define LIPO_mV_EMPTY 	7600	// Need 7.6V for blue LED  - Lipo: 2S full 8,34; empty 6,6V
#define SENSE_PIN 0xFF // activiation Pin / output; set to 0xFF to ignore
float   	fVoltLipo	=0;
uint16_t	uiVoltAnalog=0;
uint8_t     uiVoltLevel=0;
boolean		battery_critical = false; // used to disable WiFi

String MessageString;

unsigned long udpPacketCount = 0, udpPacketErrorCount = 0;
unsigned long tempTimer, flashTimer, powerOffTimer, aliveTimer, aliveE131Timer, noWiFiTimer, batteryTimer;
unsigned long lastTimeHost = 0, lastTimeMaster = 0, lastTimeUdp = 0;
int retval ;

#define WIFI_CONNECT_TIMEOUT 20000  /* 10 seconds */

ESP8266WebServer server(80);			// The Webserver

boolean 	firstStart 		= true;		// On firststart = true, NTP will try to get a valid time
int 		AdminTimeOutCounter = 0;	// Counter for Disabling the AdminMode
strDateTime DateTime;					// Global DateTime structure, will be refreshed every Second
WiFiUDP 	UDPNTPClient;				// NTP Client
unsigned long UnixTimestamp = 0;		// GLOBALTIME  ( Will be set by NTP)
boolean 	Refresh 		= false; 	// For Main Loop, to refresh things like GPIO / WS2812
int 		cNTP_Update 	= 0;		// Counter for Updating the time via NTP
Ticker 		tkSecond;					// Second - Timer for Updating Datetime Structure
boolean 	AdminEnabled 	= false;	// Enable Admin Mode for a given Time
boolean 	SoftAPup 		= false;	// Enable Admin Mode for a given Time
unsigned int WiFiConnectCnt = 0;
byte 		Minute_Old 		= 100;		// Helpvariable for checking, when a new Minute comes up (for Auto Turn On / Off)


#define update_server 		"192.168.179.4"
#define firmware_version	FIRMWARE
// firmware_version room"_messwerte_0.X.Y"
#define update_php 			"/ucls/updateFW.php"
#define update_bin 			"/ucls/ESP12E_E131_DMX.bin"
#define update_file 		"http://192.168.179.4/ucls/update/ESP12E_E131_DMX.bin"


struct strConfig {
	String 	ssid;
	String 	password;
	byte  	IP[4];
	byte  	Netmask[4];
	byte  	Gateway[4];
	boolean dhcp;
	String 	ntpServerName;
	long 	Update_Time_Via_NTP_Every;
	long 	timezone;
	boolean daylight;
	boolean AutoTurnOff;
	boolean AutoTurnOn;
	byte 	TurnOffHour;
	byte 	TurnOffMinute;
	byte 	TurnOnHour;
	byte 	TurnOnMinute;
	byte 	LED_R;
	byte 	LED_G;
	byte 	LED_B;
	unsigned long deviceNum;
	unsigned long moduleNum;
	unsigned long universe;
	unsigned long DxmPowerOffTime;
	boolean DMXoutputOn;
	byte 	LEDconfig;
	unsigned long batAnalogRef;
	float 	batDividerRatio;
	boolean P9813outputOn;
	boolean OLEDoutputOn;
	String 	Firmware;
	String 	DeviceName;
}   config;

void WriteConfig(void);
void EraseConfig(void);
void ConfigureWifi(void);
void dmx_serial_init(void);

#endif
