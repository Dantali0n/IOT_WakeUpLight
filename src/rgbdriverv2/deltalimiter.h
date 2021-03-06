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

#ifndef DELTA_LIMITER_H
#ifdef __cplusplus
#define DELTA_LIMITER_H

#include <Arduino.h>
#include "actor.h"

class DeltaLimiter: public Actor {
  private:
    bool bHasUpdate;
    uint16_t deltaLimit;
  public:
    DeltaLimiter(uint16_t deltaLimit);
    void update(uint32_t deltaTime);
    bool hasUpdate();
};

#endif /* __cplusplus */
#endif /* DELTA_LIMITER_H */