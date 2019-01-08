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

#ifndef FRAME_RATE_H
#define FRAME_RATE_H

#include <Arduino.h>
#include "actor.h"

class FrameRate: public Actor {
  private:
    static const uint32_t MICRO_TO_MINUTE = 60000000;
    bool bHasUpdate;
    uint32_t frames;
    uint32_t framerate;
  public:
    FrameRate();
    void update(uint32_t deltaTime);
    void reset();
    uint32_t get();
    uint32_t getFrames();
    bool hasUpdate();
};

#endif

