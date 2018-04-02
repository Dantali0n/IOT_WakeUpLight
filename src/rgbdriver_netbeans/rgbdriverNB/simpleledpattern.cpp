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

/**
 * 
 */
void simpleLedPattern::updateStrips() {
  for (std::list<templateNeopixel>::iterator it = strips.begin(); it != strips.end(); it++) {
      it->toggleVariable(); // dummy method to show the strip is still accessible
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