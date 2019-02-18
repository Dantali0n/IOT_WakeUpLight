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

#ifndef ARDUINO_H
#ifdef __cplusplus
#define ARDUINO_H

#include <stdint.h>
#include <math.h>
#include <string>

typedef bool boolean;
typedef unsigned char byte;

uint32_t micros();

inline uint64_t random(uint64_t limit) {
	return 0;
};

inline uint64_t random(uint64_t lowerLimit, uint64_t uppperLimit) {
	return 0;
};

// Ohboi this is gonna be fun.... 
class String : public std::string {
	public:
		String(const char* input);
};

class serial {
	public:
		serial();
		void begin(uint32_t baudRate);

		void print(String text);
		void println(String text);

		void print(float& text);
		void println(float& text);
};

extern serial Serial;

#undef GLIBC_INSIDE

#endif /* __cplusplus */
#endif /* ARDUINO_H */