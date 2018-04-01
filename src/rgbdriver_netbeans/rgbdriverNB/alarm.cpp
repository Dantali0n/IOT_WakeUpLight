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

#include "alarm.h"

/**
 * 
 */
alarm::alarm(microTime trigger, ledPattern &alarmPattern) {
    this->alarmPattern = &alarmPattern;
    this->trigger = trigger;
    this->interval = 0;
    this->hasInterval = false;
}

/**
 * 
 */
alarm::alarm(microTime trigger, unsigned long interval, ledPattern &alarmPattern) {
    this->alarmPattern = &alarmPattern;
    this->trigger = trigger;
    this->interval = interval;
    this->hasInterval = true;
}

/**
 *
 */
bool alarm::check(microTime &current) {
    if(this->trigger < current) {
        if(this->hasInterval) {
            std::cout << "Add interval to time" << std::endl;
            this->trigger = microTime(current.year(), current.month(), current.day(), current.hour(), current.minute(), current.second());
            this->trigger += this->interval;
        }
        return true;
    }
    else {
        return false;
    }
}

/**
 * retrieves an pointer to the ledPattern that should be activated when the alarm triggers
 * @Return ledPattern pointer
 * @TODO maybe should not be pointer, better parse by value and make sure new ledPattern is copy of internal ledPattern pointer content
 */
ledPattern * alarm::getAlarmPattern() {
  return alarmPattern;
}

/**
 * retrieves an pointer to the ledPattern that should be activated when the alarm triggers
 * @Return ledPattern pointer
 * @TODO maybe should not be pointer, better parse by value and make sure new ledPattern is copy of internal ledPattern pointer content
 */
microTime * alarm::getTrigger() {
  return &trigger;
}

/**
 * 
 */
bool alarm::getInterval() {
  return hasInterval;
}
