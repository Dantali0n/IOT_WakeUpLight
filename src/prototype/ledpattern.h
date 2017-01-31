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

#ifndef _Led_Pattern_h
#ifdef __cplusplus
#define _Led_Pattern_h

#include <Arduino.h>
#include "wulTypedef.h"
#include "rgbColor.h"

/**
 * Statefull lighting pattern for led strip.
 * Supports incremental state updates using deltaTime in microseconds
 * @TODO add enumerator for different modes
 */
class ledPattern {
  	private:
  		unsigned int currentDuration;
  		unsigned int finalDuration;
  		rgbColor currentColor;
  		rgbColor finalColor;
	public:
		ledPattern(rgbColor startColor, rgbColor endColor, unsigned int duration);
		void update(unsigned int deltaTime);
		rgbColor getColor();
};

#endif /* __cplusplus */
#endif /* _Led_Pattern_h */