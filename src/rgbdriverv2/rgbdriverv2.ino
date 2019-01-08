#include <ArduinoSTL.h>
#include <list>

#include "typedefinitions.h"
#include "serialcommand.h"
#include "deltatimer.h"
#include "neopatterns.h"
#include "neoanimation.h"
#include "framerate.h"

/**
* 
*/

// constants //
const int PIXEL_COUNT = 30;

// input / output //
// the dummy output
const int ledPin = LED_BUILTIN;
const int stripStartPin = 10;
const int numStrips = 5;


// callbacks & other predeclaritive functions // 
void modeSwitch(NeoPatterns* stick, animation mode);
void stickComplete(NeoPatterns* stick);
void boardSpecificSetup();

// none const variables and objects // 
DeltaTimer              deltaTimer; // allows update function with deltaTime increments to send to all actors
FrameRate               rate; // keep track of current framerate
std::list<NeoPatterns*> patterns          = std::list<NeoPatterns*>();
animation               currentMode       = RAINBOW;
int                     currentBrightness = 0;
int                     currentSpeed      = 0;

int lastPowerButtonState = 0;
uint32_t lastSerialUpdate = 0; // track when the last serial debug information
bool ledState = false;

class serialCommandHandler: public serialCommandDelegate {
  void eventSetBrightness(uint8_t brightness){
    currentBrightness = brightness;
  }

  void eventSetSpeed(uint16_t speed){
    currentSpeed = speed;
  }

  void eventSetAnimation(animation pattern){
    for (std::list<NeoPatterns*>::iterator it = patterns.begin(); it != patterns.end(); it++) {
      modeSwitch((*it), pattern);
    }
  }

  void eventSetColor(uint32_t color){
    for (std::list<NeoPatterns*>::iterator it = patterns.begin(); it != patterns.end(); it++) {
      (*it)->Color1 = color;
      (*it)->Color2 = color;
    }
  }

  void eventSetDirection(bool reverse){

  }

  void eventSetPWM(uint8_t pwm){
    analogWrite(11, pwm);
  }
};

serialCommandHandler serialCH = serialCommandHandler();
serialCommand serialC = serialCommand(&serialCH);

/**
 * Initialize Serial, IO and the rgb leds
 */
void setup() {
  if(DEBUG) Serial.begin(9600);

  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH);

  boardSpecificSetup();

  for(uint8_t i = 0; i < numStrips; i++) {
    NeoPatterns* stick = new NeoPatterns(PIXEL_COUNT, (stripStartPin-i), NEO_GRB + NEO_KHZ800, &stickComplete); 
    patterns.push_back(stick);
    stick->begin();

    currentMode = RAINBOW;
    stick->RainbowCycle(3);
    
//    currentMode = COLOR_WHITE;
//    stick->NoPattern();
//    stick->Color1 = 16777215;
//    stick->ColorSet(16777215);
  }

  // initialize outputs
  pinMode(ledPin, OUTPUT);
}

/**
 * When the pattern is done read back the color settings on the potentiometer
 */
void stickComplete(NeoPatterns* stick)
{
  // Random color change for next scan
  switch(currentMode){
      case RAINBOW:
          break;
      case COLOR_WHITE:
          break;
      case COLOR_WIPE_CHRISTMAS:
          stick->Reverse();
          if(stick->Color1 == 16711680) {
            stick->Color1 = 65280;
          }
          else {
            stick->Color1 = 16711680;
          }
          break;
      case COLOR_WIPE_RANDOM:
          stick->Reverse();
          stick->Color1 = random(2147483646);
          stick->Color2 = random(2147483646);
          break;
      case SCANNER_WHITE:
          break;
      case SCANNER_RANDOM:
          stick->Reverse();
          stick->Color1 = random(16777215);
          stick->Color2 = random(16777215);
      default:
          break;
    }
}

/**
 * Runs board specific configuration
 */
void boardSpecificSetup() {
  // pro mini only
  #ifdef ARDUINO_AVR_PRO
    if(DEBUG) Serial.println("Dev: Arduino pro");
    //TIMSK0 = 0; // disable timer0 - required for micros() en millis()
    TIMSK1 = 0; // disable timer1
    TIMSK2 = 0; // disable timer2
    DIDR0 = 0x000011; // disable digital input into adc
  #endif

  #ifdef ARDUINO_ARCH_ESP32
    if(DEBUG) Serial.println("Dev: ESP32");
    analogReadResolution(8);
    analogSetCycles(3);
  #endif
}

void loop() {
  // update to main deltaTimer
  deltaTimer.update(micros());
  uint32_t deltaTime = deltaTimer.getDeltaTime();

  // change ledstrip settings
  for (std::list<NeoPatterns*>::iterator it = patterns.begin(); it != patterns.end(); it++) {
    (*it)->setBrightness(currentBrightness);
    (*it)->Interval = currentSpeed;
    (*it)->update(deltaTime);
  }

  // determine if ledstrip update is appropriate for the passed time
  rate.update(deltaTime);

  serialC.processCommands();

  // output serial information
  if(rate.getFrames() > lastSerialUpdate + 512 || rate.hasUpdate()) {
    lastSerialUpdate = rate.getFrames(); // also auto resets lastSerialUpdate back to low value incase of rate.hasUpdate
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);
    if(DEBUG) {
      uint32_t currentFrames = rate.get();
      Serial.print(currentBrightness);
      Serial.print(" ");
      Serial.print(currentSpeed);
      Serial.print(" ");
      Serial.print(currentFrames);
      Serial.print(" ");
      if((1.d / currentFrames * 1000.d) > currentSpeed) { Serial.print("LAGG");}
      Serial.println();
    }
  }
 }

/**
 * 
 */
void modeSwitch(NeoPatterns* stick, animation mode) {
  switch(mode){
    case RAINBOW:
        currentMode = COLOR_WHITE;
        stick->NoPattern();
        stick->Color1 = 16777215;
        stick->ColorSet(16777215);
        break;
    case COLOR_WHITE:
        currentMode = COLOR_WIPE_CHRISTMAS;
        stick->ColorWipe(16711680, stick->Interval);
        break;
    case COLOR_WIPE_CHRISTMAS:
        currentMode = COLOR_WIPE_RANDOM;
        stick->ColorWipe(random(16777215), stick->Interval);
        break;
    case COLOR_WIPE_RANDOM:
        currentMode = SCANNER_WHITE;
        stick->Scanner(16777215, stick->Interval);
        break;
    case SCANNER_WHITE:
        currentMode = SCANNER_RANDOM;
        stick->Scanner(random(16777215), stick->Interval);
        break;
    case SCANNER_RANDOM:
        currentMode = RAINBOW;
        stick->RainbowCycle(stick->Interval);
        break;
    default:
        break;
  }
}

