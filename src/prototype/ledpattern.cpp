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
 */
ledPattern::ledPattern(rgbColor startColor, rgbColor endColor, unsigned long duration) {
  this->finished = false;
	this->currentDuration = 0UL;
	this->finalDuration = duration;
	this->currentColor = startColor;
	this->finalColor = endColor;
}

/**
 * Update the state of the ledpattern by incrementing the time in microseconds
 * @Param deltaTime the time in microseconds to progress the ledPattern
 * @TODO improve math -> size of early increments is to low
 */
void ledPattern::update(unsigned long deltaTime) {
  if(this->currentDuration >= this->finalDuration) {
    this->finished = true; 
    return;
  }
  
  if(deltaTime > 0) { // prevent division by zero
    int timeRemaining = this->finalDuration - this->currentDuration;
    int stepRed = 0;
    int stepGreen = 0;
    int stepBlue = 0;
    
    int intermediateRed = 0;
    int intermediateGreen = 0;
    int intermediateBlue = 0;
    
    if(timeRemaining <= 0) return; // nothing left to do
    
    int stepsRemaining = ceil(timeRemaining / deltaTime);
    this->currentDuration += deltaTime;

    intermediateRed = this->finalColor.getRed() - this->currentColor.getRed();
    if(intermediateRed != 0) {
        stepRed = floor(intermediateRed / stepsRemaining); // large step logic
        if(stepRed == 0 && intermediateRed / (float)stepsRemaining >= 0.5 || stepRed == 0 && intermediateRed / (float)stepsRemaining <= -0.5) { // small step logic
          stepRed = 1;
        }
    }

    intermediateGreen = this->finalColor.getGreen() - this->currentColor.getGreen();
    if(intermediateGreen != 0) {
        stepGreen = floor(intermediateGreen / stepsRemaining); // large step logic
        if(stepGreen == 0 && intermediateGreen / (float)stepsRemaining >= 0.5 || stepGreen == 0 && intermediateGreen / (float)stepsRemaining <= -0.5) { // small step logic
          stepGreen = 1;
        }
    }

    intermediateBlue = this->finalColor.getBlue() - this->currentColor.getBlue();
    if(intermediateBlue != 0) {
        stepBlue = floor(intermediateBlue / stepsRemaining); // large step logic
        if(stepBlue == 0 && intermediateBlue / (float)stepsRemaining >= 0.5 || stepBlue == 0 && intermediateBlue / (float)stepsRemaining <= -0.5) { // small step logic
          stepBlue = 1;
        }
    }
  
    byte newRed = constrain((int)this->currentColor.getRed() + stepRed, 0, 255);
    byte newGreen = constrain((int)this->currentColor.getGreen() + stepGreen, 0, 255);
    byte newBlue = constrain((int)this->currentColor.getBlue() + stepBlue, 0, 255);
    
    this->currentColor.setRed(newRed);
    this->currentColor.setGreen(newGreen);
    this->currentColor.setBlue(newBlue);
  }
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
