/*--------------------------------------------------
ESP8266 ArtNet Receiver

Hardware: ESP12E / ESP07

Module will connect to WiFI,
Once connection is established, module is waiting for udp packets to get E1.31 packages containing  DMX data

Software based on the source provided by
https://github.com/forkineye/E131
http://www.john-lassen.de/en/projects/esp-8266-arduino-ide-webconfig
http://www.whatimade.today/esp8266-on-websockets-mdns-ota-and-leds

--------------------------------------------------*/

/*
 * TO DO
 *
 * Do not blink after the show has started
 *
 * WiFi reconnect: E131.cpp::beginMulticast - initWiFi(ssid, passphrase)
 * when first connect successful
 *    - store IP adddress
 *    - store WiFi channel    wifi.getchannel();	// Gets the current WiFi channel.
 *    - store AP MAC address
 * 	re-connect
 * 	  - use static IP and channel
 * 	  -	fallback if timeout is reached -> foregt static IP and channel
 * 	  - measure time to connect
 */

#define DEVICE_NAME "ucl-01-spot" // default name for factory reset
// Im Gegensatz zu Domainnamen dürfen Hostnames nur aus den ASCII-Zeichen a–z bzw. A–Z (zwischen Groß- und Kleinbuchstaben wird nicht unterschieden),
// den Ziffern 0–9 und
// dem Bindestrich-Minus - bestehen

#define WIFI_CONNECT_TIMEOUT 20000  /* 20 seconds */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266httpUpdate.h>

#include <WiFiClient.h>
#include <Ticker.h>
#include <EEPROM.h>
#include "helpers.h"
#include "global.h"

/*
Include the HTML, STYLE and Script "Pages"
*/
#include "Page_Default.h"
#include "Page_Admin.h"
#include "Page_Script.js.h"
#include "Page_Style.css.h"
#include "Page_NTPsettings.h"
#include "Page_Information.h"
#include "Page_General.h"
#include "PAGE_NetworkConfiguration.h"
#include "PAGE_DMX.h"
#include "PAGE_DMX_Sender.h"

//Globals
ESP8266WebServer 		httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

#include "E131Constants.h"
#include <SPI.h>
#include <WiFiUdp.h>

#include <ArduinoJson.h>
// StaticJsonBuffer<511> jsonBuffer;
DynamicJsonBuffer jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

// ------------------------------------------------------------------------------------- dmx_serial_init
// initialize DMX output if required
void dmx_serial_init() {
	if (config.DMXoutputOn == true)
	{
		dmx.init(32); // use 32 channels i/o 512
		for (int thisChannel = DMX_RED; thisChannel <= (DMX_RED+4); thisChannel++)
			dmx.write(thisChannel+1, 0);
		dmx.update();
	}
} // dmx_serial_init()

// ------------------------------------------------------------------------------------- HTTPUpdateConnect
// start update server protected with password defined in global.h
void HTTPUpdateConnect() {
  httpUpdater.setup(&server, UPDATE_USER_NAME, UPDATE_USER_PASSWORD);
  server.begin();
  DEBUGGING_L("> HTTPUpdateServer ready! Open http://");
  DEBUGGING_L(config.DeviceName);
  DEBUGGING(".local/update in your browser\n");
} // HTTPUpdateConnect()

// ------------------------------------------------------------------------------------- MDNSConnect
// MDNS - currently not used in this application
void MDNSConnect() {
	// strcpy(hostString, "goodbye");
	// strcat(hostString, config.DeviceName);
	char hostString[] = DEVICE_NAME;

   if (!MDNS.begin(hostString)) {
	   DEBUGGING("> ERROR setting up MDNS responder!");
	   while (1) {
		   delay(1000);
	   }
  }
  DEBUGGING_L("> mDNS responder started, name ");
  DEBUGGING  (hostString);
  MDNS.addService("ws", "tcp", 81);
  MDNS.addService("http", "tcp", 80);
  DEBUGGING("> mDNS services ws-81 and http-80 started");
  // digitalWrite(LED_GREEN,255);
}

// ------------------------------------------------------------------------------------- Battery_Init
// initialize the paramaters for battery measurement
// a little paranoid but I want to protect a division by 0
void Battery_Init() {

	pinMode(ANALOG_PIN, INPUT);
	if(config.LEDconfig == 0) // NodeMCU LUA module
	{
		config.batAnalogRef 	= ANALOG_REF_mV;
		config.batDividerRatio 	= DIVIDER_RATIO;
	}
	else // ESP12E module w/o board
	{
		config.batAnalogRef 	= ANALOG_REF_mV_E;
		config.batDividerRatio 	= DIVIDER_RATIO_E;
	}
	DEBUGGING("> battery - init ");

} // Battery_Init()

// ------------------------------------------------------------------------------------- LedInit
// set ports for PWM output
// ports can be changed on WEB page Page_DMX.h
void Led_Init() {

	pLED_RED  	= LED_PIN[config.LEDconfig][0];
	pLED_GREEN 	= LED_PIN[config.LEDconfig][1];
	pLED_BLUE 	= LED_PIN[config.LEDconfig][2];
	pLED_RED2	= LED_PIN[config.LEDconfig][3];

	if(bStartup==true)
	{
		pinMode(LED_STATUS, OUTPUT);
		pinMode(pLED_BLUE, OUTPUT);
		pinMode(pLED_GREEN,OUTPUT);
		pinMode(pLED_RED, 	OUTPUT);
	}

	DEBUGGING_L("> PIN mode set - config: ");
	DEBUGGING(config.LEDconfig);

} // LedInit()

// ------------------------------------------------------------------------------------- Led_Test
// test all LEDs by fading in and out
void Led_TestLoop() {

    if(iLED == LEDcnt) // last LED in array exceeded
    	iLED = 0;

    digitalWrite(LEDs[iLED], iBrightness);
    if(P9813_NUM_LEDS>0 && iLED == 0) p9813.setColorRGB(0, iBrightness, 0, 				0);
    if(P9813_NUM_LEDS>0 && iLED == 1) p9813.setColorRGB(0, 0, 			iBrightness, 	0);
    if(P9813_NUM_LEDS>0 && iLED == 2) p9813.setColorRGB(0, 0, 			0, 				iBrightness);
    // iLED ++;
    iBrightness += incr;

    if(iBrightness > 200 && incr > 0 ) // decrease brightness when max brightness is reached
    {
    	incr = -5;
    	iBrightness += incr;
    }
    if(iBrightness < 0 && incr < 0 ) // decrease brightness when max brightness is reached
    {
    	incr = 5;
    	iBrightness += incr;
    	iLED++;
    }

    // DEBUGGING(iBrightness);
    delay(40);

} // Led_Test()

// ------------------------------------------------------------------------------------- Led_Blink
// swtich LED on and off
// used when powering up the module to visualize it is operating
void Led_Blink(int iLED, int brightness, int duration) {

	digitalWrite(iLED,brightness);
	if(iLED == pLED_RED)
		if(P9813_NUM_LEDS>0) p9813.setColorRGB(0, brightness, 0, 0);
	if(iLED == pLED_GREEN)
		if(P9813_NUM_LEDS>0) p9813.setColorRGB(0, 0, brightness, 0);
	if(iLED == pLED_BLUE)
		if(P9813_NUM_LEDS>0) p9813.setColorRGB(0, 0, 0, brightness);

	delay(duration);

	digitalWrite(iLED,0);
	if(P9813_NUM_LEDS>0) p9813.setColorRGB(0, 0, 0, 0);

} // Led_Blink()

// ------------------------------------------------------------------------------------- Led_Off
// switch all LEDs off
void Led_Off() {

	analogWrite(pLED_RED,		0);
	analogWrite(pLED_GREEN,		0);
	analogWrite(pLED_BLUE,		0);
	analogWrite(pLED_RED2, 		0);

	if(P9813_NUM_LEDS>0) p9813.setColorRGB(0, 0, 0, 0);

	if (config.DMXoutputOn == false)
		digitalWrite(LED_STATUS, 0);
	DMX_LED_off();


} // LedInit()

// ------------------------------------------------------------------------------------- Led_Set
// set LED to a specific value
void Led_Set(int iRed,  int iGreen, int iBlue) {

	analogWrite(pLED_RED,		iRed);
	analogWrite(pLED_GREEN,		iGreen);
	analogWrite(pLED_BLUE,		iBlue);

	iLedRed   = iRed;
	iLedGreen = iGreen;
	iLedBlue  = iBlue;

	if(P9813_NUM_LEDS>0) p9813.setColorRGB(0, iRed, iGreen, iBlue);

    if (config.DMXoutputOn == true)
    {
        dmx.write(1, iRed	);
        dmx.write(2, iGreen	);
        dmx.write(3, iBlue	);
        dmx.update();
    }

} // Led_Set()

// ------------------------------------------------------------------------------------- LedInit
// switch the LED on/off, one after the other RED,GREEN,BLUE
void Led_Test() {

	int iDelay = 400;
	DEBUGGING("> blink ...");
	esp_status 	= "LED test";
	esp_level	= 75;
	NodeServer_text_Multicast("LED test");

	digitalWrite(LED_STATUS, LOW);
	Led_Off();
	delay(iDelay);

	esp_status = "LED TEST RED";
	digitalWrite(LED_STATUS, HIGH);
	Led_Set(255,0,0);
	OLED_status_DMX();
	delay(iDelay);

	esp_status = "LED TEST GREEN";
	digitalWrite(LED_STATUS, LOW);
	Led_Set(0,255,0);
	OLED_status_DMX();
	delay(iDelay);

	esp_status = "LED TEST BLUE";
	digitalWrite(LED_STATUS, HIGH);
	Led_Set(0,0,255);
	OLED_status_DMX();
	delay(iDelay);

	esp_status = "LED TEST OFF";
	digitalWrite(LED_STATUS, LOW);
	Led_Set(0,0,0);
	OLED_status_DMX();
	delay(iDelay);
}

// ------------------------------------------------------------------------------------- Led_TestStartup
// switch the LED on/off, one after the other RED,GREEN,BLUE
void Led_TestStartup() {
	int iDelay = 400;
	DEBUGGING("> blink ...");

	digitalWrite(LED_STATUS, LOW);
	Led_Off();

	digitalWrite(LED_STATUS, HIGH);
	Led_Set(1,0,0);
	delay(iDelay);

	digitalWrite(LED_STATUS, LOW);
	Led_Set(0,0,0);
}


// ------------------------------------------------------------------------------------- server_reply
void server_reply(String message){

	String msg = "<!DOCTYPE HTML><html><head><title>";
	msg += config.DeviceName;
	msg += "</title></head><body>";
	msg += "<h1>";
	msg += config.DeviceName;
	msg += "</h1><h3>";
	msg += message;
	msg += "</h3></body></html>";

	NodeServer_text_Multicast(message);
	server.send ( 200, "text/html", msg );
}

// ------------------------------------------------------------------------------------- firwmareUpdateFixed
void firwmareUpdateFixed()
{
	String MessageString;


	  Serial.println ("------------------------------------------------------------ firmware update");
	  Serial.println (update_file);
	  esp_status = "FLASH";
	  esp_level	= 0;

	  MessageString  = " Firmware update: ";
	  MessageString += update_file;
	  NodeServer_text_Multicast(MessageString);

	  server_reply(MessageString);

	  t_httpUpdate_return ret = ESPhttpUpdate.update(update_file); //, "", ""); //, firmware_version);
	  switch(ret) {
	    case HTTP_UPDATE_FAILED:
	        Serial.println("[update] Update failed.");
	        Serial.print(F("Update error "));
	        Serial.print(ESPhttpUpdate.getLastError());
	        Serial.print(" = ");
	        Serial.print(ESPhttpUpdate.getLastErrorString());
	        Serial.println();
	        break;
	    case HTTP_UPDATE_NO_UPDATES:
	        Serial.println("[update] Update no Update.");
	        break;
	    case HTTP_UPDATE_OK:
	        Serial.println("[update] Update ok."); // may not called we reboot the ESP
	        break;
	  }
	  esp_level	= 30;
	  NodeServer_text_Multicast("update failed");
}

// ------------------------------------------------------------------------------------- firwmareUpdatePHP
void firwmareUpdatePHP()
{
	  Serial.println ("------------------------------------- firmware update");
	  Serial.println (update_file);
	  esp_status = "FLASH";
	  esp_level	= 0;

	  Serial.print (update_server);
	  Serial.print (" - ");
	  Serial.println(update_php);

	  t_httpUpdate_return ret = ESPhttpUpdate.update(update_server, 80, update_php, ""); //, firmware_version);
	  switch(ret) {
	    case HTTP_UPDATE_FAILED:
	        Serial.println("[update] Update failed.");
	        break;
	    case HTTP_UPDATE_NO_UPDATES:
	        Serial.println("[update] Update no Update.");
	        break;
	    case HTTP_UPDATE_OK:
	        Serial.println("[update] Update ok."); // may not called we reboot the ESP
	        break;
	  }
}


// ------------------------------------------------------------------------------------- server_init
// attach the WEB pages to the server
void e131_send_10s(int iRed, int iGreen, int iBlue)
{
	uint32_t timeout = millis();
	iLedRed 	= iRed; // temp data
	iLedGreen 	= iGreen; // temp data
	iLedBlue 	= iBlue; // temp data
	iLedRed2 	= 0; // temp data

	esp_status 	= "e131 test";
	esp_level	= 30;
	NodeServer_text_Multicast("test");

	unsigned long cntTimer = millis(), deltaTime=0, deltaTimePrior=0;


	while (true) {
		delay(40);
		e131_send_rgb_all(0,0,0);
		if (millis() - timeout > 10000) {
            break;
        }
        deltaTime = millis() - cntTimer;
        deltaTime /= 1000;
        if (deltaTime != deltaTimePrior){ // change message every second
          	NodeServer_text_Multicast((String)deltaTime);
          	OLED_print(deltaTime*7, 54, (String) deltaTime);
           	deltaTimePrior = 	deltaTime;
        }
        yield();
	} // while
}


// ------------------------------------------------------------------------------------- server_init
// attach the WEB pages to the server
void esp_reset()
{
	String MessageString = "RESET...";
	esp_status 	= "RESET";
	esp_level	= 0;
	NodeServer_text_Multicast(MessageString);
	delay(50);
	ESP.restart	();
}

// ------------------------------------------------------------------------------------- json_on
// attach the WEB pages to the server
void json_on()
{
	node_json = true;
}
void json_off()
{
	node_json = false;
}

// ------------------------------------------------------------------------------------- server_init
// attach the WEB pages to the server
void server_init()
{
	server.on ( "/", processExample  );

	server.on ( "/update-pull", firwmareUpdateFixed  );
	server.on ( "/update-php" , firwmareUpdatePHP    );

	server.on ( "/test" ,  			[]() { 	server_reply("LED test"					); Led_Test		();   		}  );
	server.on ( "/reset",   		[]() { 	server_reply("RESET"                 	); esp_reset	();   		}  );
	server.on ( "/off"  ,   		[]() { 	server_reply("switch all LEDs off"   	); Led_Off		();   		}  );
	server.on ( "/on"	, 	  		[]() { 	server_reply("set LED to 1"			   	); Led_Set		(1,0,0	);  }  );
	server.on ( "/on100", 	  		[]() { 	server_reply("set LED to 100"		   	); Led_Set		(100,0,0);  }  );
	server.on ( "/e131test", 		[]() { 	server_reply("send E1.31 for 10s ..."	); e131_send_10s(0,0,0);	}  );
	server.on ( "/json-on"  , 		[]() { 	server_reply("JSON ON"   				); node_json = true;   		}  );
	server.on ( "/json-off"  , 		[]() { 	server_reply("JSON OFF"   				); node_json = false;   	}  );

	server.on ( "/admin/filldynamicdata", filldynamicdata );

	server.on ( "/favicon.ico",   	[]() { 	server.send ( 200, "text/html", "" );   }  );

	server.on ( "/admin.html", 		[]() {  server.send ( 200, "text/html", PAGE_AdminMainPage );   }  );
	server.on ( "/config.html", 			send_network_configuration_html );
	server.on ( "/info.html", 		[]() {  server.send ( 200, "text/html", PAGE_Information );   }  );
	server.on ( "/ntp.html", 				send_NTP_configuration_html  );
	server.on ( "/dmx", 					send_DMX_configuration_html  );
	server.on ( "/dmx.html", 				send_DMX_configuration_html  );
	server.on ( "/dmxtx.html", 				send_DMX_TX_html  );
	server.on ( "/general.html", 			send_general_html  );
	server.on ( "/style.css", 		[]() { 	server.send ( 200, "text/plain", PAGE_Style_css );  } );
	server.on ( "/microajax.js", 	[]() { 	server.send ( 200, "text/plain", PAGE_microajax_js );  } );
	server.on ( "/admin/values", 			send_network_configuration_values_html );
	server.on ( "/admin/connectionstate", 	send_connection_state_values_html );
	server.on ( "/admin/infovalues", 		send_information_values_html );
	server.on ( "/admin/ntpvalues", 		send_NTP_configuration_values_html );
	server.on ( "/admin/dmxvalues", 		send_DMX_configuration_values_html );
	server.on ( "/admin/dmxtxvalues", 		send_DMX_TX_values_html );
	server.on ( "/admin/generalvalues", 	send_general_configuration_values_html);
	server.on ( "/admin/devicename",     	send_devicename_value_html);

	server.onNotFound ( []() { Serial.println("Uuups .... Page Not Found"); server.send ( 400, "text/html", "Uuups .... Page not Found" );   }  );
	server.begin();
	Serial.println( "> HTTP server started" );

} // server_init()

// ------------------------------------------------------------------------------------- flash_get_config
// write default parameters to flash
void flash_set_default_config(){

	// DEFAULT CONFIG
	config.ssid 			= WIFI_SSID;
	config.password 		= WIFI_PASS;
	config.dhcp 			= true;
	config.IP[0] = 192;		config.IP[1] = 168;		config.IP[2] = 179;		config.IP[3] = 100;
	config.Netmask[0] = 255;config.Netmask[1] = 255;config.Netmask[2] = 255;config.Netmask[3] = 0;
	config.Gateway[0] = 192;config.Gateway[1] = 168;config.Gateway[2] = 1;	config.Gateway[3] = 1;
	config.ntpServerName 	= "0.de.pool.ntp.org";
	config.Update_Time_Via_NTP_Every =  0;
	config.timezone 		= 10;
	config.daylight 		= true;
	config.DeviceName 		= DEVICE_NAME;
	config.AutoTurnOff 		= false;
	config.AutoTurnOn 		= false;
	config.TurnOffHour 		= 0;
	config.TurnOffMinute 	= 0;
	config.TurnOnHour 		= 0;
	config.TurnOnMinute 	= 0;
	config.deviceNum 		= 0;
	config.moduleNum 		= 0;
	config.DMXoutputOn 		= false;
	config.universe 		= 2;
	config.DxmPowerOffTime 	= 2;
	config.LEDconfig 		= 0;
	config.batAnalogRef 	= ANALOG_REF_mV;
	config.batDividerRatio 	= DIVIDER_RATIO;
	config.P9813outputOn 	= false;
	config.OLEDoutputOn 	= false;
	config.Firmware 		= FIRMWARE;

	WriteConfig();

} //flash_get_config()

// ------------------------------------------------------------------------------------- WiFiConnect
// start Access Point
void WifiConnectAP() {

	DEBUGGING("--------------------------------------------------- WifiConnectAP ");
	DEBUGGING_L(">> SSID: ");
	DEBUGGING(ACCESS_POINT_NAME);

	// AP mode
	// WiFi.mode(WIFI_AP); // Access point (AP) mode, where it creates its own network that others can join
  	WiFi.mode(WIFI_AP_STA);
  	WiFi.softAP(ACCESS_POINT_NAME , ACCESS_POINT_PASSWORD);
  	IPAddress myIP = WiFi.softAPIP();

  	DEBUGGING_L(">> WiFi - connected to Access Point with IP ");
  	DEBUGGING(myIP);
  	Led_Blink(pLED_RED,100, 300);
  	Led_Blink(pLED_RED,100, 300);
  	SoftAPup = true;


}

// ------------------------------------------------------------------------------------- WifiStopAP
// stop access point
void WifiStopAP() {

  	DEBUGGING("WiFi AP> disconnect Access Point");
	WiFi.softAPdisconnect();
	WiFi.mode(WIFI_STA);
	delay(100);
	SoftAPup = false;
}

// ------------------------------------------------------------------------------------- module_init
// usually part of setup but required during operation if WiFI connection got lost
void module_init(){

	int connect_count = 0;

	  esp_level		= 10;
	  Led_Off();

	  OLED_clear();
	  OLED_print(0,0,"WiFi connect  ");
      OLED_print(100,0,fwVers);
	  OLED_logo(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits, false);

	  if(bStartup==true) // blink only during initial startup to avoid flashing during show
	  {
		  Led_Blink(LED_STATUS	,25,150);
		  delay(150);
		  Led_Blink(LED_STATUS	,25,150);
		  WiFi.setAutoConnect(false); // do not connect on power on to the last used access point
	  }
	  else
		  WiFi.setAutoConnect(true); // connect to the last used access point

	  if(connect_count > 10) //
	  {
		  bStartup==true;
		  connect_count = 0;
	  }

	  // connect to WiFi and set multicast address based on universe
	  universe = config.universe;

	  DEBUGGING_L("\nmodule_init> connect using ip ");
	  DEBUGGING  (esp_ip);

	  OLED_print(0,50,config.ssid.c_str());
	  retval = e131.beginMulticast(config.ssid.c_str(), config.password.c_str(), universe);
	  if (retval == WL_CONNECTED)
	  {
		  // WifiStopAP(); 		// AP is only needed if normal WiFi is missing
		  // WiFi.printDiag(Serial);

		  const char* modes[] = { "NULL", "STA", "AP", "STA+AP" };
		  Serial.print("WiFi> Mode    : ");
		  Serial.println(modes[WiFi.getMode()]);

		  const char* phymodes[] = { "", "B", "G", "N" };
		  Serial.print("WiFi> PHY mode: ");
		  Serial.println(phymodes[(int) WiFi.getPhyMode()]);

		  Serial.print("WiFi> Channel : ");
		  Serial.println(WiFi.channel());

		  DMX_Init(); 			// calculate channel; set buffer
		  server_init(); 		// attach html pages
		  HTTPUpdateConnect(); 	// OTA update server

		  // MDNSConnect(); // multicast DNS
		  // NTPRefreshAP(); // get ntp time from AP
		  esp_level		= 50;
		  NodeServer_init_Multicast(); // udp begin, connect to master panel
		  NodeServer_text_Multicast("ALIVE");
		  esp_status = "ALIVE";

		  if(bStartup==true)
		  {
			  Led_Blink(LED_STATUS	,25,300);

			  // get ip of ESP8266 module
			  esp_ip 	= WiFi.localIP();
	          gateway 	= WiFi.gatewayIP();
	          dns 		= gateway;
	          WiFi.config(esp_ip, gateway, subnet, dns );
	          esp_ip_str = WiFi.localIP().toString();
		  }
		  bStartup = false;
	  }
	  else // WLAN AP not found
	  {
		  if(bStartup==true) // injecting pause during startup mode to save power and avoid modules gets to hot
		  {
			  Led_Blink(LED_STATUS,	25,150);
			  OLED_clear();
			  OLED_print(0,0,"WiFi connect FAILED");

			  // wifi_set_sleep_type(LIGHT_SLEEP_T); // switch modem off
			  WiFi.mode(WIFI_OFF);
			  DEBUGGING_L(" zzZZZ");
			  WiFi.forceSleepBegin();
			  delay(4000);
			  // wifi_set_sleep_type(NONE_SLEEP_T); // switch back to normal state, no sleep
			  WiFi.forceSleepWake();
			  DEBUGGING_L(" AWAKE  ");
		  }
	  }
	  connect_count++;
	  if(connect_count>5) // enable DHCP again
		  WiFi.config(ip0, gateway0, subnet, dns0 );
}

// ------------------------------------------------------------------------------------- setup
// starting the device
void setup()
{
  Led_Init();		// set output ports for LED
  // udp_send_init();
  EEPROM.begin(512);

  esp_status 	= "BOOT";
  esp_level		= 0;

  Serial.begin(115200); // start serial
  delay(100);

  DEBUGGING("\n\n");
  DEBUGGING  ("--------------------------------------------------- ESP8266 E131 client ");
  DEBUGGING_L("         Firmware ");
  DEBUGGING(FIRMWARE);

  config.OLEDoutputOn = true; // DEBUG ONLY !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  OLED_init();
  OLED_print(0,0,"boot ...");
  OLED_logo(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits, false);
  // OLED_logo(0, 0, OLED_IMG_Logo_width, OLED_IMG_Logo_height, OLED_IMG_Logo, true);

	if (!ReadConfig()) // read parameters from flash
	{
		flash_set_default_config();
		Serial.println("> General config applied");
	}
	// config.OLEDoutputOn = true; // DEBUG ONLY !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	OLED_init();
	config.Firmware = FIRMWARE;
	fwVers = config.Firmware.substring(0, 5);
	OLED_print(100,0,fwVers);
    Led_Init();		// set output ports for LED
	P9813_init(); 	// initialize serial MOSFET module P9813
	Battery_Init();	// initialize battery monitor on A0

	if (AdminEnabled)
		WifiConnectAP();
	else
		WiFi.mode(WIFI_STA);

	WiFi.hostname(String(config.DeviceName));

	Led_TestStartup(); // blink RGB stripe

	// set to 0 to enable DHCP for startup
	esp_ip		= ip0;
    gateway 	= gateway0;
    dns 		= gateway0;

	module_init();

	bStartup==false;

	// set various timers
	tempTimer = flashTimer = powerOffTimer = aliveTimer = aliveE131Timer = noWiFiTimer = millis(); // start timing for auto switch off
	/*
	wifi_set_sleep_type(LIGHT_SLEEP_T); // switch modem off
	wifi_set_sleep_type(NONE_SLEEP_T); // switch back to normal state, no sleep
	*/
}

// ------------------------------------------------------------------------------------- // ------------------------------------------------------------------------------------- loop

void WifiSignal(){

	/*
	High quality	90% ~= -55db
	Medium quality	50% ~= -75db
	Low quality	 30% ~= -85db
	Unusable quality	   8% ~= -96db
	*/

	rssi_value = WiFi.RSSI();
	rssi_percent = 2 * (rssi_value + 100);

	if(rssi_value > -55) {
		rssi_percent = 100;
		rssi_qual 	 = "high";
		rssi_bars 	 = 4;
	}
	else if(rssi_value >= -75)
	{
		rssi_qual = "medium";
		rssi_bars = 3;
	}
	else if(rssi_value >= -85)
	{
		rssi_qual = "low";
		rssi_bars = 2;
	}
	else
	{
		rssi_percent = 0;
		rssi_qual = "very poor";
		rssi_bars = 1;
	}

    /*
	Serial.print  ("RSSI: ");
	Serial.print  (rssi);
	Serial.print  (" ");
	Serial.print  (rssi_qual);
	Serial.print  (" ");
	Serial.println(rssi_bars);
    */

}


// ------------------------------------------------------------------------------------- loop
void loop()
{
	tempTimer = millis();

	  if (WiFi.status() != WL_CONNECTED) // check if WiFi is conencted
	  {
		DEBUGGING("========== WIFI Reconnect ============= ");
	    module_init(); 		// connect to Wifi + start http server
	  } else
	  {
		  if (battery_critical == true){
	    	  esp_level		= 0;
	    	  esp_status = "BATTERY CRITICAL - FORCE SLEEP";
	    	  MessageString  = esp_status;
	    	  NodeServer_text_Multicast(MessageString);			  // wifi_set_sleep_type(LIGHT_SLEEP_T); // switch modem off
	    	  yield();
			  WiFi.mode(WIFI_OFF);
			  DEBUGGING_L(" zzZZZ");
			  WiFi.forceSleepBegin();
			  delay(4000);
			  // wifi_set_sleep_type(NONE_SLEEP_T); // switch back to normal state, no sleep
			  WiFi.forceSleepWake();
			  DEBUGGING_L(" AWAKE  ");
		  }
		  else	// battery not critical
		  {
			  // handle_udp();
			  handle_e131();			// read udp ACN/E1.31 packages
			  WiFiConnectCnt = 0; 	// reset Connect counter

			  if (udpActive == false) // give udp priority and delay http requests until no udp packets are received
			  {
				  esp_level		= 50;
				  // if button is pressed, send multicast broadcast
				  iRetVal = digitalRead (Button1);
				  if(iRetVal == LOW ) {
					  esp_status = "LED TEST";
					  esp_status = "send E1.31 test 0";
					  MessageString  = esp_status;
					  NodeServer_text_Multicast(MessageString);
					  // Led_Test();
					  e131_send_10s(0,0,0);
					  esp_status = "send E1.31 test done";
					  MessageString  = esp_status;
					  NodeServer_text_Multicast(MessageString);
				  }
				  /*
    			iRetVal = digitalRead (Button2);
    			if(iRetVal == HIGH ) {
    				esp_status = "send E1.31 test";
    				MessageString  = esp_status;
    				NodeServer_text_Multicast(MessageString);
    				udp_send_test();
    			}
    			iRetVal = digitalRead (Button3);
    			if(iRetVal == HIGH ) {
    				esp_status = "send E1.31 test 0";
    				MessageString  = esp_status;
    				NodeServer_text_Multicast(MessageString);
    				e131_send_10s(0,0,0);
    				esp_status = "send E1.31 test done";
    				MessageString  = esp_status;
    				NodeServer_text_Multicast(MessageString);
    			}
				   */
				  if (millis() - lastTimeHost > 10)
				  {
					  server.handleClient();
					  lastTimeHost = millis();
					  yield();
				  }
				  // send status every second if udp is not active
				  if (millis() - lastTimeMaster > 1000)
				  {
					  DMX_LED_off();
					  battery_getval();
					  WifiSignal();
					  Led_Off();
					  yield();

					  if (uiVoltLevel < 50) // battery too low
					  {
						  MessageString  	= " zzzZZ BATTERY TOO LOW";
						  esp_status 		= "BattLow";
						  esp_level		= 20;
						  Led_Set(50,0,0); delay(50); Led_Off();
						  delay(50);
						  Led_Set(50,0,0); delay(50); Led_Off();
						  delay(500);
					  }
					  else
					  {
						  MessageString  	= " no udp - zzzZZZZ";
						  esp_status 		= "zzZZ";
					  }

				  if (testmode == true)
            		MessageString += " - testloop";
				  NodeServer_text_Multicast(MessageString);
				  lastTimeMaster = millis();
				  yield();
				  }
				  if (testmode == true)
					  Led_TestLoop();
				  if (testDmxRGB == true)
					  e131_test_rgb();

			  } // if (udpActive == false)

		  } // battery critical

	  } // if (WiFi.status() != WL_CONNECTED)
}

