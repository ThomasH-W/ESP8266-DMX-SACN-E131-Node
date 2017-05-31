#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2017-05-13 07:46:45

#include "Arduino.h"
#define DEVICE_NAME "ucl-01-spot"
#define WIFI_CONNECT_TIMEOUT 20000
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
#include "E131Constants.h"
#include <SPI.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include "global.h"
#include "global.h"
#include "OLED_images.h"
#include "global.h"
void dmx_serial_init() ;
void HTTPUpdateConnect() ;
void MDNSConnect() ;
void Battery_Init() ;
void Led_Init() ;
void Led_TestLoop() ;
void Led_Blink(int iLED, int brightness, int duration) ;
void Led_Off() ;
void Led_Set(int iRed,  int iGreen, int iBlue) ;
void Led_Test() ;
void Led_TestStartup() ;
void server_reply(String message);
void firwmareUpdateFixed() ;
void firwmareUpdatePHP() ;
void e131_send_10s(int iRed, int iGreen, int iBlue) ;
void esp_reset() ;
void json_on() ;
void json_off() ;
void server_init() ;
void flash_set_default_config();
void WifiConnectAP() ;
void WifiStopAP() ;
void module_init();
void setup() ;
void WifiSignal();
void loop() ;
void battery_getval();
void e131_send_rgb_all(const uint8_t dRed,const uint8_t dGreen,const uint8_t dBlue);
void e131_send_rgb_set(const uint8_t DmxSet, const uint8_t dRed,const uint8_t dGreen,const uint8_t dBlue);
void e131_send_rgb_set_ext(const uint8_t DmxSet, const uint8_t dRed,const uint8_t dGreen,const uint8_t dBlue);
void e131_test_rgb();
void reconnect_e131();
void handle_e131() ;
void DMX_LED_off() ;
void DMX_LED_set(int iRed,  int iGreen, int iBlue) ;
void DMX_Init() ;
void NodeServer_init_Multicast() ;
void NodeServer_text_Multicast_json(String sMessage) ;
void NodeServer_text_Multicast_json_crash(String sMessage) ;
void NodeServer_text_Multicast_flat(String sMessage) ;
void OLED_status_DMX();
void NodeServer_text_Multicast(String sMessage) ;
void NodeServer_ping_Multicast() ;
void udp_send_init() ;
void udp_send_test() ;
void ConfigureWifi() ;
void EraseConfig() ;
void WriteConfig() ;
boolean ReadConfig() ;
void NTPRefreshAP()   ;
void NTPRefresh() ;
void Second_Tick() ;
void OLED_print(int16_t x, int16_t y, String message) ;
void OLED_Wlan_bar(int16_t x, int16_t y,int rssi_percent) ;
void OLED_logo(int16_t xMove, int16_t yMove, int16_t width, int16_t height, const char *xbm, boolean img_clear) ;
void OLED_clear() ;
void OLED_init(void) ;
void P9813_show(int xx, uint8_t LedRed, uint8_t LedGreen, uint8_t LedBlue) ;
void P9813_init(void) ;
void P9813_test(void) ;

#include "ESP12E_E131_DMX.ino"

#include "ESP12E_E131_udp_FIE.ino"
#include "ESP12E_WiFi_EEPROM.ino"
#include "OLED.ino"
#include "P9813.ino"

#endif
