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
    "brightness", "speed", "pattern", "color", "direction", "pwm"
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
    Serial.print("processCommands - have serial - ");
    int i = 0;
    char bytes[64] = {};
    while(Serial.available()) {
      Serial.print("b");
      bytes[i] = Serial.read();
      i++;
    }
    String fetched = String(bytes);
    int substr = fetched.indexOf('\n');

    // remove everything after the line ending and the line ending itself
    if(substr > 0) {
      Serial.print(" - have line ending");
      
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
    if(DEBUG) Serial.print(" ");
    if(DEBUG) Serial.println(currentCommand);

    // Serial.print("Command: ");
    // Serial.println(currentCommand);

    // Command has line ending and is complete and thus ready for processing
    if(isComplete) {

      if(currentCommand.startsWith(COMMANDS_STRING[COMMANDS_ENUM::brightness])) {
        currentCommand = currentCommand.substring(String(COMMANDS_STRING[COMMANDS_ENUM::brightness]).length() +1); 
        processSetBrightness();
      }
      else if(currentCommand.startsWith(COMMANDS_STRING[COMMANDS_ENUM::speed])) {
        currentCommand = currentCommand.substring(String(COMMANDS_STRING[COMMANDS_ENUM::speed]).length() +1);
        processSetSpeed();
        // bool newFlicker = currentCommand.substring(0,1).equals("1");
        // Serial.print(COMMANDS_STRING[COMMANDS_ENUM::speed]);
        // Serial.print(": ");
        // Serial.println(newFlicker);
        
        // eventHandler->eventSetMinuteFlicker(newFlicker);
      }
      else if(currentCommand.startsWith(COMMANDS_STRING[COMMANDS_ENUM::pattern])) {
        currentCommand = currentCommand.substring(String(COMMANDS_STRING[COMMANDS_ENUM::pattern]).length() +1);
        processSetAnimation();

        // int newFlicker = currentCommand.toInt();
        // Serial.print(COMMANDS_STRING[COMMANDS_ENUM::pattern]);
        // Serial.print(": ");
        // Serial.println(newFlicker);
        // eventHandler->eventSetNumMinuteFlicker(newFlicker);
      }
      else if(currentCommand.startsWith(COMMANDS_STRING[COMMANDS_ENUM::color])) {
        currentCommand = currentCommand.substring(String(COMMANDS_STRING[COMMANDS_ENUM::color]).length() +1);
        processSetColor();

        // bool newRunningPatterns = currentCommand.substring(0,1).equals("1");
        // Serial.print(COMMANDS_STRING[COMMANDS_ENUM::color]);
        // Serial.print(": ");
        // Serial.println(newRunningPatterns);
        // eventHandler->eventSetRunningPatterns(newRunningPatterns);
      }
      else if(currentCommand.startsWith(COMMANDS_STRING[COMMANDS_ENUM::direction])) {
        currentCommand = currentCommand.substring(String(COMMANDS_STRING[COMMANDS_ENUM::direction]).length() +1);
        processSetDirection();

        // bool newRunningPatterns = currentCommand.substring(0,1).equals("1");
        // Serial.print(COMMANDS_STRING[COMMANDS_ENUM::direction]);
        // Serial.print(": ");
        // Serial.println(newRunningPatterns);
        // eventHandler->eventSetRunningPatterns(newRunningPatterns);
      }
      else if(currentCommand.startsWith(COMMANDS_STRING[COMMANDS_ENUM::pwm])) {
        currentCommand = currentCommand.substring(String(COMMANDS_STRING[COMMANDS_ENUM::pwm]).length() +1);
        processSetPWM();

        // bool newLeds = currentCommand.substring(0,1).equals("1");
        // Serial.print(COMMANDS_STRING[COMMANDS_ENUM::pwm]);
        // Serial.print(": ");
        // Serial.println(newLeds);
        // eventHandler->eventSetLeds(newLeds);
      }
      else {
        Serial.println(currentCommand);
        Serial.println("Invalid command!");
      }
      
      currentCommand = "";
    }
  }
}

void serialCommand::processSetBrightness() {
  long brightness = currentCommand.toInt();
  if(brightness > UINT8_MAX) brightness = UINT8_MAX;
  eventHandler->eventSetBrightness(brightness);
}

void serialCommand::processSetSpeed() {
  long speed = currentCommand.toInt();
  if(speed > UINT16_MAX) speed = UINT16_MAX;
  eventHandler->eventSetSpeed(speed);
}

void serialCommand::processSetAnimation() {
  animation anim = NO_PATTERN_UPDATE;

  if(currentCommand.startsWith(LED_ANIMATION_STRING[animation::RAINBOW])) {
    anim = RAINBOW;
  }
  else if(currentCommand.startsWith(LED_ANIMATION_STRING[animation::COLOR_WHITE])) {
    anim = COLOR_WHITE;
  }
  else if(currentCommand.startsWith(LED_ANIMATION_STRING[animation::COLOR_WIPE_CHRISTMAS])) {
    anim = COLOR_WIPE_CHRISTMAS;
  }
  else if(currentCommand.startsWith(LED_ANIMATION_STRING[animation::COLOR_WIPE_RANDOM])) {
    anim = COLOR_WIPE_RANDOM;
  }
  else if(currentCommand.startsWith(LED_ANIMATION_STRING[animation::SCANNER_WHITE])) {
    anim = SCANNER_WHITE;
  }
  else if(currentCommand.startsWith(LED_ANIMATION_STRING[animation::SCANNER_RANDOM])) {
    anim = SCANNER_RANDOM;
  }
  
  eventHandler->eventSetAnimation(anim);
}

void serialCommand::processSetColor() {
  long color = currentCommand.toInt();
  if(color > UINT32_MAX) color = UINT32_MAX;
  eventHandler->eventSetColor(color);
}

void serialCommand::processSetDirection() {

  eventHandler->eventSetDirection(true);
}

void serialCommand::processSetPWM() {
  long pwm = currentCommand.toInt();
  if(pwm > 255) pwm = 255;
  eventHandler->eventSetPWM(pwm);
}
