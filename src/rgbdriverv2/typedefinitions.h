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

#ifndef TYPEDEF_DEFINITIONS_H
#ifdef __cplusplus
#define TYPEDEF_DEFINITIONS_H

// TODO: Change into enum as mix and matching produces undesirable results.
static const bool DEBUG = false; // output debugging information
static const bool COMPUTER_SERIAL = false; // serial output is for esp8266
static const bool PERFORMANCE_PROFILE = false; // output performance / timing information statistics

/* Colors */
namespace COLORS {
	static const uint32_t WHITE = 16777215;
	static const uint32_t BLACK = 0;
}

namespace TIME {
	static const uint16_t MILLIS_TO_MICROS = 1000;
}

typedef unsigned char uint8_t;

#endif /* __cplusplus */
#endif /* TYPEDEF_DEFINITIONS_H */