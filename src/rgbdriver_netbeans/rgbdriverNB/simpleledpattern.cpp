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

#include "simpleledpattern.h"

/**
 * If being bombarded with serial debug messages please check ledpattern.h for the debug constant and change it to false
 */

// amount of different colors the rainbow pattern will show during its lifetime
const uint16_t simpleLedPattern::numColorsRainbow = 100;

/**
 * 
 */
//ledPattern::ledPattern() {
//  this->strips = std::list<Adafruit_NeoPixel>();
//  this->finished = false;
//  this->doesFinish = false;
//  this->safeDelete = false;
//  this->currentDuration = 0UL;
//  this->patternMode = patternMode;
//  this->startColor = rgbColor(0,0,0);
//  this->currentColor = rgbColor(0,0,0);
//  this->finalColor = rgbColor(0,0,0);
//  this->patternMode = patternModes::linear;
//}

/**
 * Initiate ledPattern with random start and end colors
 * @Param duration the time to transistion from start to finish in microseconds
 * @Param patternMode the interpolation mode for the color transistion, see ledPattern::patternModes for options
 */
// ledPattern::ledPattern(unsigned long duration, ledPattern::patternModes patternMode) {
//   this->strips = std::list<Adafruit_NeoPixel>();
//   this->finished = false;
//   this->finishes = true;
//   this->paused = false;
//   this->safeDelete = false;
//   this->currentDuration = 0UL;
//   this->finalDuration = duration;
//   this->startColor = rgbColor((byte)random(0,256), (byte)random(0,256), (byte)random(0,256));
//   this->currentColor = startColor;
//   this->finalColor = rgbColor((byte)random(0,256), (byte)random(0,256), (byte)random(0,256));
// }

/**
 * Initiate ledPattern
 * @Param startColor the initial rgb color of the pattern
 * @Param endColor the final rgb color of the pattern
 * @Param duration the time to transistion from start to finish in microseconds
 * @Param patternMode the interpolation mode for the color transistion, see ledPattern::patternModes for options
 */
// ledPattern::ledPattern(rgbColor startColor, rgbColor endColor, unsigned long duration, ledPattern::patternModes patternMode) {
//   this->strips = std::list<Adafruit_NeoPixel>();
//   this->finished = false;
//   this->finishes = true;
//   this->paused = false;
//   this->safeDelete = false;
// 	this->currentDuration = 0UL;
//   this->finalDuration = duration;
//   this->startColor = startColor;
//   this->currentColor = startColor;
//   this->finalColor = endColor;

//  switch(this->patternMode) {
//    case linear:
//      break;
//    case cubic:
//      break;
//    case bicubic:
//      break;
//    case polynomial:
//      break;
//    case cspline:
//      break;
//    case akima:
//      break;
//    case rainbow:
//      break;
//    default:
//      break;
//  }
// }

/**
 * Initiate ledPattern
 * @Param strip the ledstrip that will be controlled by ledPattern
 * @Param duration the time to transistion from start to finish in microseconds
 * @Param patternMode the interpolation mode for the color transistion, see ledPattern::patternModes for options
 */
simpleLedPattern::simpleLedPattern(templateNeopixel &strip, uint32_t duration, simpleLedPattern::patternModes patternMode) : ledPattern(strip) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> colorRand(0, 256);
    std::uniform_int_distribution<> timeRand(1000, 10000);
  this->currentDuration = 0;
  this->finalDuration = duration;
  this->startColor = rgbColor((byte)colorRand(eng), (byte)colorRand(eng), (byte)colorRand(eng));
  this->currentColor = startColor;
  this->finalColor = rgbColor((byte)colorRand(eng), (byte)colorRand(eng), (byte)colorRand(eng));
  this->patternMode = patternMode;
}

/**
 * Initiate ledPattern
 * @Param strip the ledstrip that will be controlled by ledPattern
 * @Param startColor the initial rgb color of the pattern
 * @Param endColor the final rgb color of the pattern
 * @Param duration the time to transistion from start to finish in microseconds
 * @Param patternMode the interpolation mode for the color transistion, see ledPattern::patternModes for options
 */
simpleLedPattern::simpleLedPattern(templateNeopixel &strip, rgbColor startColor, rgbColor endColor, uint32_t duration, simpleLedPattern::patternModes patternMode) : ledPattern(strip) {
  this->currentDuration = 0;
  this->finalDuration = duration;
  this->startColor = startColor;
  this->currentColor = startColor;
  this->finalColor = endColor;
  this->patternMode = patternMode;
}

/**
 * Update the state of the ledpattern by incrementing the time in microseconds
 * @Param deltaTime the time in microseconds to progress the ledPattern
 */
void simpleLedPattern::increment(uint32_t deltaTime) {
  // dont do anything during the updating of paused patterns
  if(this->paused) {
    std::cout << "increment: paused" << std::endl;
    return;
  }

  // Determine if the pattern has finished animating
  if(this->currentDuration >= this->finalDuration) {
    std::cout << "increment: finished" << std::endl;
    
    // if the pattern finishes end it
    if(this->finishes) {
      std::cout << "increment: finishes" << std::endl;
      this->finished = true;
      return;
    }
    // if it loops reset the currentDuration
    else {
      std::cout << "increment: finished but reset" << std::endl;
      this->currentDuration = 0;
    }
  }

  // prevent division by zero
  if(deltaTime > 0) { 

    this->currentDuration += deltaTime;
    if(this->currentDuration > this->finalDuration) { this->currentDuration = this->finalDuration; } // clamp duration
    
    switch(patternMode) {
      case linear:
        updateLinear(deltaTime);
        break;
      case cubic:
        break;
      case bicubic:
        break;
      case polynomial:
        break;
      case cspline:
        break;
      case akima:
        break;
      case rainbow:
        break;
      default:
        break;
    }
  }
}

/**
 * Resets the pattern for another round from the start
 * @warning Not the same as looping
 */
void simpleLedPattern::reset() {
  this->finished = false;
  this->currentDuration = 0;
  this->currentColor = this->startColor;
}


/**
 * Linear transistioning between two rgb values with a function of deltaTime
 * @math endtime / currenttime = divTime
 * @math endcolor - scolor = divColor
 * @math divcolor / divTime = currentcolor
 */
void simpleLedPattern::updateLinear(uint32_t deltaTime) {
  bool changed = false; // if their is any change in the pattern
  
  double divTime = this->finalDuration / (double)this->currentDuration;
  
  int16_t newRed = 0;
  int16_t newGreen = 0;
  int16_t newBlue = 0;  

  int16_t intermediateRed = 0;
  int16_t intermediateGreen = 0;
  int16_t intermediateBlue = 0;

  //intermediateRed = (int)this->finalColor.getRed() - (int)this->currentColor.getRed();
  intermediateRed = (int16_t)this->finalColor.getRed() - (int16_t)this->startColor.getRed();
  // if(RGBRL_DEBUG) { Serial.print(intermediateRed); Serial.print('|'); }
  if(intermediateRed != 0) {
      newRed = round(intermediateRed / divTime); // large step logic
      if(RGBRL_DEBUG) { std::cout << newRed << '|'; }
      newRed += this->startColor.getRed();
  }

  //intermediateGreen = (int)this->finalColor.getGreen() - (int)this->currentColor.getGreen();
  intermediateGreen = (int16_t)this->finalColor.getGreen() - (int16_t)this->startColor.getGreen();
  // if(RGBRL_DEBUG) { Serial.print(intermediateGreen); Serial.print('|'); }
  if(intermediateGreen != 0) {
      newGreen = round(intermediateGreen / divTime); // large step logic
      if(RGBRL_DEBUG) { std::cout << newGreen << '|'; }
      newGreen += this->startColor.getGreen();
  }

  // intermediateBlue = (int)this->finalColor.getBlue() - (int)this->startColor.getBlue();
  intermediateBlue = (int16_t)this->finalColor.getBlue() - (int16_t)this->startColor.getBlue();
  // if(RGBRL_DEBUG) { Serial.println(intermediateBlue); }
  if(intermediateBlue != 0) {
      newBlue = round(intermediateBlue / divTime); // large step logic
      if(RGBRL_DEBUG) { std::cout << newBlue; }
      newBlue += this->startColor.getBlue();
  }

  if(RGBRL_DEBUG) { std::cout << std::endl; }

  if(intermediateRed != 0) { changed = true; this->currentColor.setRed(newRed); }
  if(intermediateGreen != 0) { changed = true; this->currentColor.setGreen(newGreen); }
  if(intermediateBlue != 0) { changed = true; this->currentColor.setBlue(newBlue); }

  // only update the led stripts if the color value changes
  if(changed) updateStrips();
}

//void ledPattern::updateLinear(unsigned long deltaTime) {
//  int timeRemaining = this->finalDuration - this->currentDuration;
//  int stepRed = 0;
//  int stepGreen = 0;
//  int stepBlue = 0;
//  
//  int intermediateRed = 0;
//  int intermediateGreen = 0;
//  int intermediateBlue = 0;
//  
//  if(timeRemaining <= 0) {
//    if(WUL_DEBUG) { Serial.print(this->finalColor.getRed()); Serial.print(this->finalColor.getGreen()); Serial.println(this->finalColor.getBlue()); }
//    this->currentColor.setRed(this->finalColor.getRed());
//    this->currentColor.setGreen(this->finalColor.getGreen());
//    this->currentColor.setBlue(this->finalColor.getBlue());
//    return; // nothing left to do
//  }
//
//  this->currentDuration += deltaTime;
//  double percentRemaining = 100 - ((double)this->currentDuration / this->finalDuration) * 100;
//  currentPercentage = abs(percentRemaining - 100); // I know abs is pretty heavy but at 80mhz who cares really?
//
//  //intermediateRed = (int)this->finalColor.getRed() - (int)this->startColor.getRed();
//  intermediateRed = (int)this->finalColor.getRed() - (int)this->currentColor.getRed();
//  if(WUL_DEBUG) { Serial.print(intermediateRed); Serial.print('|'); }
//  if(intermediateRed != 0) {
//      stepRed = round(intermediateRed / percentRemaining); // large step logic
//  }
//
//  //intermediateGreen = (int)this->finalColor.getGreen() - (int)this->currentColor.getGreen();
//  intermediateGreen = (int)this->finalColor.getGreen() - (int)this->currentColor.getGreen();
//  if(WUL_DEBUG) { Serial.print(intermediateGreen); Serial.print('|'); }
//  if(intermediateGreen != 0) {
//      stepGreen = round(intermediateGreen / percentRemaining); // large step logic
//  }
//
//  // intermediateBlue = (int)this->finalColor.getBlue() - (int)this->startColor.getBlue();
//  intermediateBlue = (int)this->finalColor.getBlue() - (int)this->currentColor.getBlue();
//  if(WUL_DEBUG) { Serial.println(intermediateBlue); }
//  if(intermediateBlue != 0) {
//      stepBlue = round(intermediateBlue / percentRemaining); // large step logic
//  }
//
//  if(WUL_DEBUG) {
//    Serial.print("Steps:");
//    Serial.print(stepRed);
//    Serial.print(',');
//    Serial.print(stepGreen);
//    Serial.print(',');
//    Serial.println(stepBlue);
//  }
//
////  byte newRed = constrain(stepRed, 0, 255);
////  byte newGreen = constrain(stepGreen, 0, 255);
////  byte newBlue = constrain(stepBlue, 0, 255);
//
//  byte newRed = this->currentColor.getRed();
//  byte newGreen = this->currentColor.getGreen();
//  byte newBlue = this->currentColor.getBlue();
//  
//  newRed += stepRed;
//  newGreen += stepGreen;
//  newBlue += stepBlue;
//  
//  this->currentColor.setRed(newRed);
//  this->currentColor.setGreen(newGreen);
//  this->currentColor.setBlue(newBlue);
//}

//void simpleLedPattern::updateCubic(unsigned long deltaTime) {
//  
//}

//void simpleLedPattern::updateBiCubic(unsigned long deltaTime) {
//  
//}

//void simpleLedPattern::updatePolynomial(unsigned long deltaTime) {
//  
//}

//void simpleLedPattern::updateCspline(unsigned long deltaTime) {
//  
//}

//void simpleLedPattern::updateAkima(unsigned long deltaTime) {
//  
//}

//void simpleLedPattern::updateRainbow(unsigned long deltaTime) {
//  
//}

/**
 * 
 */
void simpleLedPattern::updateStrips() {
  for (std::list<templateNeopixel>::iterator it = strips.begin(); it != strips.end(); it++) {
//    int numPixels = it->numPixels();
//    for (int iPixel = 0; iPixel < numPixels; iPixel++) {
//      it->setPixelColor(iPixel,
//          (byte)this->currentColor.getRed(),
//          (byte)this->currentColor.getGreen(),
//          (byte)this->currentColor.getBlue());
//    }
//    it->show();
  }
}

/**
 * returns the current color of the ledPattern
 * @Return rgbColor the current color of the pattern
 */
rgbColor simpleLedPattern::getColor() {
	return this->currentColor;
}

/**
 * returns the current duration of the ledPattern
 * @Return unsigned long microtime duration
 */
uint32_t simpleLedPattern::getCurrentDuration() {
  return this->currentDuration;
}

/**
 * returns the final duration of the ledPattern
 * @Return unsigned long microtime duration
 */
uint32_t simpleLedPattern::getFinalDuration() {
  return this->finalDuration;
}