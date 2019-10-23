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

/*
 * Arduino based dependencies (Can be installed through Arduino IDE)
 * ArduinoSTL         by Mike Matera
 * Adafruit GFX       by Adafruit
 * Adafruit NeoPixel  by Adafruit
 * CRC32              by Christopher Baker
 */

#include <ArduinoSTL.h>
#include <list>

#include "typedefinitions.h"
#include "serialcommand.h"
#include "deltatimer.h"
#include "neopatterns.h"
#include "neoanimation.h"
#include "framerate.h"

// constants //
static const int         PIXEL_COUNT         = 30;
static const animation   INITIAL_ANIMATION   = RAINBOW;
static const int         INITIAL_BRIGHTNESS  = 255;
static const int         INITIAL_SPEED       = 50;

// input / output //
static const int        ledPin            = LED_BUILTIN;
static const int        stripStartPin     = 10;
static const int        numStrips         = 7;

// callbacks & other predeclaritive functions // 
void stickComplete(NeoAnimation* stick);
void boardSpecificSetup();

// none const variables and objects // 
DeltaTimer                deltaTimer; // allows update function with deltaTime increments to send to all actors
FrameRate                 rate; // keep track of current framerate
std::list<NeoAnimation*>  patterns          = std::list<NeoAnimation*>();


int       lastPowerButtonState = 0;
bool      ledState = false;

// TODO move this two branched template code into a function were only the specifics are passed

class SerialCommandHandler: public SerialCommandDelegate {
  void eventSetBrightness(uint8_t brightness, int8_t stripIndex){

    if(stripIndex != -1 && stripIndex < (long)patterns.size()) {
      std::list<NeoAnimation*>::iterator it = patterns.begin();
      std::advance (it, stripIndex);
      (*it)->setBrightness(brightness);
      return;
    }

    for (std::list<NeoAnimation*>::iterator it = patterns.begin(); it != patterns.end(); it++) {
      (*it)->setBrightness(brightness);
    }
  }

  void eventSetSpeed(uint16_t speed, int8_t stripIndex){

    if(stripIndex != -1 && stripIndex < (long)patterns.size()) {
      std::list<NeoAnimation*>::iterator it = patterns.begin();
      std::advance (it, stripIndex);
      (*it)->Interval = speed;
      return;
    }

    for (std::list<NeoAnimation*>::iterator it = patterns.begin(); it != patterns.end(); it++) {
      (*it)->Interval = speed;
    }
  }

  void eventSetAnimation(animation pattern, int8_t stripIndex){

    if(stripIndex != -1 && stripIndex < (long)patterns.size()) {
      std::list<NeoAnimation*>::iterator it = patterns.begin();
      std::advance (it, stripIndex);
      (*it)->animationSwitch(pattern);
      return;
    }

    for (std::list<NeoAnimation*>::iterator it = patterns.begin(); it != patterns.end(); it++) {
      (*it)->animationSwitch(pattern);
    }
  }

  void eventSetColor(uint32_t color, int8_t colorIndex, int8_t stripIndex){

    if(stripIndex != -1 && stripIndex < (long)patterns.size()) {
      std::list<NeoAnimation*>::iterator it = patterns.begin();
      std::advance (it, stripIndex);
      if(colorIndex == -1 || colorIndex == 1) (*it)->Color1 = color;
      if(colorIndex == -1 || colorIndex == 2) (*it)->Color2 = color;
      return;
    }

    for (std::list<NeoAnimation*>::iterator it = patterns.begin(); it != patterns.end(); it++) {
      if(colorIndex == -1 || colorIndex == 1) (*it)->Color1 = color;
      if(colorIndex == -1 || colorIndex == 2) (*it)->Color2 = color;
    }
  }

    void eventSetIndex(uint16_t index, bool percentage, int8_t stripIndex){

    if(stripIndex != -1 && stripIndex < (long)patterns.size()) {
      std::list<NeoAnimation*>::iterator it = patterns.begin();
      std::advance (it, stripIndex);
      if(percentage) index = ((*it)->TotalSteps * index) / 100;
      (*it)->Index = index;
      return;
    }

    for (std::list<NeoAnimation*>::iterator it = patterns.begin(); it != patterns.end(); it++) {
      (*it)->Index = index;
    }
  }

  void eventSetPath(direction dir, int8_t stripIndex){

    if(stripIndex != -1 && stripIndex < (long)patterns.size()) {
      std::list<NeoAnimation*>::iterator it = patterns.begin();
      std::advance (it, stripIndex);
      (*it)->Direction = dir;
      return;
    }

    for (std::list<NeoAnimation*>::iterator it = patterns.begin(); it != patterns.end(); it++) {
      (*it)->Direction = dir;
    }
  }

  void eventSetPWM(uint8_t pwm){
    analogWrite(11, pwm);
  }
};

SerialCommandHandler serialCH = SerialCommandHandler();
SerialCommand serialC = SerialCommand(&serialCH);

/**
 * Initialize Serial, IO and the rgb leds
 */
void setup() {
  Serial.begin(9600);
  Serial.println("RGBDriver V2");

  pinMode(11, OUTPUT); // PWM LED

  boardSpecificSetup();

  for(uint8_t i = 0; i < numStrips; i++) {
    int pixels = PIXEL_COUNT;
    // if(i == numStrips -1) pixels *= 2; // override pixel count for specific indexes
    //NeoAnimation* stick = new NeoAnimation(pixels, (stripStartPin-i), NEO_GRB + NEO_KHZ800, &stickComplete); 
    NeoAnimation* stick = new NeoAnimation(pixels, (stripStartPin-i), NEO_GRB + NEO_KHZ800); 
    patterns.push_back(stick);
    stick->begin();
    stick->animationSwitch(INITIAL_ANIMATION);
    stick->setBrightness(INITIAL_BRIGHTNESS);
    stick->Interval = INITIAL_SPEED;
  }

  // initialize outputs
  pinMode(ledPin, OUTPUT); 
}

/**
 * Runs board specific configuration
 */
void boardSpecificSetup() {
  // pro mini only
  #ifdef ARDUINO_AVR_PRO
    if(DEBUG_MODE == DEBUG) Serial.println("Dev: Arduino pro");
    //TIMSK0 = 0; // disable timer0 - required for micros() en millis()
    TIMSK1 = 0; // disable timer1
    TIMSK2 = 0; // disable timer2
    DIDR0 = 0x000011; // disable digital input into adc
  #endif

  #ifdef ARDUINO_ARCH_ESP32
    if(DEBUG_MODE == DEBUG) Serial.println("Dev: ESP32");
    analogReadResolution(8);
    analogSetCycles(3);
  #endif
}

void loop() {
  // update to main deltaTimer
  deltaTimer.update(micros());
  uint32_t deltaTime = deltaTimer.getDeltaTime();

  // change ledstrip settings
  for (std::list<NeoAnimation*>::iterator it = patterns.begin(); it != patterns.end(); it++) {
    (*it)->update(deltaTime);
  }

  // determine if ledstrip update is appropriate for the passed time
  rate.update(deltaTime);

  serialC.processCommands();

  // output serial information
  if(rate.getFrames() >= rate.getAverage()) { // display one serial message every second
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);
    if(DEBUG_MODE == DEBUG) {
      for (std::list<NeoAnimation*>::iterator it = patterns.begin(); it != patterns.end(); it++) {
        Serial.print("{");
        Serial.print((*it)->getBrightness());
        Serial.print(",");
        Serial.print((*it)->Interval);
        Serial.print(",");
        Serial.print(PATTERN_STRING[(*it)->ActivePattern]);
        Serial.print(",");
        Serial.print(DIRECTION_STRING[(*it)->Direction]);
        Serial.print(",");
        Serial.print(TRACE_STRING[(*it)->Trace]);
        Serial.print(",");
        Serial.print((*it)->Color1);
        Serial.print(",");
        Serial.print((*it)->Color2);
        Serial.print(",");
        Serial.print((*it)->TotalSteps);
        Serial.print(",");
        Serial.print((*it)->Index);
        Serial.println("}");
      }
      Serial.print(rate.get());
      // Serial.print(" ");
      // if((1.d / currentFrames * 1000.d) > currentSpeed) { Serial.print("LAGG");}
      Serial.println();
    }
  }
}