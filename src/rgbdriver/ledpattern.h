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
#include <Adafruit_NeoPixel.h>
#include <ArduinoSTL.h>
#include <list>
#include "rgbrltypedef.h"
#include "rgbcolor.h"

/**
 * Statefull lighting pattern for led strip.
 * Supports incremental state updates using deltaTime in microseconds
 * @TODO add none linear interpolation for color transistions
 */
class ledPattern {
    public:
      // enum patternModes {
      //   linear,
      //   cubic,
      //   bicubic,
      //   polynomial,
      //   cspline,
      //   akima,
      //   rainbow
      // };

      // static int numColorsRainbow;
  	protected:
      bool finished; // if the ledPattern has finished animating
      bool finishes; // if the ledPattern ever finishes animating
      bool paused; // pause the progression of the animation
      bool disable; // disable the ledStrip
      bool safeDelete; // if the ledPattern is created with the new keyword and can safely be called by delete
  		// unsigned long currentDuration; // current total animation time in microseconds
  		// unsigned long finalDuration; // time the animation finishes in microseconds
      
      //char stepsRed[100]; 
      //char stepsGreen[100]; 
      //char stepsBlue[100]; 

      std::list<Adafruit_NeoPixel> strips; // the neopixel strips controlled by ledPattern
      
      // rgbColor startColor; 
  		// rgbColor currentColor;
  		// rgbColor finalColor;
      
      // patternModes patternMode;

      // void updateLinear(unsigned long deltaTime);
      // void updateCubic(unsigned long deltaTime);
      // void updateBiCubic(unsigned long deltaTime);
      // void updatePolynomial(unsigned long deltaTime);
      // void updateCspline(unsigned long deltaTime);
      // void updateAkima(unsigned long deltaTime);
      // void updateRainbow(unsigned long deltaTime);
  private:
//    virtual void do_increment(uint32_t deltaTime) = 0;
//    virtual void do_reset() = 0;
	public:
    // virtual ~ledPattern();
    // ledPattern();
    // ledPattern(unsigned long duration, patternModes patternMode);
		// ledPattern(rgbColor startColor, rgbColor endColor, unsigned long duration, patternModes patternMode);

    ledPattern(Adafruit_NeoPixel &strip);

    // ledPattern(Adafruit_NeoPixel strip, unsigned long duration, patternModes patternMode);
    // ledPattern(Adafruit_NeoPixel strip, rgbColor startColor, rgbColor endColor, unsigned long duration, patternModes patternMode);
   
		void increment(uint32_t deltaTime);
    void reset();

    // void addStrip(Adafruit_NeoPixel strip);
    // void removeStrip(Adafruit_NeoPixel strip);

    Adafruit_NeoPixel* getStrip();
    
		// rgbColor getColor();
    // byte getPercentage();
    // unsigned long getCurrentDuration();
    // unsigned long getFinalDuration();
    bool isFinished();
    bool doesFinish();
    bool isPaused();
    void setPaused(bool paused);
    bool isDisabled();
    void setDisabled(bool disabled);
    bool isSafeDelete();
    void setSafeDelete(bool safeDelete);
};

#endif /* __cplusplus */
#endif /* _Led_Pattern_h */


