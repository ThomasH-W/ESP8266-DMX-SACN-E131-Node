/*
 * P9813.ino
 *
 *  Created on: 11.01.2017
 *      Author: Thomas
 */

#include "global.h"


// p9813.setColorRGB(       0, *bufferLED1,  *bufferLED2, *bufferLED3);

//-----------------------------------------------------------------------------------------
void P9813_show(int xx, uint8_t LedRed, uint8_t LedGreen, uint8_t LedBlue)
{
    if(config.P9813outputOn == true)
	{
		p9813.setColorRGB(  0, LedRed,  LedGreen, LedBlue);
		// DEBUGGING("p");
	}
}

//-----------------------------------------------------------------------------------------
void P9813_init(void)
{
    if(config.P9813outputOn == true)
	{
		pinMode(P9813_CLK , OUTPUT);
		pinMode(P9813_DATA, OUTPUT);
		delay(10);

		p9813.init();
		p9813.setColorRGB(0, 0, 0, 0);
		DEBUGGING("> P9813 set");
	}
}

//-----------------------------------------------------------------------------------------
void P9813_test(void)
{
    if(config.P9813outputOn == true)
	{

	DEBUGGING("> blink P9813 ...");
	byte power = 0;

for (byte j=0; j<20; j++)
{
  for (byte i=0; i<P9813_NUM_LEDS; i++)
  {
    if (i%2 == 0)
    	p9813.setColorRGB(i, power, 0, 0);
    else
    	p9813.setColorRGB(i, 0, 255-power, 0);
  }
  power+= 10;

  delay(20);
}

for (byte j=0; j<20; j++)
{
  for (byte i=0; i<P9813_NUM_LEDS; i++)
  {
    if (i%2 == 0)
    	p9813.setColorRGB(i, 0, power, 0);
    else
    	p9813.setColorRGB(i, 0, 0, 255-power);
  }
  power+= 10;

  delay(20);
}
p9813.setColorRGB(0, 0, 0, 0);

	}

}
