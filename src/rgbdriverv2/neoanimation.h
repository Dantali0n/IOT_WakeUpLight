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

#ifndef _NEO_ANIMATION_H
#ifdef __cplusplus
#define _NEO_ANIMATION_H

#include <Arduino.h>
#include "typedefinitions.h"
#include "actor.h"
#include "neopatterns.h"

// Animation types supported:
enum animation { 
	NO_PATTERN_UPDATE, 
	RAINBOW, 
	COLOR_SOLID, STROBE_SOLID,
	COLOR_WIPE_SOLID, COLOR_WIPE_CHRISTMAS, COLOR_WIPE_RANDOM, 
	SCANNER_SOLID, SCANNER_RANDOM, 
	FADE_SOLID, FADE_RANDOM,
	FIRE_SOLID,
	METEOR_SOLID, METEOR_SCANNER_SOLID
};

// String representation of each animation type from the enum
const static char *LED_ANIMATION_STRING[] = {
    "", 
    "RAINBOW", 
    "COLOR_SOLID", "STROBE_SOLID",
    "COLOR_WIPE_SOLID", "COLOR_WIPE_CHRISTMAS", "COLOR_WIPE_RANDOM", 
    "SCANNER_SOLID", "SCANNER_RANDOM", 
    "FADE_SOLID", "FADE_RANDOM",
    "FIRE_SOLID",
    "METEOR_SOLID", "METEOR_SCANNER_SOLID"
};

class NeoAnimation : public NeoPatterns {
public:
	animation currentAnimation;

	NeoAnimation(uint16_t pixels, uint8_t pin, uint8_t type);
	NeoAnimation(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)(NeoAnimation* stick));

	void animationSwitch(animation anim);

protected:
 	static void patternComplete(NeoAnimation* stick);

 	// STROBE_COLOR Variables
 	// Use with speed 25 for best result
 	const uint8_t NUM_STROBE_TICKS = 5; 
	uint8_t strobeTicks = 0;
};

#endif /* __cplusplus */
#endif /* _NEO_ANIMATION_H */