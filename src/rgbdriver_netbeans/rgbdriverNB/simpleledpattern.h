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

#include "rgbrltypedef.h"
#include "rgbcolor.h"
#include "ledpattern.h"
#include "templateneopixel.h"

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

      	static const uint16_t numColorsRainbow;
        
    protected:
        uint32_t currentDuration; // current total animation time in microseconds
        uint32_t finalDuration; // time the animation finishes in microseconds
      
    	rgbColor startColor; 
        rgbColor currentColor;
        rgbColor finalColor;
      
    	patternModes patternMode;

    	void updateLinear(uint32_t deltaTime);

        void updateStrips(); 
    public:

    	simpleLedPattern(templateNeopixel &strip, uint32_t duration, patternModes patternMode);
    	simpleLedPattern(templateNeopixel &strip, rgbColor startColor, rgbColor endColor, uint32_t duration, patternModes patternMode);

       void increment(uint32_t deltaTime);
       void reset();
    
	rgbColor getColor();
    	uint32_t getCurrentDuration();
    	uint32_t getFinalDuration();
};

#endif /* __cplusplus */
#endif /* _Led_Pattern_h */