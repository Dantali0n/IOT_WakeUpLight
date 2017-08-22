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

#include "serialcommand.h"

const char *serialCommand::COMMANDS_STRING[] = {
    "set_time", "running_patterns", "minute_flicker", "num_minute_flicker", "leds"
};

/**
 * Assign instance of delegate upon object construction, do not allow serialCommand to exist without a delegate object
 */
serialCommand::serialCommand(serialCommandDelegate *eventHandler) {
  currentCommand = "";
  this->eventHandler = eventHandler;
}

void serialCommand::processCommands() {
  bool isComplete = false;
  if (Serial.available()) {
    if(WUL_DEBUG) Serial.print("processCommands - have serial - ");
    int i = 0;
    char bytes[64] = {};
    while(Serial.available()) {
       if(WUL_DEBUG) Serial.print("b");
       bytes[i] = Serial.read();
       i++;
    }
    String fetched = String(bytes);
    int substr = fetched.indexOf('\n');

    // remove everything after the line ending and the line ending itself
    if(substr > 0) {
      if(WUL_DEBUG) Serial.print(" - have line ending");
      
      // Serial.print("Before: ");
      // Serial.println(bytes);
      
      for(int i = substr; i < 64; i++) {
        bytes[i] = NULL;
      }
      
      // Serial.print("after: ");
      // Serial.println(bytes);
      
      isComplete = true;
    }

    currentCommand += String(bytes);
    if(WUL_DEBUG) Serial.print(" ");
    if(WUL_DEBUG) Serial.println(currentCommand);

    // Serial.print("Command: ");
    // Serial.println(currentCommand);

    // Command has lineending and is complete and thus ready for processing
    if(isComplete) {
      
      if(currentCommand.startsWith(COMMANDS_STRING[COMMANDS_ENUM::set_time])) {
        Serial.println(COMMANDS_STRING[COMMANDS_ENUM::set_time]);

        // the residual command is whats left after the first space character
        currentCommand = currentCommand.substring(String(COMMANDS_STRING[COMMANDS_ENUM::set_time]).length() +1); 
        Serial.println(currentCommand);
        processSetTime();
      }
      else if(currentCommand.startsWith(COMMANDS_STRING[COMMANDS_ENUM::minute_flicker])) {
        currentCommand = currentCommand.substring(String(COMMANDS_STRING[COMMANDS_ENUM::minute_flicker]).length() +1);
        bool newFlicker = currentCommand.substring(0,1).equals("1");
        
        Serial.print(COMMANDS_STRING[COMMANDS_ENUM::minute_flicker]);
        Serial.print(": ");
        Serial.println(newFlicker);
        
        eventHandler->eventSetMinuteFlicker(newFlicker);
      }
      else if(currentCommand.startsWith(COMMANDS_STRING[COMMANDS_ENUM::num_minute_flicker])) {
        currentCommand = currentCommand.substring(String(COMMANDS_STRING[COMMANDS_ENUM::num_minute_flicker]).length() +1);
        int newFlicker = currentCommand.toInt();
        
        Serial.print(COMMANDS_STRING[COMMANDS_ENUM::num_minute_flicker]);
        Serial.print(": ");
        Serial.println(newFlicker);
        
        eventHandler->eventSetNumMinuteFlicker(newFlicker);
      }
      else if(currentCommand.startsWith(COMMANDS_STRING[COMMANDS_ENUM::running_patterns])) {
        currentCommand = currentCommand.substring(String(COMMANDS_STRING[COMMANDS_ENUM::running_patterns]).length() +1);
        bool newRunningPatterns = currentCommand.substring(0,1).equals("1");
        
        Serial.print(COMMANDS_STRING[COMMANDS_ENUM::running_patterns]);
        Serial.print(": ");
        Serial.println(newRunningPatterns);
        
        eventHandler->eventSetRunningPatterns(newRunningPatterns);
      }
      else if(currentCommand.startsWith(COMMANDS_STRING[COMMANDS_ENUM::leds])) {
        currentCommand = currentCommand.substring(String(COMMANDS_STRING[COMMANDS_ENUM::leds]).length() +1);
        bool newLeds = currentCommand.substring(0,1).equals("1");
        
        Serial.print(COMMANDS_STRING[COMMANDS_ENUM::leds]);
        Serial.print(": ");
        Serial.println(newLeds);
        
        eventHandler->eventSetLeds(newLeds);
      }
      else {
        Serial.println(currentCommand);
        Serial.println("Invalid command!");
      }
      
      currentCommand = "";
    }
  }
}

void serialCommand::processSetTime() {
  unsigned int year = (unsigned int) currentCommand.substring(0,4).toInt();
  Serial.print("year: ");
  Serial.println(year);
  
  byte month = (byte)currentCommand.substring(5,7).toInt();
  Serial.print("month: ");
  Serial.println(month);
  
  byte day = (byte)currentCommand.substring(8,10).toInt();
  Serial.print("day: ");
  Serial.println(day);
  
  byte hours = (byte)currentCommand.substring(11,13).toInt();
  Serial.print("hours: ");
  Serial.println(hours);
  
  byte minutes = (byte)currentCommand.substring(14,16).toInt();
  Serial.print("minutes: ");
  Serial.println(minutes);
  
  byte seconds = (byte)currentCommand.substring(17,19).toInt();
  Serial.print("seconds: ");
  Serial.println(seconds);
  
  eventHandler->eventSetTime(microTime(year, month, day, hours, minutes, seconds));
}



