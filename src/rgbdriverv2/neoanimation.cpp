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

#include "neoanimation.h"

/**
 * 
 */
NeoAnimation::NeoAnimation(uint16_t pixels, uint8_t pin, uint8_t type)
	:NeoPatterns(pixels, pin, type, NeoAnimation::patternComplete)
{
    OnComplete = NeoAnimation::patternComplete;
}

/**
 * 
 */
NeoAnimation::NeoAnimation(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)(NeoAnimation* stick))
	:NeoPatterns(pixels, pin, type, callback)
{
    OnComplete = callback;
}

/**
 * Switching between available animations from neoanimation
 * Controls the underlying neopatterns attributes and functions.
 * @param animation the desired animation
 */
void NeoAnimation::animationSwitch(animation anim) {
  	switch(anim){
    case RAINBOW:
        this->currentAnimation = RAINBOW;
        this->RainbowCycle(this->Interval);
        break;
    case SOLID_COLOR:
        this->currentAnimation = SOLID_COLOR;
        this->NoPattern();
        this->ColorSet(this->Color1);
        break;
    case COLOR_WIPE_SOLID:
    	this->currentAnimation = COLOR_WIPE_SOLID;
        // TODO: Implement
        break;
    case COLOR_WIPE_CHRISTMAS:
        this->currentAnimation = COLOR_WIPE_CHRISTMAS;
        this->ColorWipe(16711680, this->Interval);
        break;
    case COLOR_WIPE_RANDOM:
        this->currentAnimation = COLOR_WIPE_RANDOM;
        this->ColorWipe(random(16777215), this->Interval);
        break;
    case SCANNER_SOLID:
        this->currentAnimation = SCANNER_SOLID;
        this->Scanner(this->Color1, this->Interval, DUAL);
        break;
    case SCANNER_RANDOM:
        this->currentAnimation = SCANNER_RANDOM;
        this->Scanner(random(16777215), this->Interval, DUAL);
        break;
    case FADE_SOLID:
    	this->currentAnimation = FADE_SOLID;
    	this->Fade(this->Color1, this->Color2, 254, this->Interval);
        break;
    case FADE_RANDOM:
    	this->currentAnimation = FADE_RANDOM;
        this->Fade(random(16777215), random(16777215), 254, this->Interval);
        break;
    default:
        break;
  	}
}

/**
 * 
 * @param NeoAnimation* 
 */
void NeoAnimation::patternComplete(NeoAnimation* stick) {
  	switch(stick->currentAnimation){
  	case RAINBOW:
	    break;
	case SOLID_COLOR:
	  	break;
	case COLOR_WIPE_CHRISTMAS:
		stick->Reverse();
		if(stick->Color1 == 16711680) {
			stick->Color1 = 65280;
		}
		else {
			stick->Color1 = 16711680;
		}
		break;
	case COLOR_WIPE_RANDOM:
		stick->Reverse();
		stick->Color1 = random(2147483646);
		stick->Color2 = random(2147483646);
		break;
	case SCANNER_SOLID:
		// stick->Reverse();
		break;
	case SCANNER_RANDOM:
		stick->Reverse();
		stick->Color1 = random(16777215);
		stick->Color2 = random(16777215);
		break;
	case FADE_SOLID:
		stick->Reverse();
		break;
	case FADE_RANDOM:
		if(stick->Direction == REVERSE) {
			stick->Color2 = random(16777215);
		}
		else {
	    	stick->Color1 = random(16777215);
	  	}
	  	stick->Reverse();
	  	break;
	default:
      	break;
    }
}