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

#ifndef _RGBrl_Typedef_h
#define _RGBrl_Typedef_h

#include <list>
#include <iostream>
#include <random>
#include <functional>
#include <math.h>
#include <stdint.h>

typedef unsigned char byte;

template<class T>
const T& constrain(const T& x, const T& a, const T& b) {
    if(x < a) {
        return a;
    }
    else if(b < x) {
        return b;
    }
    else
        return x;
}

//std::random_device rd;
//std::mt19937::result_type seed = rd();
//auto colorRand = std::bind(std::uniform_int_distribution<int>(0, 256), std::mt19937(seed));
//auto timeRand = std::bind(std::uniform_int_distribution<int>(1000, 10000), std::mt19937(seed));

//namespace rgbrlrand {
//    
//}

//extern static const std::random_device rd; // obtain a random number from hardware
//extern static const std::mt19937 eng; // seed the generator
//extern static const std::uniform_int_distribution<> colorRand;
//extern static const std::uniform_int_distribution<> timeRand;

// used to determine if debug info should be outputted to serial
// Probably should be done with ifdef instead so we can patch it out compile time and not waste CPU cycles + open an attack vector
static const bool RGBRL_DEBUG = false; 

#endif /* _RGBrl_Typedef_h */

