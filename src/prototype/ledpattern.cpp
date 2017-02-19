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
 * Initiate ledPattern
 * @Param startColor the initial rgb color of the pattern
 * @Param endColor the final rgb color of the pattern
 * @Param duration the time to transistion from start to finish in microseconds
 * @Param patternMode the interpolation mode for the color transistion, see ledPattern::patternModes for options
 */
ledPattern::ledPattern(rgbColor startColor, rgbColor endColor, unsigned long duration, ledPattern::patternModes patternMode) {
  this->finished = false;
  this->currentPercentage = 0;
	this->currentDuration = 0UL;
	this->finalDuration = duration;
  this->startColor = startColor;
	this->currentColor = startColor;
	this->finalColor = endColor;
  this->patternMode = patternMode;

  /**
   * @TODO Calculate color difference per rgb by percentage
   * See variables stepsRed, stepsGreen and stepsBlue
   * It is extremely likely the precalculating of steps will make individual update methods per mode redundant!
   */
  switch(this->patternMode) {
    case linear:
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
    default:
      break;
  }
}

/**
 * Update the state of the ledpattern by incrementing the time in microseconds
 * @Param deltaTime the time in microseconds to progress the ledPattern
 * @TODO improve math -> increments start big get smaller towards the end problem resides in stepsize logic
 */
void ledPattern::update(unsigned long deltaTime) {
  if(this->currentDuration >= this->finalDuration) {
    this->finished = true; 
    return;
  }
  
  if(deltaTime > 0) { // prevent division by zero
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
      default:
        break;
    }
  }
}


void ledPattern::updateLinear(unsigned long deltaTime) {
  int timeRemaining = this->finalDuration - this->currentDuration;
  int stepRed = 0;
  int stepGreen = 0;
  int stepBlue = 0;
  
  int intermediateRed = 0;
  int intermediateGreen = 0;
  int intermediateBlue = 0;
  
  if(timeRemaining <= 0) {
    this->currentColor.setRed(this->finalColor.getRed());
    this->currentColor.setGreen(this->finalColor.getGreen());
    this->currentColor.setBlue(this->finalColor.getBlue());
    return; // nothing left to do
  }

  this->currentDuration += deltaTime;
  double percentRemaining = 100 - ((double)this->currentDuration / this->finalDuration) * 100;
  

  intermediateRed = this->finalColor.getRed() - this->startColor.getRed();
  if(intermediateRed != 0) {
      stepRed = round(intermediateRed / percentRemaining); // large step logic
  }

  intermediateGreen = this->finalColor.getGreen() - this->currentColor.getGreen();
  if(intermediateGreen != 0) {
      stepGreen = round(intermediateGreen / percentRemaining); // large step logic
  }

  intermediateBlue = this->finalColor.getBlue() - this->startColor.getBlue();
  if(intermediateBlue != 0) {
      stepBlue = round(intermediateBlue / percentRemaining); // large step logic
  }

  byte newRed = constrain(stepRed, 0, 255);
  byte newGreen = constrain(stepGreen, 0, 255);
  byte newBlue = constrain(stepBlue, 0, 255);
  
  this->currentColor.setRed(newRed);
  this->currentColor.setGreen(newGreen);
  this->currentColor.setBlue(newBlue);
}

void ledPattern::updateCubic(unsigned long deltaTime) {
  
}

void ledPattern::updateBiCubic(unsigned long deltaTime) {
  
}

void ledPattern::updatePolynomial(unsigned long deltaTime) {
  
}

void ledPattern::updateCspline(unsigned long deltaTime) {
  
}

void ledPattern::updateAkima(unsigned long deltaTime) {
  
}

/**
 * returns the current color of the ledPattern
 * @Return rgbColor the current color of the pattern
 */
rgbColor ledPattern::getColor() {
	return this->currentColor;
}

/**
 * returns if the pattern has finished
 * @Return if the pattern has finished
 */
bool ledPattern::isFinished() {
  return this->finished;
}
