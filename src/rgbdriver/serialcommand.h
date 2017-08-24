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


#ifndef _Serial_Command_h
#ifdef __cplusplus
#define _Serial_Command_h

#include <Arduino.h>
#include "rgbrltypedef.h"
#include "microtime.h"

/**
 * Abstract delegate to allow outside of serialCommand processing of events
 */
class serialCommandDelegate {
  public:
    virtual void eventSetTime(microTime newTime) = 0;
    virtual void eventSetLeds(bool on) = 0;
    virtual void eventSetRunningPatterns(bool newRunningPatterns) = 0;
    virtual void eventSetMinuteFlicker(bool newFlicker) = 0;
    virtual void eventSetNumMinuteFlicker(int newNumFlicker) = 0;
};

class serialCommand {
  private:
    serialCommandDelegate *eventHandler;
    String currentCommand;
    void processSetTime();
  public:
    enum COMMANDS_ENUM {
      set_time, running_patterns, minute_flicker, num_minute_flicker, leds
    };
    
    static const char *COMMANDS_STRING[];

    serialCommand(serialCommandDelegate *eventHandler);
    
    void processCommands();
};

#endif /* __cplusplus */
#endif /* _Serial_Command_h */


