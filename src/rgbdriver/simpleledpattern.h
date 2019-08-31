/*
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

/*  * * * * * * * * * * * * * * * * * * * * * * * * * * *
 Code by Dantali0n
 https://dantalion.nl
* * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef _Simple_Led_Pattern_h
#ifdef __cplusplus
#define _Simple_Led_Pattern_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoSTL.h>
#include <list>
#include "rgbrltypedef.h"
#include "rgbcolor.h"
#include "ledpattern.h"

/**
 * Statefull lighting pattern for led strip.
 * Supports incremental state updates using deltaTime in microseconds
 * @TODO add none linear interpolation for color transistions
 */
class simpleLedPattern : public ledPattern {
    public:
		enum patternModes {
	        linear,
	        cubic,
	        bicubic,
	        akima,
	        rainbow
		};

      	static uint16_t numColorsRainbow;
  	protected:
  		uint32_t currentDuration; // current total animation time in microseconds
  		uint32_t finalDuration; // time the animation finishes in microseconds
      
    	//char stepsRed[100]; 
    	//char stepsGreen[100]; 
    	//char stepsBlue[100]; 

    	std::list<Adafruit_NeoPixel> strips; // the neopixel strips controlled by ledPattern
      
    	rgbColor startColor; 
		  rgbColor currentColor;
		  rgbColor finalColor;
      
    	patternModes patternMode;

    	void updateLinear(uint32_t deltaTime);
    	void updateCubic(uint32_t deltaTime);
    	void updateBiCubic(uint32_t deltaTime);
    	void updatePolynomial(uint32_t deltaTime);
    	void updateCspline(uint32_t deltaTime);
    	void updateAkima(uint32_t deltaTime);
    	void updateRainbow(uint32_t deltaTime);

      void updateStrips(); 
	public:
    	// ledPattern();
    	// ledPattern(unsigned long duration, patternModes patternMode);
		  // ledPattern(rgbColor startColor, rgbColor endColor, unsigned long duration, patternModes patternMode);

    	simpleLedPattern(Adafruit_NeoPixel &strip, uint32_t duration, patternModes patternMode);
    	simpleLedPattern(Adafruit_NeoPixel &strip, rgbColor startColor, rgbColor endColor, uint32_t duration, patternModes patternMode);

      // void do_increment(uint32_t deltaTime);
      // void do_reset();

       void increment(uint32_t deltaTime);
       void reset();
      
    	// void addStrip(Adafruit_NeoPixel strip);
    	// void removeStrip(Adafruit_NeoPixel strip);
    
		  rgbColor getColor();
    	// byte getPercentage();
    	uint32_t getCurrentDuration();
    	uint32_t getFinalDuration();
};

#endif /* __cplusplus */
#endif /* _Led_Pattern_h */
