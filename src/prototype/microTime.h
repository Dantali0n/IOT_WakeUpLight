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
#include "wulTypedef.h"

/**
 * MicroTime is a small time and date keeping class which can store time in intervals of microseconds.
 * Time is kept at UTC and any timezone translation needs to be done outside of microTime.
 * microTime does have the capability to store a timezone, although India and Nepal timezones are unsupported.
 */
class microTime {
  private:
    static const int MICRO_TO_MILLI_DIV = 1000;

    static const unsigned long CASCADE_MICRO = 1000000;
    static const byte CASCADE_SECOND = 60;
    static const byte CASCADE_MINUTE = 60;
    static const byte CASCADE_HOUR = 24;
    static const byte CASCADE_DAY_FS = 28;
    static const byte CASCADE_DAY_FL = 29;
    static const byte CASCADE_DAY_S = 30;
    static const byte CASCADE_DAY_L = 31;
    static const byte CASCADE_MONTH = 12;
    
  	bool timeSet;
    char timeZone; // does not work for Nepal or India which have a timezone of + 5.75 and 5.50 #sorry

  	unsigned int years;
  	byte months;
  	byte days;
  	byte hours;
  	byte minutes;
  	byte seconds;
  	unsigned long microSeconds;

    static byte getCascadeDay(unsigned int years, byte months);
  
    void cascadeMicroSecond();
    void cascadeSecond();
    void cascadeMinute();
    void cascadeHour();
    void cascadeDay();
    void cascadeMonth();
  public:
  	microTime();
    microTime(unsigned int year, byte month, byte day, byte hour, byte minute, byte second);
    microTime(unsigned int year, byte month, byte day, byte hour, byte minute, byte second, unsigned long microSecond);

    bool operator<(microTime& rhs);
    bool operator>(microTime& rhs);
    bool operator<=(microTime& rhs);
    bool operator>=(microTime& rhs);
    bool operator==(microTime& rhs);
    bool operator!=(microTime& rhs);
    void operator+=(microTime& rhs);

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
