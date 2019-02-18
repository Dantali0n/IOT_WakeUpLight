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

#include "framerate.h"

FrameRate::FrameRate():Actor() {
  lastDelta = 0;
  reset();
}

void FrameRate::update(uint32_t deltaTime) {
  lastUpdate += deltaTime;
  lastDelta = deltaTime;
  
  if(lastUpdate >= TIME::MICROS_TO_MINUTES) {
    lastUpdate = 0;
    reset();
  }
  else {
    frames++;
  }
}

void FrameRate::reset() {
  frames = 0;
}

uint32_t FrameRate::get() {
  if(lastDelta == 0) return 0; // can't determine framerate with a 0 delta
  return TIME::MICROS_TO_SECONDS / lastDelta;
}

uint32_t FrameRate::getAverage() {
  if(lastUpdate == 0) return 0; // can't determine framerate with a 0 lastUpdate
  return (TIME::MICROS_TO_SECONDS / lastUpdate) * frames;
}

uint32_t FrameRate::getFrames() {
  return frames;
}
