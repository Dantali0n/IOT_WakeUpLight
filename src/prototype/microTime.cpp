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

#include "microTime.h"

/**
 * Initiate microTime internal values but isSet stays false
 */
microTime::microTime() {
	  timeZone = 0;
  	years = 0;
  	months = 1;
  	days = 1;
  	hours = 0;
  	minutes = 0;
  	seconds = 0;
  	microSeconds = 0;
  	timeSet = false;
}

/**
 * Initiate microTime with the provided date and time, will set isSet to true
 * @Param year full 4 digit year clamped between 0 and 9999
 * @Param month clamped between 1 and 12
 * @Param day clamped between 1 and 31
 * @Param hour clamped between 0 and 23
 * @Param second clamped between 0 and 59
 * @Param microSecond clamped between 0 and 999999
 */
microTime::microTime(unsigned int years, byte months, byte days, byte hours, byte seconds, unsigned long microSeconds) {
	this->timeZone = 0;
	this->years = years;
	this->months = months;
	this->days = days;
	this->hours = hours;
	this->seconds = seconds;
	this->microSeconds = microSeconds;
	this->timeSet = true;
}

/**
 * Increments the current date and time with the specified micro seconds, will cascade to other variables
 * @Param additionalMicros microseconds passed since last update
 */
void microTime::update(unsigned long additionalMicros) {
  this->microSeconds += additionalMicros;
  cascadeMicroSecond();
  cascadeSecond();
  cascadeMinute();
  cascadeHour();
  cascadeDay();
  cascadeMonth();
}

// bool microTime::operator<(const microTime& rhs) const {
//     return true;
// }

/**
 *
 */
// inline bool microTime::operator< (const microTime& lhs, const microTime& rhs) { 
//   return true;
// }

/**
 *
 */
// inline bool microTime::operator> (const microTime& lhs, const microTime& rhs) { 
//   return  operator< (rhs,lhs);
// }

/**
 *
 */
unsigned int microTime::year() {
  return years;
}

/**
 *
 */
byte microTime::month() {
  return months;
}

/**
 *
 */
byte microTime::day() {
  return days;
}

/**
 *
 */
byte microTime::hour() {
  return hours;
}

/**
 *
 */
byte microTime::minute() {
  return minutes;
}

/**
 *
 */
byte microTime::second() {
  return seconds;
}

/**
 *
 */
unsigned int microTime::milliSecond() {
  return microSeconds / MICRO_TO_MILLI_DIV;
}

/**
 *
 */
unsigned long microTime::microSecond() {
  return microSeconds;
}

/**
 *
 */
bool microTime::isTimeSet() {
  return timeSet;
}

/**
 *
 */
char microTime::getTimeZone() {
  return timeZone;
}

/**
 *
 */
void microTime::setTimeZone(char timeZone) {
  this->timeZone = constrain(timeZone, -12, 14);
}

/**
 *
 */
void microTime::cascadeMicroSecond() {
  if(microSeconds >= CASCADE_MICRO) {
    int increment = microSeconds / CASCADE_MICRO;
    seconds += increment;
    microSeconds -= increment * CASCADE_MICRO;
  }
}

/**
 *
 */
void microTime::cascadeSecond() {
  if(seconds >= CASCADE_SECOND) {
    int increment = seconds / CASCADE_SECOND;
    minutes += increment;
    seconds -= increment * CASCADE_SECOND;
  }
}

/**
 *
 */
void microTime::cascadeMinute() {
  if(minutes >= CASCADE_MINUTE) {
    int increment = minutes / CASCADE_MINUTE;
    hours += increment;
    minutes -= increment * CASCADE_MINUTE;
  }
}

/**
 *
 */
void microTime::cascadeHour() {
  if(hours >= CASCADE_HOUR) {
    int increment = hours / CASCADE_HOUR;
    days += increment;
    hours -= increment * CASCADE_HOUR;
  }  
}

/**
 *
 */
void microTime::cascadeDay() {
  byte currentCascade = 31;

  if(months == 2) { // februari is special
    // every year which can be divided by four is a year with a extra day
    if(years % 4 == 0 && years % 100 != 0 && years % 400 == 0) { 
      currentCascade = CASCADE_DAY_FL;
    }
    else {
      currentCascade = CASCADE_DAY_FS;  
    }
  }
  else if(months % 2 == 0) {
    currentCascade = CASCADE_DAY_S;  
  }
  else {
    currentCascade = CASCADE_DAY_L;
  }
  
  if(days >= currentCascade) {
    int increment = days / currentCascade;
    months += increment;
    days -= increment * currentCascade;
  }    
}

/**
 *
 */
void microTime::cascadeMonth() {
  if(months >= CASCADE_MONTH) {
    int increment = months / CASCADE_MONTH;
    years += increment;
    months -= increment * CASCADE_MONTH;
  } 
}

