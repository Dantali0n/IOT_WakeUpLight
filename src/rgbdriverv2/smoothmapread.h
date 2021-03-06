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

#ifndef SMOOTH_MAP_READ_H
#ifdef __cplusplus
#define SMOOTH_MAP_READ_H

#include <Arduino.h>
#include "typedefinitions.h"

class SmoothMapRead {
public:
	int input;
	uint16_t lowDeathzone = 20;
	uint16_t highDeathzone = 1000;
	uint16_t lowOutput = 0;
	uint16_t highOutput = 255;
	uint8_t numSamples = 5;

	SmoothMapRead(int input);

	uint16_t read();
	uint16_t read(uint8_t numSamples);
};

#endif /* __cplusplus */
#endif /* SMOOTH_MAP_READ_H */