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

#include "ledpattern.h"

/**
 * If being bombarded with serial debug messages please check ledpattern.h for the debug constant and change it to false
 */

/**
 * 
 */
ledPattern::ledPattern(Adafruit_NeoPixel &strip) {
 this->strips = std::list<Adafruit_NeoPixel>();
 this->finished = false;
 this->finishes = true;
 this->paused = false;
 this->disable = false;
 this->safeDelete = false;

 this->strips.push_front(strip);
}

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
 * Update the state of the ledpattern by incrementing the time in microseconds
 * @Param deltaTime the time in microseconds to progress the ledPattern
 */
void ledPattern::increment(uint32_t deltaTime) {
  // do_increment(deltaTime);
  
  // dont do anything during the updating of paused patterns
  if(this->paused) {
    return;
  }

  // if the pattern finishes end it
  if(this->finishes) {
    this->finished = true;
    return;
  }
}

/**
 * Resets the pattern for another round from the start
 * @warning Not the same as looping
 */
void ledPattern::reset() {
  // do_reset();
  
  this->finished = false;
}


/**
 * Linear transistioning between two rgb values with a function of deltaTime
 * @math endtime / currenttime = divTime
 * @math endcolor - scolor = divColor
 * @math divcolor / divTime = currentcolor
 */
// void ledPattern::updateLinear(unsigned long deltaTime) {
//   double divTime = this->finalDuration / (double)this->currentDuration;
  
//   int newRed = 0;
//   int newGreen = 0;
//   int newBlue = 0;  

//   int intermediateRed = 0;
//   int intermediateGreen = 0;
//   int intermediateBlue = 0;

//   //intermediateRed = (int)this->finalColor.getRed() - (int)this->currentColor.getRed();
//   intermediateRed = (int)this->finalColor.getRed() - (int)this->startColor.getRed();
//   // if(RGBRL_DEBUG) { Serial.print(intermediateRed); Serial.print('|'); }
//   if(intermediateRed != 0) {
//       newRed = round(intermediateRed / divTime); // large step logic
//       if(RGBRL_DEBUG) { Serial.print(newRed); Serial.print('|'); }
//       newRed += this->startColor.getRed();
//   }

//   //intermediateGreen = (int)this->finalColor.getGreen() - (int)this->currentColor.getGreen();
//   intermediateGreen = (int)this->finalColor.getGreen() - (int)this->startColor.getGreen();
//   // if(RGBRL_DEBUG) { Serial.print(intermediateGreen); Serial.print('|'); }
//   if(intermediateGreen != 0) {
//       newGreen = round(intermediateGreen / divTime); // large step logic
//       if(RGBRL_DEBUG) { Serial.print(newGreen); Serial.print('|'); }
//       newGreen += this->startColor.getGreen();
//   }

//   // intermediateBlue = (int)this->finalColor.getBlue() - (int)this->startColor.getBlue();
//   intermediateBlue = (int)this->finalColor.getBlue() - (int)this->startColor.getBlue();
//   // if(RGBRL_DEBUG) { Serial.println(intermediateBlue); }
//   if(intermediateBlue != 0) {
//       newBlue = round(intermediateBlue / divTime); // large step logic
//       if(RGBRL_DEBUG) { Serial.print(newBlue); }
//       newBlue += this->startColor.getBlue();
//   }

//   if(RGBRL_DEBUG) { Serial.println(); }

//   if(intermediateRed != 0) this->currentColor.setRed(newRed);
//   if(intermediateGreen != 0) this->currentColor.setGreen(newGreen);
//   if(intermediateBlue != 0) this->currentColor.setBlue(newBlue);
// }

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

// void ledPattern::updateCubic(unsigned long deltaTime) {
  
// }

// void ledPattern::updateBiCubic(unsigned long deltaTime) {
  
// }

// void ledPattern::updatePolynomial(unsigned long deltaTime) {
  
// }

// void ledPattern::updateCspline(unsigned long deltaTime) {
  
// }

// void ledPattern::updateAkima(unsigned long deltaTime) {
  
// }

// void ledPattern::updateRainbow(unsigned long deltaTime) {
  
// }


Adafruit_NeoPixel* ledPattern::getStrip() {
  return &strips.front();
}
/**
 * returns the current color of the ledPattern
 * @Return rgbColor the current color of the pattern
 */
// rgbColor ledPattern::getColor() {
// 	return this->currentColor;
// }

/**
 * returns the current duration of the ledPattern
 * @Return unsigned long microtime duration
 */
// unsigned long ledPattern::getCurrentDuration() {
//   return this->currentDuration;
// }

/**
 * returns the final duration of the ledPattern
 * @Return unsigned long microtime duration
 */
// unsigned long ledPattern::getFinalDuration() {
//   return this->finalDuration;
// }

/**
 * returns if the pattern has finished
 * @Return if the pattern has finished
 */
bool ledPattern::isFinished() {
  return this->finished;
}

/**
 * returns if the pattern ever finishes
 * @Return if the pattern ever finishes
 */
bool ledPattern::doesFinish() {
  return this->finishes;
}

/**
 * returns if the pattern has finished
 * @Return if the pattern has finished
 */
bool ledPattern::isPaused() {
  return this->paused;
}

/**
 * Set if the ledPattern should pause the progression of animations
 */
void ledPattern::setPaused(bool paused) {
  this->paused = paused;
}

/**
 * returns if the pattern is disabled
 * @Return if the pattern is disabled
 */
bool ledPattern::isDisabled() {
  return this->disable;
}

/**
 * Set if the ledPattern should disable the display on the ledStrips
 */
void ledPattern::setDisabled(bool disable) {
  this->disable = disable;
}

/**
 * returns if the pattern has finished
 * @Return if the pattern has finished
 */
bool ledPattern::isSafeDelete() {
  return this->safeDelete;
}

/**
 * Set if the ledPattern can be safely called delete upon
 */
void ledPattern::setSafeDelete(bool safeDelete) {
  this->safeDelete = safeDelete;
}


