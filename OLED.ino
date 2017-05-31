/*
 * OLED.ino
 *
 *  Created on: 11.01.2017
 *      Author: Thomas
 */

#include "global.h"
#include "OLED_images.h"

//-----------------------------------------------------------------------------------------
void OLED_print(int16_t x, int16_t y, String message)
{
    if(config.OLEDoutputOn == true)
	{
    	display.setTextAlignment(TEXT_ALIGN_LEFT);
    	display.drawString(x, y, message);
    	// display.drawStringMaxWidth(x, y, 128,message);
    	yield();
    	display.display();
    	yield();
	}
}

//-----------------------------------------------------------------------------------------
void OLED_Wlan_bar(int16_t x, int16_t y,int rssi_percent)
{
	int bar_width  		= 5;
	int bar_heigth_max 	= 20; // 100%
	int bar_heigth 		= bar_heigth_max;
	int bar_offset 		= bar_width +2;

    if(config.OLEDoutputOn == true)
	{

    	if(rssi_percent>=20){
    		bar_heigth = bar_heigth_max*0.25;
    		display.fillRect  (x + (bar_offset*0) , y - bar_heigth , bar_width, bar_heigth);
    		// display.drawString(x + (bar_offset*0) , y +30 , "1");
    	}

    	if(rssi_percent>=40){
    		bar_heigth = bar_heigth_max*0.5;
    		display.fillRect  (x + (bar_offset*1) , y - bar_heigth , bar_width, bar_heigth);
    		// display.drawString(x + (bar_offset*1) , y +30 , "2");
    	}

    	if(rssi_percent>=60){
    		bar_heigth = bar_heigth_max*0.75;
    		display.fillRect  (x + (bar_offset*2) , y - bar_heigth , bar_width, bar_heigth);
    		// display.drawString(x + (bar_offset*2) , y +30 , "3");
    	}

    	if(rssi_percent>80){
    		bar_heigth = bar_heigth_max;
    		display.fillRect  (x + (bar_offset*3) , y - bar_heigth , bar_width, bar_heigth);
    		// display.drawString(x + (bar_offset*3) , y +30 , "4");
    	}
}
}
//-----------------------------------------------------------------------------------------
void OLED_logo(int16_t xMove, int16_t yMove, int16_t width, int16_t height, const char *xbm, boolean img_clear)
{
    if(config.OLEDoutputOn == true)
	{
        if(img_clear == true)
        {
        	display.clear();
           	yield();
        }
        // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
        // on how to create xbm files
        display.drawXbm(xMove, yMove, width, height, xbm);
        // display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
        yield();
        display.display();
		DEBUGGING("OLED> show logo");
	}
}
//-----------------------------------------------------------------------------------------
void OLED_clear()
{
    if(config.OLEDoutputOn == true)
	{
    	display.clear();
       	yield();
	}
}

//-----------------------------------------------------------------------------------------
void OLED_init(void)
{
    if(config.OLEDoutputOn == true)
	{
    	display.init();
    	display.flipScreenVertically();
    	display.setFont(ArialMT_Plain_10);
    	display.clear();
		DEBUGGING("> display begin");
	}
    else
    	DEBUGGING("> DISPLAY NOT USED");
}
