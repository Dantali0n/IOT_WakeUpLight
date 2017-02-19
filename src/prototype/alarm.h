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

#ifndef _Alarm_h
#ifdef __cplusplus
#define _Alarm_h

// #include <Arduino.h> // needed for PROGMEM support -> if applicable to microcontroller architecture.
#include "wulTypedef.h"
#include "ledpattern.h"
#include "microTime.h"

/**
 * Alarms are predefined events which trigger when a specific moment in time has passed.
 * They can be given an interval for repetition or be a one time event.
 * Alarms come with configurable behaviour for instance the pattern of light which is shown when active.
 * Only one alarm can be active at a time and alarms do not stack. 
 * If a new alarm gets activated while one is running the current alarm gets overwritten with the new one.
 */
class alarm {
	private:
    ledPattern *alarmPattern; // @TODO not store internal ledPattern as pointer since alarm should be owner of ledPattern object
		microTime trigger;
		microTime interval;
		microTime lastCheck;
		bool hasInterval;
	public:
	  alarm(microTime trigger, ledPattern &alarmPattern);
		alarm(microTime trigger, microTime interval, ledPattern &alarmPattern);
		bool check(microTime *current);
    ledPattern * getAlarmPattern();

};

#endif /* __cplusplus */
#endif /* _Alarm_h */
