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
    case SUNRISE:
        this->currentAnimation = SUNRISE;
        this->Interval = SUNRISE_SPEED;
        this->setBrightness(UINT8_MAX);
        this->Color1 = COLORS::BLACK;
        this->Color2 = INITIAL_SUNRISE_COLOR;
        this->Fade(this->Color1, this->Color2, SUNRISE_STEPS, this->Interval);
        break;
    case COLOR_SOLID:
        this->currentAnimation = COLOR_SOLID;
        this->NoPattern();
        this->ColorSet(this->Color1);
        break;
    case STROBE_SOLID:
        this->currentAnimation = STROBE_SOLID;
        this->Fade(this->Color1, this->Color2, 1, this->Interval);
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
        this->ColorWipe(random(COLORS::WHITE), this->Interval);
        break;
    case SCANNER_SOLID:
        this->currentAnimation = SCANNER_SOLID;
        this->Scanner(this->Color1, this->Interval, DUAL);
        break;
    case SCANNER_RANDOM:
        this->currentAnimation = SCANNER_RANDOM;
        this->Scanner(random(COLORS::WHITE), this->Interval, DUAL);
        break;
    case FADE_SOLID:
    	this->currentAnimation = FADE_SOLID;
    	this->Fade(this->Color1, this->Color2, 254, this->Interval);
        break;
    case FADE_RANDOM:
    	this->currentAnimation = FADE_RANDOM;
        this->Fade(random(COLORS::WHITE), random(COLORS::WHITE), 254, this->Interval);
        break;
    case FIRE_SOLID:
    	this->currentAnimation = FIRE_SOLID;
        this->Fire(this->Color1, this->Interval);
        break;
    case METEOR_SOLID:
    	this->currentAnimation = METEOR_SOLID;
        this->Meteor(this->Color1, this->Interval);
        break;
    case METEOR_SCANNER_SOLID:
    	this->currentAnimation = METEOR_SCANNER_SOLID;
        this->Meteor(this->Color1, this->Interval);
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
	case SUNRISE:
		if(stick->Color1 == COLORS::BLACK) {
			stick->Color1 = INITIAL_SUNRISE_COLOR;
			stick->Color2 = MID_SUNRISE_COLOR;
		}
		else if(stick->Color1 == INITIAL_SUNRISE_COLOR) {
			stick->Color1 = MID_SUNRISE_COLOR;
			stick->Color2 = END_SUNRISE_COLOR;
		}
		else if(stick->Color1 == MID_SUNRISE_COLOR){
			stick->Color1 = END_SUNRISE_COLOR;
			stick->Color2 = COLORS::WHITE;
		}
		else {
			stick->animationSwitch(STROBE_SOLID);
		}
	    break;
	case COLOR_SOLID:
	  	break;
  	case STROBE_SOLID:
  		if(stick->Color1 > COLORS::BLACK) {
  			stick->Color2 = stick->Color1;
  			stick->Color1 = COLORS::BLACK;
  		}
  		else if(stick->strobeTicks == stick->NUM_STROBE_TICKS) {
  			stick->strobeTicks = 0;
  			stick->Color1 = stick->Color2;
  			stick->Color2 = COLORS::BLACK;
  		}
  		else {
  			stick->strobeTicks++;
  		}
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
		stick->Color1 = random(COLORS::WHITE);
		stick->Color2 = random(COLORS::WHITE);
		break;
	case SCANNER_SOLID:
		// stick->Reverse();
		break;
	case SCANNER_RANDOM:
		stick->Reverse();
		stick->Color1 = random(COLORS::WHITE);
		stick->Color2 = random(COLORS::WHITE);
		break;
	case FADE_SOLID:
		stick->Reverse();
		break;
	case FADE_RANDOM:
		if(stick->Direction == REVERSE) {
			stick->Color2 = random(COLORS::WHITE);
		}
		else {
	    	stick->Color1 = random(COLORS::WHITE);
	  	}
	  	stick->Reverse();
	  	break;
	case METEOR_SCANNER_SOLID:
	  	stick->Reverse();
	  	break;
	default:
      	break;
    }
}