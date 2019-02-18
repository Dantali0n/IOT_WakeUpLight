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

#include "deltatimer.h"

TEST_CASE( "DeltaTimer init", "[DeltaTimer]" ) {
	DeltaTimer d;
	d.update(0);
	REQUIRE(  d.getDeltaTime() == 0);
}

TEST_CASE( "DeltaTimer progress", "[DeltaTimer]" ) {
	DeltaTimer d;
	d.update(512);
	REQUIRE(  d.getDeltaTime() == 512);
}

TEST_CASE( "DeltaTimer max", "[DeltaTimer]" ) {
	DeltaTimer d;
	d.update(UINT32_MAX);
	REQUIRE(  d.getDeltaTime() == UINT32_MAX);
}

TEST_CASE( "DeltaTimer overflow", "[DeltaTimer]" ) {
	DeltaTimer d;
	d.update(UINT32_MAX);
	d.update(1);
	REQUIRE(  d.getDeltaTime() == 0);
}