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
ledPattern::ledPattern(templateNeopixel &strip) {
 this->strips = std::list<templateNeopixel>();
 this->finished = false;
 this->finishes = true;
 this->paused = false;
 this->disable = false;
 this->safeDelete = false;

 this->strips.push_front(strip);
}

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

templateNeopixel* ledPattern::getStrip() {
  return &strips.front();
}

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

