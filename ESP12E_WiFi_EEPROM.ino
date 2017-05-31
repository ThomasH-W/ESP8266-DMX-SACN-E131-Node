/*
 * WiFi_EEPROM.ino
 *
 *  Created on: 11.01.2017
 *      Author: Thomas
 */


#include "global.h"



// ------------------------------------------------------------------------------------- ConfigureWifi
void ConfigureWifi()
{
	int iRetVal;
	Serial.println("> Configuring Wifi");

	  if (Serial) {
	      Serial.print(F("WiFi> Connecting to "));
	      Serial.print(config.ssid.c_str());
	      Serial.print(" ");
	  }

	  if (config.password.c_str() != NULL)
	      WiFi.begin(config.ssid.c_str(), config.password.c_str());
	  else
	      WiFi.begin(config.ssid.c_str());

	  uint32_t timeout = millis();
	  uint8_t retval = 1;
	  while (( iRetVal =  WiFi.status() ) != WL_CONNECTED) {
	      delay(500);
	      if (Serial)
	          Serial.print(".");
	          delay(10);
	      if (millis() - timeout > WIFI_CONNECT_TIMEOUT) {
	          retval = 0;
	          if (Serial) {
	              Serial.println("");
	              Serial.println(F("*** Failed to connect ***"));
	          }
	          break;
	      }
	  }

	  if(iRetVal == WL_CONNECTED ){
		  Serial.print("\nWiFi> Connected with IP ");
		  Serial.println(WiFi.localIP());
		  long rssi = WiFi.RSSI();
		  Serial.print("WiFi> signal strength (RSSI):");
		  Serial.print(rssi);
		  Serial.println(" dBm");
	  } else {
		  Serial.print(">> WiFi ERROR ");
		  Serial.println(iRetVal);
	  }

	// WiFi.begin (config.ssid.c_str(), config.password.c_str());
	if (!config.dhcp)
	{
		WiFi.config(IPAddress(config.IP[0],config.IP[1],config.IP[2],config.IP[3] ),  IPAddress(config.Gateway[0],config.Gateway[1],config.Gateway[2],config.Gateway[3] ) , IPAddress(config.Netmask[0],config.Netmask[1],config.Netmask[2],config.Netmask[3] ));
	}
}

// ------------------------------------------------------------------------------------- EraseConfig
void EraseConfig()
{
	Serial.println("Reset Config");
	EEPROM.write(0,'0');
	EEPROM.write(1,'0');
	EEPROM.write(2,'0');

	EEPROM.commit();
}

// ------------------------------------------------------------------------------------- WriteConfig
void WriteConfig()
{
	Serial.println("Writing Config");

	fwVers = config.Firmware.substring(0, 5);

	EEPROM.write(0,'C');
	EEPROM.write(1,'F');
	EEPROM.write(2,'G');

	EEPROM.write(16,config.dhcp);
	EEPROM.write(17,config.daylight);

	EEPROMWritelong(18,config.Update_Time_Via_NTP_Every); // 4 Byte

	EEPROMWritelong(22,config.timezone);  // 4 Byte


	EEPROM.write(26,config.LED_R);
	EEPROM.write(27,config.LED_G);
	EEPROM.write(28,config.LED_B);

	EEPROM.write(32,config.IP[0]);
	EEPROM.write(33,config.IP[1]);
	EEPROM.write(34,config.IP[2]);
	EEPROM.write(35,config.IP[3]);

	EEPROM.write(36,config.Netmask[0]);
	EEPROM.write(37,config.Netmask[1]);
	EEPROM.write(38,config.Netmask[2]);
	EEPROM.write(39,config.Netmask[3]);

	EEPROM.write(40,config.Gateway[0]);
	EEPROM.write(41,config.Gateway[1]);
	EEPROM.write(42,config.Gateway[2]);
	EEPROM.write(43,config.Gateway[3]);


	WriteStringToEEPROM(64,config.ssid);			// 32 bytes
	WriteStringToEEPROM(96,config.password);		// 32 bytes
	WriteStringToEEPROM(128,config.ntpServerName);	// ?? bytes
	WriteStringToEEPROM(160,config.Firmware);	// ?? bytes
	WriteStringToEEPROM(192,config.DeviceName);

	EEPROM.write(300,config.AutoTurnOn);
	EEPROM.write(301,config.AutoTurnOff);
	EEPROM.write(302,config.TurnOnHour);
	EEPROM.write(303,config.TurnOnMinute);
	EEPROM.write(304,config.TurnOffHour);
	EEPROM.write(305,config.TurnOffMinute);

	EEPROM.write(306,config.deviceNum		);	// 4 Byte
	EEPROM.write(310,config.moduleNum		); 	// 4 Byte
	EEPROM.write(314,config.universe		); 	// 4 Byte
	EEPROM.write(318,config.DxmPowerOffTime	); 	// 4 Byte
	EEPROM.write(322,config.LEDconfig		); 	// 1 Byte
	EEPROM.write(323,config.batAnalogRef	); 	// 4 Byte
	EEPROM.write(327,config.batDividerRatio	); 	// 4 Byte = 32bit = float
	EEPROM.write(331,config.DMXoutputOn		); 	// 1 Byte
	EEPROM.write(332,config.P9813outputOn	); 	// 1 Byte
	EEPROM.write(333,config.OLEDoutputOn	); 	// 1 Byte

	/*
	Serial.print  ("EEPROM.write> deviceNum ");
	Serial.println(config.deviceNum);
	Serial.print  ("EEPROM.write> moduleNum ");
	Serial.println(config.moduleNum);
	 */


	EEPROM.commit();
}


// ------------------------------------------------------------------------------------- ReadConfig
boolean ReadConfig()
{

	Serial.println("> Reading Configuration");
	if (EEPROM.read(0) == 'C' && EEPROM.read(1) == 'F'  && EEPROM.read(2) == 'G' )
	{
		Serial.println(">> Configuration Found!");
		config.dhcp = 	EEPROM.read(16);

		config.daylight = EEPROM.read(17);

		config.Update_Time_Via_NTP_Every = EEPROMReadlong(18); // 4 Byte

		config.timezone = EEPROMReadlong(22); // 4 Byte

		config.LED_R = EEPROM.read(26);
		config.LED_G = EEPROM.read(27);
		config.LED_B = EEPROM.read(28);

		config.IP[0] = EEPROM.read(32);
		config.IP[1] = EEPROM.read(33);
		config.IP[2] = EEPROM.read(34);
		config.IP[3] = EEPROM.read(35);
		config.Netmask[0] = EEPROM.read(36);
		config.Netmask[1] = EEPROM.read(37);
		config.Netmask[2] = EEPROM.read(38);
		config.Netmask[3] = EEPROM.read(39);
		config.Gateway[0] = EEPROM.read(40);
		config.Gateway[1] = EEPROM.read(41);
		config.Gateway[2] = EEPROM.read(42);
		config.Gateway[3] = EEPROM.read(43);
		config.ssid = ReadStringFromEEPROM(64);
		config.password = ReadStringFromEEPROM(96);
		config.ntpServerName = ReadStringFromEEPROM(128);


		config.AutoTurnOn 		= EEPROM.read(300);
		config.AutoTurnOff 		= EEPROM.read(301);
		config.TurnOnHour 		= EEPROM.read(302);
		config.TurnOnMinute 	= EEPROM.read(303);
		config.TurnOffHour 		= EEPROM.read(304);
		config.TurnOffMinute 	= EEPROM.read(305);

		config.deviceNum 		= EEPROM.read(306);		// 4 Byte
		config.moduleNum 		= EEPROM.read(310);		// 4 Byte
		config.universe  		= EEPROM.read(314);		// 4 Byte
		config.DxmPowerOffTime  = EEPROM.read(318);		// 4 Byte
		config.LEDconfig  		= EEPROM.read(322);		// 1 Byte
		config.batAnalogRef  	= EEPROM.read(323);		// 4 Byte
		config.batDividerRatio  = EEPROM.read(327);		// 4 Byte
		config.DMXoutputOn	  	= EEPROM.read(331);		// 1 Byte
		config.P9813outputOn	= EEPROM.read(332);		// 1 Byte
		config.OLEDoutputOn		= EEPROM.read(333);		// 1 Byte

		/*
		Serial.print  ("EEPROM.read> deviceNum ");
		Serial.println(config.deviceNum);
		Serial.print  ("EEPROM.read> moduleNum ");
		Serial.println(config.moduleNum);
		*/

		config.Firmware   = ReadStringFromEEPROM(160);
		config.DeviceName = ReadStringFromEEPROM(192);

		return true;

	}
	else
	{
		Serial.println(">> ERROR: Configurarion NOT FOUND!!!!");
		return false;
	}
}

// ------------------------------------------------------------------------------------- NTPRefresh
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[ NTP_PACKET_SIZE];

void NTPRefreshAP()  // use NTP server on Access Point
{

	if (WiFi.status() == WL_CONNECTED)
	{
		IPAddress timeServerIP = WiFi.gatewayIP();
		//sendNTPpacket(timeServerIP); // send an NTP packet to a time server

		Serial.print("ntp> UNIX timestamp ");
		Serial.println(UnixTimestamp);

		Serial.print("ntp> sending NTP packet to ");
		Serial.println(timeServerIP);
		memset(packetBuffer, 0, NTP_PACKET_SIZE);
		packetBuffer[0] = 0b11100011;   // LI, Version, Mode
		packetBuffer[1] = 0;     // Stratum, or type of clock
		packetBuffer[2] = 6;     // Polling Interval
		packetBuffer[3] = 0xEC;  // Peer Clock Precision
		packetBuffer[12]  = 49;
		packetBuffer[13]  = 0x4E;
		packetBuffer[14]  = 49;
		packetBuffer[15]  = 52;
		UDPNTPClient.beginPacket(timeServerIP, 123);
		UDPNTPClient.write(packetBuffer, NTP_PACKET_SIZE);
		UDPNTPClient.endPacket();


		delay(1000);

		int cb = UDPNTPClient.parsePacket();
		if (!cb) {
			Serial.println("ntp> NTP no packet received");
		}
		else
		{
			Serial.print("ntp> NTP packet received, length=");
			Serial.println(cb);
			UDPNTPClient.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
			unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
			unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
			unsigned long secsSince1900 = highWord << 16 | lowWord;
			const unsigned long seventyYears = 2208988800UL;
			unsigned long epoch = secsSince1900 - seventyYears;
			UnixTimestamp = epoch;
			Serial.print("ntp> UNIX timestamp ");
			Serial.println(UnixTimestamp);
		}
	}
}


void NTPRefresh()
{

	if (WiFi.status() == WL_CONNECTED)
	{
		IPAddress timeServerIP;
		WiFi.hostByName(config.ntpServerName.c_str(), timeServerIP);
		//sendNTPpacket(timeServerIP); // send an NTP packet to a time server


		Serial.println("> sending NTP packet...");
		memset(packetBuffer, 0, NTP_PACKET_SIZE);
		packetBuffer[0] = 0b11100011;   // LI, Version, Mode
		packetBuffer[1] = 0;     // Stratum, or type of clock
		packetBuffer[2] = 6;     // Polling Interval
		packetBuffer[3] = 0xEC;  // Peer Clock Precision
		packetBuffer[12]  = 49;
		packetBuffer[13]  = 0x4E;
		packetBuffer[14]  = 49;
		packetBuffer[15]  = 52;
		UDPNTPClient.beginPacket(timeServerIP, 123);
		UDPNTPClient.write(packetBuffer, NTP_PACKET_SIZE);
		UDPNTPClient.endPacket();


		delay(1000);

		int cb = UDPNTPClient.parsePacket();
		if (!cb) {
			Serial.println(">> NTP no packet yet");
		}
		else
		{
			Serial.print(">> NTP packet received, length=");
			Serial.println(cb);
			UDPNTPClient.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
			unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
			unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
			unsigned long secsSince1900 = highWord << 16 | lowWord;
			const unsigned long seventyYears = 2208988800UL;
			unsigned long epoch = secsSince1900 - seventyYears;
			UnixTimestamp = epoch;
		}
	}
}

// ------------------------------------------------------------------------------------- Second_Tick
void Second_Tick()
{
	strDateTime tempDateTime;
	AdminTimeOutCounter++;
	cNTP_Update++;
	UnixTimestamp++;
	ConvertUnixTimeStamp(UnixTimestamp +  (config.timezone *  360) , &tempDateTime);
	if (config.daylight) // Sommerzeit beachten
		if (summertime(tempDateTime.year,tempDateTime.month,tempDateTime.day,tempDateTime.hour,0))
		{
			ConvertUnixTimeStamp(UnixTimestamp +  (config.timezone *  360) + 3600, &DateTime);
		}
		else
		{
			DateTime = tempDateTime;
		}
	else
	{
			DateTime = tempDateTime;
	}
	Refresh = true;
}
