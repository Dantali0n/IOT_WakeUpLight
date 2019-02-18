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

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "neopatterns.h"

bool callbackCalled = false;

void emptyPatternComplete(NeoPatterns* stick) {

};

void measurePatternComplete(NeoPatterns* stick) {
	callbackCalled = true;
};

TEST_CASE( "NeoPatterns init", "[NeoPatterns]" ) {
	NeoPatterns pattern(30, 8, NEO_RGB, emptyPatternComplete);
	REQUIRE( sizeof(pattern) > 0);
}

TEST_CASE( "NeoPatterns pattern updates and callback", "[NeoPatterns]" ) {
	NeoPatterns pattern(30, 8, NEO_RGB, measurePatternComplete);
	pattern.RainbowCycle(5);
	for(uint16_t i = 0; i < pattern.TotalSteps; i++) {
		pattern.update(pattern.Interval * TIME::MILLIS_TO_MICROS);
	}
	REQUIRE( callbackCalled == true);
	callbackCalled = false;
}