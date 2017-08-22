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

#include "microtime.h"

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
 * @Param years full 4 digit year clamped between 0 and 9999
 * @Param months clamped between 1 and 12
 * @Param days clamped between 1 and 31
 * @Param hours clamped between 0 and 23
 * @Param minutes clamped between 0 and 59
 * @Param seconds clamped between 0 and 59
 */
microTime::microTime(unsigned int years, byte months, byte days, byte hours, byte minutes, byte seconds) {
  this->timeZone = 0;
  this->years = constrain(years, 1970, 9999);
  this->months = constrain(months, 1, 12);
  
  byte currentCascadeDay = getCascadeDay(this->years, this->months); // only works if year and month are available hence it is not on top of this method.
  this->days = constrain(days, 1, currentCascadeDay);
  this->hours = constrain(hours, 0 , CASCADE_HOUR -1);
  this->minutes = constrain(minutes, 0 , CASCADE_MINUTE -1);
  this->seconds = constrain(seconds, 0 , CASCADE_MINUTE -1);
  this->microSeconds = 0;
  this->timeSet = true;
}

/**
 * Initiate microTime with the provided date and time, will set isSet to true
 * @Param years full 4 digit year clamped between 0 and 9999
 * @Param months clamped between 1 and 12
 * @Param days clamped between 1 and 31
 * @Param hours clamped between 0 and 23
 * @Param minutes clamped between 0 and 59
 * @Param seconds clamped between 0 and 59
 * @Param microSeconds clamped between 0 and 999999
 */
microTime::microTime(unsigned int years, byte months, byte days, byte hours, byte minutes, byte seconds, unsigned long microSeconds) {
	this->timeZone = 0;
	this->years = constrain(years, 1970, 9999);
	this->months = constrain(months, 1, 12);
  
  byte currentCascadeDay = getCascadeDay(this->years, this->months); // only works if year and month are available hence it is not on top of this method.
	this->days = constrain(days, 1, currentCascadeDay);
	this->hours = constrain(hours, 0 , CASCADE_HOUR -1);
  this->minutes = constrain(minutes, 0 , CASCADE_MINUTE -1);
	this->seconds = constrain(seconds, 0 , CASCADE_SECOND -1);
	this->microSeconds = constrain(microSeconds, 0, CASCADE_MICRO -1);
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

/**
 * 
 */
bool microTime::operator< (microTime& rhs){ 
  if(this->year() > rhs.year()) return false;
  if(this->month() > rhs.month()) return false;
  if(this->day() > rhs.day()) return false;
  if(this->hour() > rhs.hour()) return false;
  if(this->minute() > rhs.minute()) return false;
  if(this->second() > rhs.second()) return false;
  if(this->microSecond() > rhs.microSecond()) return false;
  return true;
}

/**
 * 
 */
bool microTime::operator> (microTime& rhs) { 
  return rhs < *this; 
}

/**
 * 
 */
bool microTime::operator<=(microTime& rhs) { 
  return !(*this > rhs); 
}

/**
 * 
 */
bool microTime::operator>=(microTime& rhs) { 
  return !(*this < rhs); 
}

/**
 * 
 */
bool microTime::operator==(microTime& rhs){ 
  if(this->year() != rhs.year()) return false;
  if(this->month() != rhs.month()) return false;
  if(this->day() != rhs.day()) return false;
  if(this->hour() != rhs.hour()) return false;
  if(this->minute() != rhs.minute()) return false;
  if(this->second() != rhs.second()) return false;
  if(this->microSecond() != rhs.microSecond()) return false;
  return true;
}

/**
 * 
 */
bool microTime::operator!=(microTime& rhs){ 
  return !(*this == rhs);
}

void microTime::operator+=(unsigned long seconds) {
  this->seconds += seconds;
  cascadeSecond();
  cascadeMinute();
  cascadeHour();
  cascadeDay();
  cascadeMonth();
}

/**
 * 
 * @UNTESTED
 * @Warning this can never properly work because of the clamping inside microtime
 */
void microTime::operator+=(microTime& rhs){
  this->years += rhs.year();
  this->months += rhs.month();
  this->days += rhs.day();
  this->hours += rhs.hour();
  this->minutes += rhs.minute();
  this->seconds += rhs.second();
  this->microSeconds += rhs.microSecond();

  cascadeMicroSecond();
  cascadeSecond();
  cascadeMinute();
  cascadeHour();
  cascadeDay();
  cascadeMonth();
}

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
 * Determine the current point of cascading for the number of days
 * @Param years the year to determine cascading for
 * @Param months the month to determine cascading for
 */
byte microTime::getCascadeDay(unsigned int years, byte months) {
  if(months == 2) { // februari is special
    // every year which can be divided by four is a year with a extra day
    if(years % 4 == 0 && years % 100 != 0 && years % 400 == 0) { 
      return CASCADE_DAY_FL;
    }
    else {
      return CASCADE_DAY_FS;  
    }
  }
  else if(months % 2 == 0) {
    return CASCADE_DAY_S;  
  }
  else {
    return CASCADE_DAY_L;
  }
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
  byte currentCascade = getCascadeDay(this->years, this->months);
  
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


