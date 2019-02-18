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

#include "framerate.h"

TEST_CASE( "FrameRate init", "[FrameRate]" ) {
	FrameRate r;
	r.update(0);
	REQUIRE( r.getFrames() == 1);
}

TEST_CASE( "FrameRate indeterminable framerate", "[FrameRate]" ) {
	FrameRate r;
	r.update(0);
	REQUIRE( r.get() == 0);
}

TEST_CASE( "FrameRate indeterminable framerate average", "[FrameRate]" ) {
	FrameRate r;
	r.update(0);
	REQUIRE( r.getAverage() == 0);
}

TEST_CASE( "FrameRate update small", "[FrameRate]" ) {
	FrameRate r;
	r.update(TIME::MICROS_TO_SECONDS);
	REQUIRE( r.get() == 1);
}

TEST_CASE( "FrameRate update large", "[FrameRate]" ) {
	FrameRate r;
	r.update(167); // to get 6000 needs to be 166.667 micros
	REQUIRE( r.get() == Approx(6000).margin(20));
}

TEST_CASE( "FrameRate reset", "[FrameRate]" ) {
	FrameRate r;
	r.update(0);
	r.reset();
	REQUIRE( r.getFrames() == 0);
}

TEST_CASE( "FrameRate frame counting", "[FrameRate]" ) {
	FrameRate r;
	r.update(0);
	r.update(0);
	r.update(0);
	REQUIRE( r.getFrames() == 3);
}

TEST_CASE( "FrameRate frame average", "[FrameRate]" ) {
	FrameRate r;
	r.update(16666);
	r.update(16666);
	r.update(16667);
	r.update(16667);
	REQUIRE( r.getAverage() == 60);
}

TEST_CASE( "FrameRate frame average reset", "[FrameRate]" ) {
	FrameRate r;
	r.update(TIME::MICROS_TO_MINUTES);
	REQUIRE( r.getAverage() == 0);
	REQUIRE( r.get() == 0);
	REQUIRE( r.getFrames() == 0);
}

TEST_CASE( "FrameRate assert secondly update", "[FrameRate]" ) {
	FrameRate r;
	r.update(200000);
	r.update(200000);
	r.update(200000);
	r.update(200000);
	REQUIRE( (r.getFrames() >= r.get()) == false);
	r.update(200000);
	REQUIRE( (r.getFrames() >= r.get()) == true);
}