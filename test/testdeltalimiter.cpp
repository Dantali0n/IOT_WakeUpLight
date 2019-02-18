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

#include "deltalimiter.h"

TEST_CASE( "DeltaLimiter limit update", "[DeltaLimiter]" ) {
	DeltaLimiter d(512);
	REQUIRE( d.hasUpdate() == false);
}

TEST_CASE( "DeltaLimiter should have an update", "[DeltaLimiter]" ) {
	DeltaLimiter d(1);
	d.update(1);
	REQUIRE( d.hasUpdate() == true);
}

TEST_CASE( "DeltaLimiter update once", "[DeltaLimiter]" ) {
	DeltaLimiter d(512);
	d.update(512);
	REQUIRE( d.hasUpdate() == true);
	REQUIRE( d.hasUpdate() == false);
}

TEST_CASE( "DeltaLimiter overflow", "[DeltaLimiter]" ) {
	DeltaLimiter d(65535);
	d.update(65536);
	REQUIRE( d.hasUpdate() == true);
}