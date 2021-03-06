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

#ifndef _Led_Pattern_h
#ifdef __cplusplus
#define _Led_Pattern_h

#include <Arduino.h>
#include "wultypedef.h"
#include "rgbcolor.h"

/**
 * Statefull lighting pattern for led strip.
 * Supports incremental state updates using deltaTime in microseconds
 * @TODO add none linear interpolation for color transistions
 */
class ledPattern {
    public:
      enum patternModes {
        linear,
        cubic,
        bicubic,
        akima,
        rainbow
      };

      static int numColorsRainbow;
  	private:
    
      /**
       * If being bombarded with serial debug messages please check ledpattern.h for the debug constant and change it to false
       */
      bool finished;
      bool safeDelete;
  		unsigned long currentDuration;
  		unsigned long finalDuration;
      
      //char stepsRed[100]; 
      //char stepsGreen[100]; 
      //char stepsBlue[100]; 
      
      rgbColor startColor;
  		rgbColor currentColor;
  		rgbColor finalColor;
      
      patternModes patternMode;

      void updateLinear(unsigned long deltaTime);
      void updateCubic(unsigned long deltaTime);
      void updateBiCubic(unsigned long deltaTime);
      void updateAkima(unsigned long deltaTime);
      void updateRainbow(unsigned long deltaTime);
	public:
		ledPattern(rgbColor startColor, rgbColor endColor, unsigned long duration, patternModes patternMode);
		void update(unsigned long deltaTime);
    void reset();
		rgbColor getColor();
    // byte getPercentage();
    unsigned long getCurrentDuration();
    unsigned long getFinalDuration();
    bool isFinished();
    bool isSafeDelete();
    void setSafeDelete(bool safeDelete);
};

#endif /* __cplusplus */
#endif /* _Led_Pattern_h */


