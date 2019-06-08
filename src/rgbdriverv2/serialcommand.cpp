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

const char *SerialCommand::COMMANDS_STRING[] = {
    "brightness", "speed", "pattern", "color", "path", "pwm"
};

/**
 * Assign instance of delegate upon object construction, do not allow SerialCommand to exist without a delegate object
 */
SerialCommand::SerialCommand(SerialCommandDelegate *eventHandler) 
  : serialIn("") 
{
  this->eventHandler = eventHandler;
}

void SerialCommand::processCommands() {
  bool isComplete = false; // Determine if the received command is complete

  // Buffer input and determine if command is complete
  if (Serial.available()) {
    if(DEBUG_MODE == DEBUG) serialOut += "processCommands - have serial - ";
    int i = 0;
    char bytes[64] = {};
    while(Serial.available()) {
      if(DEBUG_MODE == DEBUG) serialOut += "b";
      bytes[i] = Serial.read();
      i++;
    }
    String fetched = String(bytes);
    int substr = fetched.indexOf('\n');

    // remove everything after the line ending and the line ending itself
    if(substr > -1) {
      if(DEBUG_MODE == DEBUG) serialOut += " - have line ending";
      
      isComplete = true;
    }

    serialIn += String(bytes);
    if(DEBUG_MODE == DEBUG) serialOut += " ";
    if(DEBUG_MODE == DEBUG) serialOut += serialIn;
  }

  // If not complete halt further execution for now.
  if(isComplete == false) return; 

  // assume valid
  bool isValid = true; 

  // Determine if the received checksum is valid
  if(validateChecksum(serialIn, generateChecksum(serialIn.substring(0, serialIn.lastIndexOf(" "))))) {
    if(DEBUG_MODE != COMPUTER_SERIAL) serialOut += "checksum valid ";
  }
  else {
    if(DEBUG_MODE != COMPUTER_SERIAL) serialOut += "checksum invalid ";
    if(DEBUG_MODE == COMPUTER_SERIAL) isValid = false; // invalid checksum makes the supplied command no longer valid but only if interfacing with none human.
  }

  // Only attempt to process the specified command if it is still valid
  if(isValid) { 
    // Determine if the command has an specified index
    // TODO: handle multiple specified indices's. 
    currentStripIndex = subtractStripIndex();

    if(serialIn.startsWith(COMMANDS_STRING[COMMANDS_ENUM::brightness])) {
      serialIn = serialIn.substring(String(COMMANDS_STRING[COMMANDS_ENUM::brightness]).length() +1); 
      processSetBrightness();
    }
    else if(serialIn.startsWith(COMMANDS_STRING[COMMANDS_ENUM::speed])) {
      serialIn = serialIn.substring(String(COMMANDS_STRING[COMMANDS_ENUM::speed]).length() +1);
      processSetSpeed();
    }
    else if(serialIn.startsWith(COMMANDS_STRING[COMMANDS_ENUM::pattern])) {
      serialIn = serialIn.substring(String(COMMANDS_STRING[COMMANDS_ENUM::pattern]).length() +1);
      processSetAnimation();
    }
    else if(serialIn.startsWith(COMMANDS_STRING[COMMANDS_ENUM::color])) {
      serialIn = serialIn.substring(String(COMMANDS_STRING[COMMANDS_ENUM::color]).length() +1);
      processSetColor();
    }
    else if(serialIn.startsWith(COMMANDS_STRING[COMMANDS_ENUM::path])) {
      serialIn = serialIn.substring(String(COMMANDS_STRING[COMMANDS_ENUM::path]).length() +1);
      processSetPath();
    }
    else if(serialIn.startsWith(COMMANDS_STRING[COMMANDS_ENUM::pwm])) {
      serialIn = serialIn.substring(String(COMMANDS_STRING[COMMANDS_ENUM::pwm]).length() +1);
      processSetPWM();
    }
    else {
      if(COMPUTER_SERIAL == false) Serial.print(serialIn); // not println as string already contains a /n at the end.
      isValid = false;
    }
  }

  if(isValid) serialOut += "ok ";
  else serialOut += "invalid ";

  // WARNING generated checksum is for string including the space after ok / invalid!
  serialOut += generateChecksum(serialOut);
  
  Serial.println(serialOut); // Push the combined data out
  serialOut = ""; // clear serialOut
  serialIn = ""; // clear serialIn
}

void SerialCommand::processSetBrightness() {
  long brightness = serialIn.toInt();
  if(brightness > UINT8_MAX) brightness = UINT8_MAX;
  eventHandler->eventSetBrightness(brightness, currentStripIndex);
}

void SerialCommand::processSetSpeed() {
  long speed = serialIn.toInt();
  if(speed > UINT16_MAX) speed = UINT16_MAX;
  eventHandler->eventSetSpeed(speed, currentStripIndex);
}

void SerialCommand::processSetAnimation() {
  animation anim = NO_PATTERN_UPDATE;

  if(serialIn.startsWith(LED_ANIMATION_STRING[animation::RAINBOW])) {
    anim = RAINBOW;
  }
  else if(serialIn.startsWith(LED_ANIMATION_STRING[animation::SUNRISE])) {
    anim = SUNRISE;
  }
  else if(serialIn.startsWith(LED_ANIMATION_STRING[animation::COLOR_SOLID])) {
    anim = COLOR_SOLID;
  }
  else if(serialIn.startsWith(LED_ANIMATION_STRING[animation::STROBE_SOLID])) {
    anim = STROBE_SOLID;
  }
  else if(serialIn.startsWith(LED_ANIMATION_STRING[animation::COLOR_WIPE_CHRISTMAS])) {
    anim = COLOR_WIPE_CHRISTMAS;
  }
  else if(serialIn.startsWith(LED_ANIMATION_STRING[animation::COLOR_WIPE_RANDOM])) {
    anim = COLOR_WIPE_RANDOM;
  }
  else if(serialIn.startsWith(LED_ANIMATION_STRING[animation::SCANNER_SOLID])) {
    anim = SCANNER_SOLID;
  }
  else if(serialIn.startsWith(LED_ANIMATION_STRING[animation::SCANNER_RANDOM])) {
    anim = SCANNER_RANDOM;
  }
  else if(serialIn.startsWith(LED_ANIMATION_STRING[animation::FADE_SOLID])) {
    anim = FADE_SOLID;
  }
  else if(serialIn.startsWith(LED_ANIMATION_STRING[animation::FADE_RANDOM])) {
    anim = FADE_RANDOM;
  }
  else if(serialIn.startsWith(LED_ANIMATION_STRING[animation::FIRE_SOLID])) {
    anim = FIRE_SOLID;
  }
  else if(serialIn.startsWith(LED_ANIMATION_STRING[animation::METEOR_SOLID])) {
    anim = METEOR_SOLID;
  }
  else if(serialIn.startsWith(LED_ANIMATION_STRING[animation::METEOR_SCANNER_SOLID])) {
    anim = METEOR_SCANNER_SOLID;
  }
  
  eventHandler->eventSetAnimation(anim, currentStripIndex);
}

void SerialCommand::processSetColor() {
  int colorIndex = -1;
  if(hasNextPart()) {
    colorIndex = getNextPart().toInt();
    serialIn = serialIn.substring(2);
  }

  long color = serialIn.toInt();
  if(color > UINT32_MAX) color = UINT32_MAX;
  eventHandler->eventSetColor(color, colorIndex, currentStripIndex);
}

void SerialCommand::processSetPath() {
  direction dir = FORWARD;
  if(serialIn.startsWith(DIRECTION_STRING[direction::REVERSE])) dir = REVERSE;
  eventHandler->eventSetPath(dir, currentStripIndex);
}

void SerialCommand::processSetPWM() {
  long pwm = serialIn.toInt();
  if(pwm > 255) pwm = 255;
  eventHandler->eventSetPWM(pwm);
}

bool SerialCommand::validateChecksum(String input, uint32_t checksum) {
  uint32_t inputChecksum = serialIn.substring(input.lastIndexOf(" ")).toInt();
  return (inputChecksum == checksum) ? true : false;
}

/**
 * 
 */
uint32_t SerialCommand::generateChecksum(String input) {
  return CRC32::calculate(input.c_str(), input.length());
}

/**
 * Determine the specified strip index (should be at the start of the command) and subtract it from serialIn
 * @return the specified index if found or -1 otherwise.
 */
int8_t SerialCommand::subtractStripIndex() {
  String nextPart = getNextPart();
  int8_t stripIndex = nextPart.toInt();
  if(stripIndex != 0 || nextPart.indexOf("0") == 0) serialIn = serialIn.substring(nextPart.length() +1);
  else stripIndex = -1;
  return stripIndex;
}

bool SerialCommand::hasNextPart() {
  return (serialIn.indexOf(" ") == -1) ? false : true; 
}

String SerialCommand::getNextPart() {
  return serialIn.substring(0, serialIn.indexOf(" "));
}