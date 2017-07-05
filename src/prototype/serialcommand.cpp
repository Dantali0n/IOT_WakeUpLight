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

String serialCommand::currentCommand = "";

const char *serialCommand::COMMANDS_STRING[] = {
    "apple", "orange", "grape", "banana", "set_time"
};

void serialCommand::processCommands() {
  bool isComplete = false;
  if (Serial.available()) {
    int i = 0;
    char bytes[64] = {};
    while(Serial.available()) {
       bytes[i] = Serial.read();
       i++;
    }
    String fetched = String(bytes);
    int substr = fetched.indexOf('\n');

    // remove everything after the line ending and the line ending itself
    if(substr > 0) {
      
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

    // Serial.print("Command: ");
    // Serial.println(currentCommand);

    // Command has lineending and is complete and thus ready for processing
    if(isComplete) {
      
      if(currentCommand == COMMANDS_STRING[COMMANDS_ENUM::apple]) {
        Serial.println(COMMANDS_STRING[COMMANDS_ENUM::apple]);
      }
      else if(currentCommand == COMMANDS_STRING[COMMANDS_ENUM::orange]) {
        Serial.println(COMMANDS_STRING[COMMANDS_ENUM::orange]);
      }
      else if(currentCommand == COMMANDS_STRING[COMMANDS_ENUM::grape]) {
        Serial.println(COMMANDS_STRING[COMMANDS_ENUM::grape]);
      }
      else if(currentCommand == COMMANDS_STRING[COMMANDS_ENUM::banana]) {
        Serial.println(COMMANDS_STRING[COMMANDS_ENUM::banana]);
      } 
      else if(currentCommand.startsWith(COMMANDS_STRING[COMMANDS_ENUM::set_time])) {
        Serial.println(COMMANDS_STRING[COMMANDS_ENUM::set_time]);
      }
      else {
        Serial.println("Invalid command!");
      }
      
      currentCommand = "";
    }
  }
}

