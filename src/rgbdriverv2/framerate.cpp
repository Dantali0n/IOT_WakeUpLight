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
  bHasUpdate = false;
  framerate = 0;
  reset();
}

void FrameRate::update(uint32_t deltaTime) {
  lastUpdate += deltaTime;
  
  if(lastUpdate > MICRO_TO_MINUTE) {
    bHasUpdate = true;
    lastUpdate = 0;
    framerate = frames / 60;
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
  return framerate;
}

uint32_t FrameRate::getFrames() {
  return frames;
}

bool FrameRate::hasUpdate() {
  bool holdBHasUpdate = bHasUpdate;
  bHasUpdate = false;
  return holdBHasUpdate;
}


