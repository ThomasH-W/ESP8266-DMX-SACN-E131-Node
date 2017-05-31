/*
 * E131_udp.c
 *
 *  Created on: 08.01.2017
 *      Author: Thomas
 */

#define _UDP WiFiUDP	/* define meta var in case code needs to handle Ethernet as well */
_UDP Udp, UdpMaster;               /* UDP handle */


// ------------------------------------------------------------------------------------- battery_getval
// LiPo batteries are sensitive if voltage is dropping a certain value
// in addition the LED stripe will need a certain voltage - espacially when showing blue
// read voltage from analog in and convert to Volt and %
void battery_getval(){

	int iVoltLevel = 0;

	if (millis() - batteryTimer > 100)
	{
		uiVoltAnalog 		= analogRead(ANALOG_PIN);
		fVoltLipo		= uiVoltAnalog * config.batDividerRatio * config.batAnalogRef / 1000 / 1024 ;
		// Lipo: 1S full 4,2; empty 3,3V
		// Lipo: 2S full 8,34; empty 6,6V
		/*
		iVoltLevel		= (int) (( (fVoltLipo * 1000) - LIPO_mV_EMPTY) / (LIPO_mV_FULL - LIPO_mV_EMPTY) *100 );
		if( iVoltLevel < 0) // check if level is below limit
			uiVoltLevel = 0;
		else
			uiVoltLevel = iVoltLevel;
		*/

		// LED Stripe level:  RED		0-7,5; 		// YELLOW	7,5 - 7,9V 		// GREEN	7,9V
		battery_critical = false; // used to disable wiFi
		if( fVoltLipo < 3.0 ) // no measurement attached to A0
			uiVoltLevel = 99;
		else if (fVoltLipo < 7.0) {
			uiVoltLevel = 0;
			battery_critical = true;
		}
		else if (fVoltLipo < 7.5)
			uiVoltLevel = 0;
		else if (fVoltLipo < 7.9)
			uiVoltLevel = 50;
		else
			uiVoltLevel = 100;

		/*
		uiVoltLevel =0;		// RED - BLUE LED too low
		if( fVoltLipo > 7.5) // YELLOW - blue LED still ok
			uiVoltLevel = 50;
		if( fVoltLipo > 7.9) // GREEN - all LED bright
			uiVoltLevel = 100;
		*/

		// Spot 1+2 are using a power from DMX device, no battery connected
		if (config.deviceNum >1 && config.deviceNum <4)
			uiVoltLevel = 100;

		batteryTimer 	= millis();
	}

}

// ------------------------------------------------------------------------------------- e131_set_rgb_all
// every module can send e131 in multicast mode
// this is for test purposes only, when the program for the light show is not available
// one device has 12 channels (DMX_SET)
// on actor=device has three modules with one LED stripe each
void e131_send_rgb_all(const uint8_t dRed,const uint8_t dGreen,const uint8_t dBlue){


	e131.clearSendBuffer();
	e131.setSourceName("ACN Tester - Thomas Hoeser");

	for(int x=0; x<4; x++) // send package 3 times, less than 150ms
	{

		for (int i=(0*DMX_SET); i<(4*DMX_SET); i +=12) // first 4 devices to not have add on modules
		{
		e131.setRGB(i,dRed,dGreen,dBlue);
		}
		for (int i=(5*DMX_SET); i<(12*DMX_SET); i +=3) // first 4 devices to not have add on modules
		{
			e131.setRGB(i,dRed,dGreen,dBlue);
		}
		if(DMXsequence>0xFF)
		  DMXsequence = 1;
		e131.setSequenceNumber(DMXsequence++);
		e131.sendPacket(universeTX);
		delay(40); // wait for 40ms to send next package
	}

}

// ------------------------------------------------------------------------------------- e131_send_rgb_set
// send R-G-B
void e131_send_rgb_set(const uint8_t DmxSet, const uint8_t dRed,const uint8_t dGreen,const uint8_t dBlue){

	if(DMXsequence>0xFF)
		  DMXsequence = 1;
	e131.setSequenceNumber(DMXsequence++);
	e131.setSourceName("ACN Tester - Thomas Hoeser");

	e131.setRGB(DmxSet*DMX_SET,dRed,dGreen,dBlue);
	e131.sendPacket(universeTX);
	delay (100);
}

// ------------------------------------------------------------------------------------- e131_send_rgb_set_ext
void e131_send_rgb_set_ext(const uint8_t DmxSet, const uint8_t dRed,const uint8_t dGreen,const uint8_t dBlue){
	uint8_t dmxStartCh = DmxSet * DMX_SET;

	testmode = false;
	MessageString = " TX: Set:";
	MessageString += (String) DmxSet;
	MessageString += " |Channel:";
	MessageString += (String) (dmxStartCh);
	MessageString += "| ";
	MessageString += (String) (dRed);
	MessageString += "-";
	MessageString += (String) (dGreen);
	MessageString += "-";
	MessageString += (String) (dBlue);
	NodeServer_text_Multicast(MessageString);
	DEBUGGING(MessageString);

	e131.clearSendBuffer();
	e131.setSourceName("ACN Tester - Thomas Hoeser");

	for(int x=0; x<4; x++) // send package 3 times, less than 150ms
	{
	e131.setSequenceNumber(DMXsequence++);
	if(DMXsequence>0xFF)
		  DMXsequence = 1;
	e131.setRGB(dmxStartCh  ,dRed, dGreen, dBlue);
	if(DmxSet>3){
		e131.setRGB(dmxStartCh+3, dRed, dGreen, dBlue);
		e131.setRGB(dmxStartCh+6, dRed, dGreen, dBlue);
	}
	e131.sendPacket(universeTX);
	delay(40); // wait for 40ms to send next package
}

}


// ------------------------------------------------------------------------------------- e131_test_rgb
void e131_test_rgb(){

	e131.setSequenceNumber(DMXsequence++);
	e131_send_rgb_all(255,0,0); 		delay (500);
	e131_send_rgb_all(0,255,0);			delay (500);
	e131_send_rgb_all(0,0,255);			delay (500);
	e131_send_rgb_all(0,0,0)  ;			delay (500);
}

// ------------------------------------------------------------------------------------- reconnect_e131
void reconnect_e131(){
	DEBUGGING_L("\ne131> reconnect ");
	e131.stopUdp();
	e131.connectMulticast(config.universe);
}

// ------------------------------------------------------------------------------------- handle_udp
// read udp package and write DMX value to LED output (analog,P9813.DMX)
void handle_e131() {
    /* Parse a packet and update pixels */

    if(e131.parsePacket())
    {
    	  udpActive = true;
    	  testmode 	= false;
    	  if(SoftAPup == true)
    		  WifiStopAP();
    	  esp_level	= 70;
    	  udpPacketCount ++;
    	  powerOffTimer = tempTimer = lastTimeUdp = millis();
    	  if (config.DMXoutputOn == false)
    		  digitalWrite(LED_STATUS, HIGH);	// set blue status LED

        if (e131.universe == universe && e131.ddStartCode == 0) { // use only e131 packages from the right universe
        	esp_status 	= "E1.31";
        	esp_level	= 100;
        	// read DMX values into temp variable
        	uiDMXRed 	= e131.data[DMX_RED		];
        	uiDMXGreen 	= e131.data[DMX_GREEN	];
        	uiDMXBlue	= e131.data[DMX_BLUE	];
        	uiDMXRed2 	= e131.data[DMX_RED2	];

        	// when device is started, the blue status LED will flash
        	// if 1st DMX channel is used, indicate e131 submission has started
        	// than the flashing of the blue status LED will stop
        	if((e131.data[0]) != 0)
        		e131Active = true;

        	// apply gamma factor on DMX values
        	// every modules can reduce the brightness -> Page_DMX.h
        	if(dmxGammaPerc <100)
        	{
        		uiDMXRed 	= uiDMXRed 		* dmxGammaPerc / 100 ; // 255 * 100 / 100 = 255; 25 * 200 / 100 = 50
        		uiDMXGreen 	= uiDMXGreen 	* dmxGammaPerc / 100 ;
        		uiDMXBlue 	= uiDMXBlue 	* dmxGammaPerc / 100 ;
        		uiDMXRed2 	= uiDMXRed2 	* dmxGammaPerc / 100 ;
        	}

        	if (uiVoltLevel < 50) // battery too low
        	{
        		Led_Off();
        	}
        	else
        	{
        		// set output for analog and P9813
        		analogWrite(pLED_RED,   	uiDMXRed);
        		analogWrite(pLED_GREEN,  	uiDMXGreen);
        		analogWrite(pLED_BLUE, 		uiDMXBlue);
        		analogWrite(pLED_RED2,  	uiDMXRed2);
        		P9813_show(  0, uiDMXRed,  uiDMXGreen, uiDMXBlue);

        		// send data to DMX output if required
        		if (config.DMXoutputOn == true)
        		{
        			// int i=0;
        			for (int thisChannel = DMX_RED, i=0; thisChannel <= (DMX_RED+4); thisChannel++, i++)
        			{
        				dmx.write(thisChannel+1, 	e131.data[thisChannel]);
        				dmx.write(i+1, 				e131.data[thisChannel]);
        			}
        			dmx.update();
        			delay(10);
        		} // DMX output
        	} // battery too low

            yield();					// grant time to background process like wifi handler

        } // if (e131.universe == UNIVERSE)

        // every 2 seconds the modules will send a status to a node server
        if (tempTimer - aliveTimer > 2000)
        {
			battery_getval();
    		WifiSignal();
        	yield();
        	NodeServer_ping_Multicast();
        	aliveTimer = tempTimer;
        	yield();
        }

        // blink blue LED to indicate idle mode with active RF connection
        if (tempTimer - aliveE131Timer > 2000)
        {
        	if(e131Active == false) // do not blink if submission has started
        	{
        		digitalWrite(LED_STATUS, LOW);
        		delay(50);
        		aliveE131Timer = tempTimer;
        	}
        }

    } // if(e131.parsePacket()) {
    // when no udp packages are received for a given time, switch off all LEDs
    // during a show people might not notice a device which is not switched on
    // but it would be very obvious if the LED are on and all other are off
    else {
        if (tempTimer - powerOffTimer > uiDxmPowerOffTime  && uiDxmPowerOffTime > 0 )
        {
        	// DEBUGGING("udp> timeout exceeded -> auto power off");
        	if (udpActive == true) // status True = 1
        	{
        		udpActive  = false;
            	e131Active = false;
            	Led_Off();
    			battery_getval();
        		WifiSignal();
        		yield();					// grant time to background process like wifi handler

        		MessageString = " no udp packet since 2 sec";
        		esp_status = "timeout";
        		NodeServer_text_Multicast(MessageString);
            	tempTimer = powerOffTimer = flashTimer = millis(); // reset timer
        		yield();					// grant time to background process like wifi handler
        	} // if (udpStatus == 1)
        } // if (tempTimer - powerOffTimer > 2000)

		if (millis() - lastTimeUdp > 2000)
		{
        	udpActive  = false;
        	e131Active = false;
        	Led_Off();
			battery_getval();
    		WifiSignal();
    		MessageString 	= " idle - reconnect E1.31";
    		esp_status 		= "CONNECT";
    		// esp_level		= 30;
    		NodeServer_text_Multicast(MessageString);
			reconnect_e131();
    		delay(100);
    		lastTimeUdp 	= millis();
    		yield();
		}
    } // no packet received


}

// ------------------------------------------------------------------------------------- DMXInit
// set paramaters to read correct channels later on
void DMX_LED_off() {

	/*
	if (config.DMXoutputOn == true)
	{
		for (int thisChannel = DMX_RED; thisChannel <= (DMX_RED+4); thisChannel++)
			dmx.write(thisChannel+1, 0);
		dmx.update();
	}
	*/

    if (config.DMXoutputOn == true)

    {
        dmx.write(1, 0	);
        dmx.write(2, 0	);
        dmx.write(3, 0	);
        dmx.update();
    }
}

// ------------------------------------------------------------------------------------- DMXInit
// set paramaters to read correct channels later on
void DMX_LED_set(int iRed,  int iGreen, int iBlue) {

    if (config.DMXoutputOn == true)

    {
        dmx.write(1, iRed	);
        dmx.write(2, iGreen	);
        dmx.write(3, iBlue	);
        dmx.update();
    }
}
// ------------------------------------------------------------------------------------- DMXInit
// set paramaters to read correct channels later on
void DMX_Init() {

	dmxCh = config.deviceNum * DMX_SET + config.moduleNum *3;
	dmxStartChannel = 1 + dmxCh;
	DMX_RED 	= 0 + dmxCh;
	DMX_GREEN 	= 1 + dmxCh;
	DMX_BLUE 	= 2 + dmxCh;
	DMX_RED2 	= 3 + dmxCh;

	uiDxmPowerOffTime = config.DxmPowerOffTime *1000;

	if (config.DMXoutputOn == true)
	{
	DEBUGGING_L("DMX> device        ");
	DEBUGGING(config.deviceNum);
	DEBUGGING_L("DMX> module        ");
	DEBUGGING(config.moduleNum);

	DEBUGGING_L("DMX> channel       ");
	DEBUGGING(dmxCh);
	DEBUGGING_L("DMX> Start channel ");
	DEBUGGING(dmxStartChannel);
	}


} // DMXInit()

// ------------------------------------------------------------------------------------- NodeServer_init_Multicast
// establish udp in order to send messages to a node server
void NodeServer_init_Multicast() {

	// IPAddress ipMulti = IPAddress(192,168,178,255);

	ipMulti = WiFi.localIP();	// get my IP address
	ipMulti[3] = 255; 			// set IP to broadcast address

	retval = UdpMaster.beginMulticast(WiFi.localIP(), ipMulti, portMulti);

    if (Serial) {
    	Serial.printf("udp> Master Multicast IP %s, UDP port %d -> ", WiFi.localIP().toString().c_str(), 45131);
        DEBUGGING(ipMulti);
        /*
        if (retval > 0)
        	DEBUGGING(" ok");
        else
        	DEBUGGING(" ERROR: no socket available");
        */
    }
}


// ------------------------------------------------------------------------------------- NodeServer_ping_Multicast
// module will send x seconds teh RGB values to the node server
void NodeServer_text_Multicast_json(String sMessage) {

	/*
	 * {"name":"Ucl-No7-Wheel","systemInfo":{"FWver":"v3.12","state":"BOOT","bat":{"volt":0.00,"perc":0},"rssi":{"db":0,"perc":0
	 * }},"dmx":{"set":11,"mod":0,"uni":2,"out":"off","color":{"r":0,"g":0,"b":0}},"msg":"ALIVE"}
	 */
	// https://jsonformatter.curiousconcept.com/

	String udpString;
	int iLedRed 	= 0;
	int iLedGreen 	= 0;
	int iLedBlue 	= 0;

	if(udpActive == true) {
		iLedRed 	= e131.data[DMX_RED		];
		iLedGreen 	= e131.data[DMX_GREEN	];
		iLedBlue 	= e131.data[DMX_BLUE	];
	}

	udpString  = "{\"name\":\"";
	udpString += config.DeviceName;
	udpString += "\",\"systemInfo\":{\"FWver\":\"";
	udpString += fwVers;
	udpString += "\",\"state\":\"";
	udpString += esp_status;
	udpString += "\",\"perc\":\"";
	udpString += esp_level;
	udpString += "\",\"ip\":\"";
	udpString += esp_ip_str;
	udpString += "\",\"bat\":{\"volt\":";
	udpString += fVoltLipo;
	udpString += ",\"perc\":";
	udpString += uiVoltLevel;
	udpString += "},\"rssi\":{\"db\":";
	udpString += rssi_value;
	udpString += ",\"perc\":";
	udpString += rssi_percent;
	udpString += "}},\"dmx\":{\"set\":";
	udpString += config.deviceNum;
	udpString += ",\"mod\":";
	udpString += config.moduleNum;
	udpString += ",\"uni\":";
	udpString += config.universe;
	udpString += ",\"serial\":\"";
	if(config.DMXoutputOn ==  true)
		udpString += "on";
	else
		udpString += "off";
	udpString += "\",\"data\":\"";
	if(udpActive == true)
		udpString += "on";
	else
		udpString += "off";

	udpString += "\",\"color\":{\"r\":";

	udpString += iLedRed;
	udpString += ",\"g\":";
	udpString += iLedGreen;
	udpString += ",\"b\":";
	udpString += iLedBlue;
	udpString += "}},\"msg\":\"";
	udpString += sMessage;

	udpString += "\"}";

    UdpMaster.beginPacketMulticast(ipMulti, portMulti,WiFi.localIP());
    UdpMaster.println(udpString);
	UdpMaster.endPacket();

	if (Serial) {
        DEBUGGING_L("udp>>");
        DEBUGGING  (udpString);
    }

} // end of NodeServer_text_Multicast_json

// ------------------------------------------------------------------------------------- NodeServer_ping_Multicast
// module will send x seconds teh RGB values to the node server
void NodeServer_text_Multicast_json_crash(String sMessage) {

	String udpString;
	int iLedRed 	= 0;
	int iLedGreen 	= 0;
	int iLedBlue 	= 0;

	// battery_getval();
	root["name"] = config.DeviceName;


	JsonObject& systemInfo= root.createNestedObject("systemInfo");
	systemInfo["FWver"] = fwVers;
	systemInfo["state"] = esp_status;

	JsonObject& bat = systemInfo.createNestedObject("bat");
	bat["volt"] = fVoltLipo;
	bat["perc"] = uiVoltLevel;

	JsonObject& rssi = systemInfo.createNestedObject("rssi");
	rssi["db"]   = rssi_value;
	rssi["perc"] = rssi_percent;

	JsonObject& dmx = root.createNestedObject("dmx");
	dmx["set"] = config.deviceNum;
	dmx["mod"] = config.moduleNum;
	dmx["uni"] = config.universe;
	if(config.DMXoutputOn ==  true)
		dmx["out"] = "on";
	else
		dmx["out"] = "off";

	if(udpActive == true) {
		iLedRed 	= e131.data[DMX_RED		];
		iLedGreen 	= e131.data[DMX_GREEN	];
		iLedBlue 	= e131.data[DMX_BLUE	];
	}

	JsonObject& color = dmx.createNestedObject("color");
	color ["r"] = iLedRed;
	color ["g"] = iLedGreen;
	color ["b"] = iLedBlue;

	root["msg"] = sMessage;

	// root.printTo(Serial);
	// Serial.println();

	// Serial.println("JSON (pretty)");
	// root.prettyPrintTo(Serial);

	// Serial.println("JSON");
	// root.printTo(udpString);
	// Serial.println(udpString);

	//  udp.beginPacket(remoteIp, remotePort);
	//  json.printTo(udp);
	//  udp.println();
	//  udp.endPacket();

	yield();
    UdpMaster.beginPacketMulticast(ipMulti, portMulti,WiFi.localIP());
    root.printTo(UdpMaster);
    UdpMaster.println();
    //UdpMaster.println(udpString);
	UdpMaster.endPacket();

	yield();
	if (Serial) {
        DEBUGGING_L("udp>>");
        root.printTo(Serial);
        DEBUGGING ("<<udp");
    }

} // end of NodeServer_text_Multicast_json

// ------------------------------------------------------------------------------------- setup
// add header to message for node server
// this header will enable the node server interface to group the modules
void NodeServer_text_Multicast_flat(String sMessage) {

	String udpString = (String)  config.DeviceName;
	udpString += " |set: ";
	udpString +=  (String) config.deviceNum;
	udpString += " |mod: ";
	udpString +=  (String) config.moduleNum;
	udpString += " |uni: ";
	udpString +=  (String) config.universe;

	udpString += " |bat: ";
	udpString += fVoltLipo;
	udpString += "V";
	udpString += " / ";
	udpString += uiVoltLevel;
	udpString += "%";

	udpString += " |rssi: ";
	udpString += rssi_percent;
	udpString += "%";
	udpString += " / ";
	udpString += rssi_value;
	udpString += "db";

	udpString += " |ver: ";
	udpString += fwVers;

	if (config.DMXoutputOn == true)
		udpString += " |DMX:on ";
	else
		udpString += " |DMX:- ";

	udpString += " |msg: ";
	udpString += sMessage;

    UdpMaster.beginPacketMulticast(ipMulti, portMulti,WiFi.localIP());
    UdpMaster.println(udpString);
	UdpMaster.endPacket();

	if (Serial) {
        DEBUGGING_L("udp>>");
        DEBUGGING  (udpString);
    }
} // end of NodeServer_text_Multicast_flat

// ------------------------------------------------------------------------------------- OLED_status_DMX
void OLED_status_DMX(){

	String oledString;

	OLED_clear();

	oledString  = "WiFi: ";
	oledString += rssi_percent;
	oledString += "%";
	oledString += " / ";
	oledString += rssi_value;
	oledString += "db";
	OLED_print(0,0,oledString);

	OLED_Wlan_bar(100,20,rssi_percent);

	oledString  = "";
	oledString += esp_status;
	OLED_print(0,40,oledString);

	if(udpActive == true) {
		iLedRed 	= e131.data[DMX_RED		];
		iLedGreen 	= e131.data[DMX_GREEN	];
		iLedBlue 	= e131.data[DMX_BLUE	];
	}

	OLED_print(0,20,"DMX: ");
	OLED_print(30,20,(String) iLedRed  );
	OLED_print(60,20,(String) iLedGreen);
	OLED_print(90,20,(String) iLedBlue );
}
// ------------------------------------------------------------------------------------- NodeServer_ping_Multicast
void NodeServer_text_Multicast(String sMessage) {
	if(node_json == true)
		NodeServer_text_Multicast_json(sMessage);
	else
		NodeServer_text_Multicast_flat(sMessage);

	OLED_status_DMX();
}

// ------------------------------------------------------------------------------------- NodeServer_ping_Multicast
// module will send x seconds teh RGB values to the node server
void NodeServer_ping_Multicast() {

	iLedRed 	= e131.data[DMX_RED		];
	iLedGreen 	= e131.data[DMX_GREEN	];
	iLedBlue 	= e131.data[DMX_BLUE	];
	iLedRed2 	= e131.data[DMX_RED2	];

	String MessageString = "E131 | R : ";
	MessageString +=  (String) iLedRed;
	MessageString += " | G : ";
	MessageString +=  (String) iLedGreen;
	MessageString += " | B : ";
	MessageString +=  (String) iLedBlue;

	NodeServer_text_Multicast(MessageString);

}

void udp_send_init() {
	pinMode(Button2, INPUT);
	pinMode(Button3, INPUT);
}
void udp_send_test() {
	MessageString = " TX test RGB";
	NodeServer_text_Multicast(MessageString);
	DEBUGGING(MessageString);
	e131_send_rgb_all(255,0,0); 		delay (500);
	e131_send_rgb_all(0,255,0);			delay (500);
	e131_send_rgb_all(0,0,255);			delay (500);
	e131_send_rgb_all(0,0,0)  ;			delay (500);
}
