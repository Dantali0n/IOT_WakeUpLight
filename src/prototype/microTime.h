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

#ifndef _Micro_Time_h
#ifdef __cplusplus
#define _Micro_Time_h

#include <Arduino.h> // needed for PROGMEM support -> if applicable to microcontroller architecture.
#include "timeLib.h"

typedef unsigned char byte;

class microTime {
  private:
    static const int MICRO_TO_MILLI_DIV PROGMEM = 1000;

    static const unsigned long CASCADE_MICRO PROGMEM = 1000000;
    static const byte CASCADE_SECOND PROGMEM = 60;
    static const byte CASCADE_MINUTE PROGMEM = 60;
    static const byte CASCADE_HOUR PROGMEM = 24;
    static const byte CASCADE_DAY_FS PROGMEM = 28;
    static const byte CASCADE_DAY_FL PROGMEM = 29;
    static const byte CASCADE_DAY_S PROGMEM = 30;
    static const byte CASCADE_DAY_L PROGMEM = 31;
    static const byte CASCADE_MONTH PROGMEM = 12;
    
  	bool timeSet;
    char timeZone; // does not work for Nepal or India which have a timezone of + 5.75 and 5.50 #sorry

  	unsigned int years;
  	byte months;
  	byte days;
  	byte hours;
  	byte minutes;
  	byte seconds;
  	unsigned long microSeconds;

    void cascadeMicroSecond();
    void cascadeSecond();
    void cascadeMinute();
    void cascadeHour();
    void cascadeDay();
    void cascadeMonth();
  public:
  	microTime();
    microTime(unsigned int year, byte month, byte day, byte hour, byte second, unsigned long microSecond);

  	void update(unsigned long additionalMicros);
  	bool isTimeSet();
    char getTimeZone();
    void setTimeZone(char timeZone);

  	unsigned int year();
  	byte month();
  	byte day();
  	byte hour();
  	byte minute();
  	byte second();
  	unsigned int milliSecond();
  	unsigned long microSecond();
};

#endif /* __cplusplus */
#endif /* _Micro_Time_h */
