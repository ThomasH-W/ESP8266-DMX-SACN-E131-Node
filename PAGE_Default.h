#pragma once
#ifndef PAGE_EXAMPLE_H
#define PAGE_EXAMPLE_H
//
//   The EXAMPLE PAGE
//
const char PAGE_example[] PROGMEM = R"=====(
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <h1>ESP12E Module / Thomas Hoeser 2017</h1>     
    
   <div id="mydynamicdata">DMX Channels: </div>   <!-- added a DIV, where the dynamic data goes to -->
   <script>                


		window.onload = function ()
		{
			load("style.css","css", function() 
			{
				load("microajax.js","js", function() 
				{
						setValues("/admin/filldynamicdata");  //-- this function calls the function on the ESP      
				});
			});
		}
		function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}



   </script>

)=====";
#endif


void filldynamicdata()
{
	int iLedRed 	= -1;
	int iLedGreen 	= -1;
	int iLedBlue 	= -1;
	int iLedRed2 	= -1;
	int iErrPckt 	= 0;
	int iErrSqc 	= 0;

	if(udpPacketCount>0)
	{
		iLedRed 	= e131.data[DMX_RED	];
		iLedGreen 	= e131.data[DMX_GREEN	];
		iLedBlue 	= e131.data[DMX_BLUE	];
		iLedRed2 	= e131.data[DMX_RED2	];
		iErrPckt 	= e131.stats.packet_errors;
		iErrSqc 	= e131.stats.sequence_errors;
	}

    String values ="";

    values += "mydynamicdata|";
    values += "firmware : " + (String) config.Firmware;
    values += "<br>";   // Build a string, like this:  ID|VALUE|TYPE
    values += "Unit     : " + (String) config.DeviceName;
    values += "<br>";   // Build a string, like this:  ID|VALUE|TYPE
    values += "Module   : " + (String) config.moduleNum;
    values += "<br>";   // Build a string, like this:  ID|VALUE|TYPE
    values += "DMX Ch   : " + (String) dmxStartChannel;
    values += "<br>";
    values += "<br>";
    values += "Status  : " + (String) esp_status;
    values += "<br>";
    values += "WLAN  : " + (String) rssi_percent;
    values += "%";
    values += " - " + (String) rssi_value;
    values += "db";
    values += "<br>";
    values += "BATT  : " + (String) uiVoltLevel;
    values += "%";
    values += " - " + (String) fVoltLipo;
    values += "V";
    values += "<br>";

    values += "<br>";   // Build a string, like this:  ID|VALUE|TYPE
    values += "RED : " 		+ (String) iLedRed;

    values += "<br>";   // Build a string, like this:  ID|VALUE|TYPE
    values += " GREEN : " 	+ (String) iLedGreen;

    values += "<br>";   // Build a string, like this:  ID|VALUE|TYPE
    values += " BLUE : " 	+ (String) iLedBlue;

    values += "|div\n";   // Build a string, like this:  ID|VALUE|TYPE

	// DEBUGGING(values);
//    values += "mydynamicdata|" + (String) + "This is filled by AJAX. Millis since start: " + (String) millis() + "|div\n";   // Build a string, like this:  ID|VALUE|TYPE
    server.send ( 200, "text/plain", values);   
}

void processExample()
{        
    if (server.args() > 0 )  // Are there any POST/GET Fields ? 
    {
       for ( uint8_t i = 0; i < server.args(); i++ ) {  // Iterate through the fields
            if (server.argName(i) == "firstname") 
            {
                 // Your processing for the transmitted form-variable 
                 String fName = server.arg(i);
            }
        }
    }
    server.send ( 200, "text/html", PAGE_example  ); 
}
