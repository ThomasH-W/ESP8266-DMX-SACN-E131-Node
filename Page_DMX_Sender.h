#pragma once

#include "global.h"

const char PAGE_DMX_TX[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<a href="admin.html"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>DMX Sender</strong>
<hr>
send DMX to multicast address:<br>
<form action="" method="get">
<table border="0"  cellspacing="0" cellpadding="3" >

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
</td>

<td>tbd</td><td>
<select  id="x_button" name="x_button">
	<option value="0">off</option>
	<option value="1">on</option>
</select>
</td>
</tr>

<tr>
<td>Red</td><td>
<select  id="x_Red" name="x_Red">
	<option value="0">0</option>
	<option value="1">1</option>
	<option value="10">10</option>
	<option value="50">50</option>
	<option value="100">100</option>
	<option value="255">255</option>
</select>
</td>

<td>Green</td><td>
<select  id="x_Green" name="x_Green">
	<option value="0">0</option>
	<option value="1">1</option>
	<option value="10">10</option>
	<option value="50">50</option>
	<option value="100">100</option>
	<option value="255">255</option>
</select>
</td>

<td>Blue</td><td>
<select  id="x_Blue" name="x_Blue">
	<option value="0">0</option>
	<option value="1">1</option>
	<option value="10">10</option>
	<option value="50">50</option>
	<option value="100">100</option>
	<option value="255">255</option>
</select>
</td>
</tr>


<tr>
	<td align="right"> loop:</td>
	<td><input type="checkbox" id="x_loop" name="x_loop"></td>
</tr>

<tr>
<td colspan="2" align="center"><input type="submit" name="testRGB" 	style="width:150px" class="btn btn--m btn--blue" value="testRGB"></td>
<td colspan="2" align="center"><input type="submit" name="testlp" 	style="width:150px" class="btn btn--m btn--blue" value="LEDloop"></td>
</tr>


</table>

<tr><td colspan="2"><hr></span></td></tr>


<table border="0"  cellspacing="0" cellpadding="3" >
<tr>
<td colspan="2" align="center"><input type="submit" name="Front" 	style="width:80px" class="btn btn--m btn--blue" value="Front"></td>
<td colspan="2" align="center"><input type="submit" name="Back" 	style="width:80px" class="btn btn--m btn--blue" value="Back"></td>
<td colspan="2" align="center"><input type="submit" name="Spot1" 	style="width:80px" class="btn btn--m btn--blue" value="Spot1"></td>
<td colspan="2" align="center"><input type="submit" name="Spot2" 	style="width:80px" class="btn btn--m btn--blue" value="Spot2"></td>
</tr>
<tr>
<td colspan="2" align="center"><input type="submit" name="Kim" 		style="width:80px" class="btn btn--m btn--blue" value="Kim"></td>
<td colspan="2" align="center"><input type="submit" name="Lale" 	style="width:80px" class="btn btn--m btn--blue" value="Lale"></td>
<td colspan="2" align="center"><input type="submit" name="Swan" 	style="width:80px" class="btn btn--m btn--blue" value="Swan"></td>
</tr>
<tr>
<td colspan="2" align="center"><input type="submit" name="Liz" 		style="width:80px" class="btn btn--m btn--blue" value="Liz"></td>
<td colspan="2" align="center"><input type="submit" name="Act5" 	style="width:80px" class="btn btn--m btn--blue" value="Act5"></td>
</tr>
</table>

<tr><td colspan="2"><hr></span></td></tr>

<label for="slider1-range">Red</label>
      <input type="range" id="slider1-range" value="x_Red2" min="0" max="255" step="5" 
      data-highlight="true" />
<br>
<label for="slider2-range">Green</label>
      <input type="range" id="slider2-range" value="x_Green2" min="0" max="255" step="5" 
      data-highlight="true" />
<br>
<label for="slider3-range">Blue</label>
      <input type="range" id="slider3-range" value="x_Blue2" min="0" max="255" step="5" 
      data-highlight="true" />



<tr><td colspan="2" align="center"><input type="submit" name="testval" 	style="width:150px" class="btn btn--m btn--blue" value="set RGB"></td></tr>
<tr><td colspan="2"><hr></span></td></tr>


</form>
<script>
	

window.onload = function ()
{
	load("style.css","css", function() 
	{
		load("microajax.js","js", function() 
		{
				setValues("/admin/dmxtxvalues");
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}



</script>
)=====";


// -----------------------------------------------------------------------------------------
void send_DMX_TX_html()
{
	String TempDeviceName;
	
	if (server.args() > 0 )  // Save Settings
	{
		String temp = "";
		for ( uint8_t i = 0; i < server.args(); i++ ) {

			if (server.argName(i) == "universe") 	config.universe 	=  server.arg(i).toInt();
			if (server.argName(i) == "x_button") 	testButton 			=  server.arg(i).toInt();
			if (server.argName(i) == "x_loop") 		dmxTxLoop 			=  true;
			if (server.argName(i) == "x_Red") 		dmxTxRed 	=  server.arg(i).toInt();
			if (server.argName(i) == "x_Green") 	dmxTxGreen 	=  server.arg(i).toInt();
			if (server.argName(i) == "x_Blue") 		dmxTxBlue	=  server.arg(i).toInt();
			if (server.argName(i) == "x_Red2") 		dmxTxRed2 	=  server.arg(i).toInt();

			if (server.argName(i) == "Front") 	e131_send_rgb_set_ext( 0, dmxTxRed, dmxTxGreen, dmxTxBlue);
			if (server.argName(i) == "Back") 	e131_send_rgb_set_ext( 1, dmxTxRed, dmxTxGreen, dmxTxBlue);
			if (server.argName(i) == "Spot1") 	e131_send_rgb_set_ext( 2, dmxTxRed, dmxTxGreen, dmxTxBlue);
			if (server.argName(i) == "Spot2") 	e131_send_rgb_set_ext( 3, dmxTxRed, dmxTxGreen, dmxTxBlue);
			if (server.argName(i) == "Laser") 	e131_send_rgb_set_ext( 4, dmxTxRed, dmxTxGreen, dmxTxBlue);
			if (server.argName(i) == "Kim") 	e131_send_rgb_set_ext( 5, dmxTxRed, dmxTxGreen, dmxTxBlue);
			if (server.argName(i) == "Lale") 	e131_send_rgb_set_ext( 6, dmxTxRed, dmxTxGreen, dmxTxBlue);
			if (server.argName(i) == "Swan") 	e131_send_rgb_set_ext( 7, dmxTxRed, dmxTxGreen, dmxTxBlue);
			if (server.argName(i) == "Liz") 	e131_send_rgb_set_ext( 8, dmxTxRed, dmxTxGreen, dmxTxBlue);
			if (server.argName(i) == "Act5") 	e131_send_rgb_set_ext( 9, dmxTxRed, dmxTxGreen, dmxTxBlue);

			if (server.argName(i) == "testRGB") {
				testmode = false;
				MessageString = " TX test RGB";
				NodeServer_text_Multicast(MessageString);
				DEBUGGING(MessageString);
    			e131_send_rgb_all(255,0,0); 		delay (500);
    			e131_send_rgb_all(0,255,0);			delay (500);
    			e131_send_rgb_all(0,0,255);			delay (500);
    			e131_send_rgb_all(0,0,0)  ;			delay (500);
			}
			if (server.argName(i) == "testlp") {
				  MessageString = " TX LED test";
				  if(testDmxRGB == true)
				  {
					  testDmxRGB = false;
					  MessageString += " off";
					  DEBUGGING("DMX> TX Test loop stop");
				  }
				  else
				  {
					  testDmxRGB = true;
					  MessageString += " on";
					  DEBUGGING("DMX> TX Test loop start");
				  }
				  NodeServer_text_Multicast(MessageString);
				  DEBUGGING(MessageString);
				  server.send ( 200, "text/html", PAGE_DMX_TX );
				  return;
			}
			if (server.argName(i) == "testval") {
				testmode = false;
				MessageString = " TX set RGB ";
				MessageString += (String) dmxTxRed;
				MessageString += " - ";
				MessageString += (String) dmxTxGreen;
				MessageString += " - ";
				MessageString += (String) dmxTxBlue;
				NodeServer_text_Multicast(MessageString);
				DEBUGGING(MessageString);
				e131_send_rgb_all(dmxTxRed2,dmxTxGreen,dmxTxBlue); 			delay (100);
			}

		} //for ( uint8_t i = 0; i < server.args(); i++ ) {
	} //if (server.args() > 0 )

	server.send ( 200, "text/html", PAGE_DMX_TX );
	// Serial.println(__FUNCTION__);
	
}

// -----------------------------------------------------------------------------------------
void send_DMX_TX_values_html()
{
	String values ="";

	values += "universe|" 		+  (String) config.universe 		+ "|input\n";
	values += "x_button|" 		+  (String) testButton				+ "|input\n";

	values += "x_Red|" 			+  (String) dmxTxRed				+ "|input\n";
	values += "x_Green|" 		+  (String) dmxTxGreen 				+ "|input\n";
	values += "x_Blue|" 		+  (String) dmxTxBlue 				+ "|input\n";
	values += "x_Red2|" 		+  (String) dmxTxRed2				+ "|input\n";

	values += "x_loop|" 		+  (String) (dmxTxLoop ? "checked" : "") + "|chk\n";

	server.send ( 200, "text/plain", values);
	// Serial.println(__FUNCTION__);
	
}
