#pragma once

#include "global.h"

const char PAGE_DMXConfiguration[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<a href="admin.html"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>DMX Settings</strong>
<hr>
<form action="" method="get">
<table border="0"  cellspacing="0" cellpadding="3" >

<tr><td align="right">Device:  </td><td><span id="devicename"></span></td></tr>
<tr><td align="right">Firmware:</td><td><span id="x_firmware"></span></td></tr>

<tr><td colspan="2"><hr></span></td></tr>


<tr><td>Actor/Spot</td><td>
<select  id="deviceNum" name="deviceNum">
	<option value="0">Front</option>
	<option value="1">Back</option>
	<option value="2">Spot 1</option>
	<option value="3">Spot 2</option>
	<option value="4">Laser</option>
	<option value="5">Actor Kim</option>
	<option value="6">Actor Lale</option>
	<option value="7">Actor Swante</option>
	<option value="8">Actor Liz</option>
	<option value="9">Actor No5</option>
	<option value="10">Actor No6</option>
	<option value="11">Actor No7</option>
	<option value="12">Actor No8</option>
</select>
</td></tr>

<tr><td>Module</td><td>
<select  id="moduleNum" name="moduleNum">
	<option value="0">Wheel</option>
	<option value="1">Post</option>
	<option value="2">Body</option>
</select>
</td></tr>

<tr><td>Universe</td><td>
<select  id="universe" name="universe">
	<option value="1">1</option>
	<option value="2">2</option>
	<option value="3">3</option>
	<option value="4">4</option>
	<option value="4711">4711</option>
	<option value="6102">6102</option>
	<option value="54321">54321</option>
</select>
</td></tr>

<tr><td align="right">DMX Channel  :</td><td><span id="x_dmxCh"></span></td></tr>
<tr><td align="right">DMX Start    :</td><td><span id="x_dmxStart"></span></td></tr>
<tr><td align="right">Gamma %</td><td>
<select  id="dmxGamma" name="dmxGamma">
	<option value="200">200</option>
	<option value="150">150</option>
	<option value="120">120</option>
	<option value="100">100</option>
	<option value="90">90</option>
	<option value="80">90</option>
	<option value="70">70</option>
	<option value="60">60</option>
	<option value="50">50</option>
	<option value="40">40</option>
	<option value="30">30</option>
	<option value="20">20</option>
	<option value="10">10</option>
</select>
</td></tr>

<tr>
	<td align="right"> DMX Output:</td>
	<td><input type="checkbox" id="dmxEnabled" name="dmxEnabled"></td>
</tr>


<tr><td>Auto Off [sec]</td><td>
<select  id="x_DxmPwrOffTime" name="x_DxmPwrOffTime">
	<option value="0">0</option>
	<option value="2">2</option>
	<option value="10">10</option>
	<option value="300">300</option>
</select>
</td></tr>

<tr><td>LED PIN R-G-B-R2</td><td>
<select  id="x_LEDconf" name="x_LEDconf">
	<option value="0">NodeMCU 14-13-12-15</option>
	<option value="1">Mini 1  13-12-14-15</option>
	<option value="2">Mini 2  12-13-14-15</option>
	<option value="3">Mini 3  13-14-12-15</option>
</select>
</td></tr>

<tr>
	<td align="right"> OLED Output:</td>
	<td><input type="checkbox" id="OledEnabled" name="OledEnabled"></td>
</tr>


<tr><td colspan="2" align="center"><input type="submit" 				style="width:150px" class="btn btn--m btn--blue" value="Save"></td></tr>

<tr><td colspan="2"><hr></span></td></tr>

<tr><td align="right">RED   :</td><td><span id="x_RED"></span></td><td><span id="x_REDg"></span></td></tr>
<tr><td align="right">GREEN :</td><td><span id="x_GREEN"></span></td><td><span id="x_GREENg"></span></td></tr>
<tr><td align="right">BLUE  :</td><td><span id="x_BLUE"></span></td><td><span id="x_BLUEg"></span></td></tr>
<tr><td align="right">RED 2 :</td><td><span id="x_RED2"></span></td><td><span id="x_RED2g"></span></td></tr>


<tr><td colspan="2"><hr></span></td></tr>
</table>

<table border="0"  cellspacing="0" cellpadding="3" >
<tr>
<td colspan="2" align="center"><input type="submit" name="test" 	 style="width:100px" class="btn btn--m btn--blue" value="Test"></td>
<td colspan="2" align="center"><input type="submit" name="testlp" 	 style="width:100px" class="btn btn--m btn--blue" value="Loop"></td>
<td colspan="2" align="center"><input type="submit" name="broadcast" style="width:100px" class="btn btn--m btn--blue" value="brdcast"></td>
</tr>
</table>

<table border="0"  cellspacing="0" cellpadding="3" >

<tr><td align="right">udp Packets :</td><td>		<span id="x_udpPackets"></span></td></tr> 
<tr><td align="right">udp Pckt Err :</td><td>	<span id="x_udpErrors"></span></td></tr>
<tr><td align="right">udp Seq. Err :</td><td>	<span id="x_udpSqcErrors"></span></td></tr>

<tr><td colspan="2"><hr></span></td></tr>
<tr><td colspan="2" align="center"><input type="submit" name="reset" style="width:120px" class="btn btn--m btn--blue" value="RESET"></td></tr>

<tr><td colspan="2"><hr></span></td></tr>
<tr><td colspan="2" align="center"><input type="submit" name="resetFD" style="width:150px" class="btn btn--m btn--blue" value="FACTORY DEFAULTS"></td></tr>

</table>
</form>
<script>
	

window.onload = function ()
{
	load("style.css","css", function() 
	{
		load("microajax.js","js", function() 
		{
				setValues("/admin/dmxvalues");
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}



</script>
)=====";

// -----------------------------------------------------------------------------------------
void send_DMX_configuration_html()
{
	String TempDeviceName;
	
	if (server.args() > 0 )  // Save Settings
	{
		config.DMXoutputOn 	=  false;
		String temp = "";
		for ( uint8_t i = 0; i < server.args(); i++ ) {

			if (server.argName(i) == "resetFD") {
				  DEBUGGING("\n RESET FACTORY DEFAULT\n");
				  EraseConfig();
				  ESP.restart();
			}

			if (server.argName(i) == "deviceNum") 	config.deviceNum 	=  server.arg(i).toInt();
			if (server.argName(i) == "moduleNum") 	config.moduleNum 	=  server.arg(i).toInt();
			if (server.argName(i) == "universe") 	config.universe 	=  server.arg(i).toInt();
			if (server.argName(i) == "dmxGamma") 	dmxGammaPerc 		=  server.arg(i).toInt();
			if (server.argName(i) == "dmxEnabled")
			{
				config.DMXoutputOn 	=  true;
				dmx_serial_init();
			}
			if (server.argName(i) == "OledEnabled")
			{
				config.OLEDoutputOn 	=  true;
				OLED_init();
			}
			if (server.argName(i) == "x_DxmPwrOffTime") 	config.DxmPowerOffTime 	=  server.arg(i).toInt();

			if (server.argName(i) == "x_LEDconf")
				{
				config.LEDconfig	 		=  server.arg(i).toInt();
				Battery_Init();
				Led_Init();
				}
			if (server.argName(i) == "test") {
				  DEBUGGING("DMX> Test");
				  Led_Test();
				  MessageString = " LED test";
				  NodeServer_text_Multicast(MessageString);
				  server.send ( 200, "text/html", PAGE_DMXConfiguration );
				  return;
			}
			if (server.argName(i) == "testlp") {
				  MessageString = " LED test";
				  if(testmode == true)
				  {
					  testmode = false;
					  MessageString += " off";
					  DEBUGGING("DMX> Test loop stop");
					  Led_Off();
				  }
				  else
				  {
					  testmode = true;
					  MessageString += " on";
					  DEBUGGING("DMX> Test loop start");
				  }
				  NodeServer_text_Multicast(MessageString);
				  server.send ( 200, "text/html", PAGE_DMXConfiguration );
				  return;
			}
			if (server.argName(i) == "broadcast") {
				  DEBUGGING("DMX> Broadcast");
				  NodeServer_ping_Multicast();
				  server.send ( 200, "text/html", PAGE_DMXConfiguration );
				  return;
			}
			if (server.argName(i) == "reset") {
				  DEBUGGING("\nDMX> RESET \n");
				  esp_reset();
				  // ESP.restart();
			}
		}
		TempDeviceName = "Ucl-";
		TempDeviceName += (String) DMX_Actor[config.deviceNum];
		if (config.deviceNum<5)
			config.moduleNum = 0;
		else {
			TempDeviceName += "-";
			TempDeviceName += (String) DMX_Module[config.moduleNum];
		}
		DEBUGGING(TempDeviceName);
		config.DeviceName = TempDeviceName;
		WiFi.hostname(TempDeviceName);

		config.Firmware = FIRMWARE;
		universe = config.universe;
		WriteConfig();
		DMX_Init();
	}
	server.send ( 200, "text/html", PAGE_DMXConfiguration );
	// Serial.println(__FUNCTION__);
	
}

// -----------------------------------------------------------------------------------------
void send_DMX_configuration_values_html()
{
	int iLedRed 	= -1;
	int iLedGreen 	= -1;
	int iLedBlue 	= -1;
	int iLedRed2 	= -1;
	int iLedgRed 	= -1;
	int iLedgGreen 	= -1;
	int iLedgBlue 	= -1;
	int iLedgRed2 	= -1;
	int iErrPckt 	= 0;
	int iErrSqc 	= 0;

	if(udpPacketCount>0)
	{
		iLedRed 	= e131.data[DMX_RED	];  // uiDMXRed
		iLedGreen 	= e131.data[DMX_GREEN	];
		iLedBlue 	= e131.data[DMX_BLUE	];
		iLedRed2 	= e131.data[DMX_RED2	];
		iLedgRed 	= uiDMXRed;  	// with gamma factor applied
		iLedgGreen 	= uiDMXGreen;  	// with gamma factor applied
		iLedgBlue 	= uiDMXBlue;  	// with gamma factor applied
		iLedgRed2 	= uiDMXRed2;  	// with gamma factor applied
		iErrPckt 	= e131.stats.packet_errors;
		iErrSqc 	= e131.stats.sequence_errors;
	}

	String values ="";

	values += "devicename|" 		+  (String) config.DeviceName 		+ "|div\n";
	values += "x_firmware|" 		+  (String) config.Firmware			+ "|div\n";

	values += "deviceNum|" 			+  (String) config.deviceNum 		+ "|input\n";
	values += "moduleNum|" 			+  (String) config.moduleNum 		+ "|input\n";
	values += "universe|" 			+  (String) config.universe 		+ "|input\n";

	values += "x_dmxCh|" 			+  (String) dmxCh 					+ "|div\n";
	values += "x_dmxStart|" 		+  (String) dmxStartChannel 		+ "|div\n";
	values += "dmxGamma|"			+  (String) dmxGammaPerc			+ "|input\n";
	values += "dmxEnabled|" 		+  (String) (config.DMXoutputOn ? "checked" : "") + "|chk\n";
	values += "x_DxmPwrOffTime|" 	+  (String) config.DxmPowerOffTime 	+ "|input\n";

	values += "x_LEDconf|" 			+  (String) config.LEDconfig		+ "|input\n";
	values += "OledEnabled|" 		+  (String) (config.OLEDoutputOn ? "checked" : "") + "|chk\n";

	values += "x_RED|" 				+  (String) iLedRed 				+ "|div\n";
	values += "x_REDg|"				+  (String) iLedgRed 				+ "|div\n";
	values += "x_GREEN|" 			+  (String) iLedGreen 				+ "|div\n";
	values += "x_GREENg|"		 	+  (String) iLedgGreen 				+ "|div\n";
	values += "x_BLUE|" 			+  (String) iLedBlue 				+ "|div\n";
	values += "x_BLUEg|" 			+  (String) iLedgBlue 				+ "|div\n";
	values += "x_RED2|" 			+  (String) iLedRed2 				+ "|div\n";
	values += "x_RED2g|" 			+  (String) iLedgRed2 				+ "|div\n";

	values += "x_udpPackets|" 		+  (String) udpPacketCount 			+ "|div\n";
	values += "x_udpErrors|" 		+  (String) iErrPckt 				+ "|div\n";
	values += "x_udpSqcErrors|"		 +  (String) iErrSqc 				+ "|div\n";

	values += "x_testloop|" 		+  (String) (testmode ? "checked" : "") + "|chk\n";

	server.send ( 200, "text/plain", values);
	// Serial.println(__FUNCTION__);
	
}
