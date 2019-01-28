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


#ifndef _SERIAL_COMMAND_H
#ifdef __cplusplus
#define _SERIAL_COMMAND_H

#include <Arduino.h>
#include <CRC32.h>
#include "typedefinitions.h"
#include "neopatterns.h"
#include "neoanimation.h"


/**
 * Abstract delegate to allow outside of SerialCommand processing of events
 */
class SerialCommandDelegate {
  public:
    virtual void eventSetBrightness(uint8_t brightness, int8_t stripIndex = -1) = 0;
    virtual void eventSetSpeed(uint16_t speed, int8_t stripIndex = -1) = 0;
    virtual void eventSetAnimation(animation anim, int8_t stripIndex = -1) = 0;
    virtual void eventSetColor(uint32_t color, int8_t colorIndex = -1, int8_t stripIndex = -1) = 0;
    virtual void eventSetPath(direction dir, int8_t stripIndex = -1) = 0;
    virtual void eventSetPWM(uint8_t pwm) = 0;
};

class SerialCommand {
  private:
    SerialCommandDelegate* eventHandler;
    String serialIn;
    String serialOut;

    int8_t currentStripIndex;

    void processSetBrightness();
    void processSetSpeed();
    void processSetAnimation();
    void processSetColor();
    void processSetPath();
    void processSetPWM();

    /* Checksumming 
     * TODO: move into base class
     */
    bool      validateChecksum(String input, uint32_t checksum);
    uint32_t  generateChecksum(String input);

    int8_t    subtractStripIndex();
    bool      hasNextPart();
    String    getNextPart();
  public:
    enum COMMANDS_ENUM {
      brightness, speed, pattern, color, path, pwm
    };

    static const char *COMMANDS_STRING[];

    SerialCommand(SerialCommandDelegate* eventHandler);

    void processCommands();
};

#endif /* __cplusplus */
#endif /* _SERIAL_COMMAND_H */