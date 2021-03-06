#include <ArduinoSTL.h>
#include <list>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "microtime.h" // library to keep track of time 
#include "rgbcolor.h" // library to hold a rgb color
#include "alarm.h"
#include "ledpattern.h"
#include "simpleledpattern.h"
#include "serialcommand.h"

#define BRIGHTNESS 250

// static const int  BUTTON_PIN = D1;  // hardware pin for the snooze / reset button
static const int LED_PIN_1 = 6;  // hardware pin for the led strip serial data
static const int LED_PIN_2 = 7;  // hardware pin for the led strip serial data
static const int LED_PIN_3 = 8;  // hardware pin for the led strip serial data
static const int LED_PIN_4 = 9;  // hardware pin for the led strip serial data
static const int LED_PIN_5 = 10;  // hardware pin for the led strip serial data
static const int LED_COUNT =  30;  // amount of leds on the led strip

static const int FLASH_DELAY = 125;
static const int OSCILLATION_TIME = 500; // used to oscillate the rgb led strip
static const int REQUEST_DELAY = 3900; // minimum time between alarm checks and clock updates in micro seconds - 3900 microseconds = 255 fps
// static const unsigned long REQUEST_DELAY = 83332; // 83332 microseconds = 12 fps

static const uint32_t MICROS_TO_SECONDS = 1000000; // constant value to convert micro seconds to seconds

const static rgbColor SITAHS_FAVORITE_MINT_GREEN = rgbColor(0, 255, 75);
const static rgbColor MARKS_FAVORITE_PINK = rgbColor(255, 100, 83);
const static rgbColor RICOS_FAVORITE_BLUE = rgbColor(0, 72, 251);

/* ===================== Constants end here ============================== */

bool buttonBounce = false; // boolean to bounce button so we prevent multiple triggers
bool hasNotFlashed = false; // boolean to flash when the pattern is complete
uint32_t currentMicros = 0; // used in loop to store micros
uint32_t previousMicros = 0; // used in loop to store micros
int curTimeZone = 1; // timeZone for the current time
int lastMinute = 0;
uint32_t averageFramerate = 0;
uint32_t numPatterns = 0;

/* ====================== Settings ==================== */
bool autoTurnOff = false; // always turn lights off when their is no active pattern
bool runningSinglePattern = true; // continious running patterns
bool minuteFlicker = true; // boolean to enable or disable flashing every minute
unsigned int numMinuteFlicker = 3; // amount of times to flicker every minute

/* ====================== Instances ====================== */

// Timer timeKeeper; // our continous time keeper
// microTime activeTime = microTime(999, 12, 30, 23 , 59, 59); // current system time
microTime activeTime = microTime(2010, 8, 18, 15 , 00, 00); // current system time
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(LED_COUNT, LED_PIN_1, NEO_GRB + NEO_KHZ800); // lights 
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(LED_COUNT, LED_PIN_2, NEO_GRB + NEO_KHZ800); // lights
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(LED_COUNT, LED_PIN_3, NEO_GRB + NEO_KHZ800); // lights
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(LED_COUNT, LED_PIN_4, NEO_GRB + NEO_KHZ800); // lights
Adafruit_NeoPixel strip5 = Adafruit_NeoPixel(LED_COUNT, LED_PIN_5, NEO_GRB + NEO_KHZ800); // lights
// ledPattern *singlePattern = new ledPattern(rgbColor(255, 0, 0), rgbColor(0, 255, 0), 10000000UL, ledPattern::patternModes::linear);

std::list<alarm> alarms = std::list<alarm>();
std::list<ledPattern*> ledPatterns = std::list<ledPattern*>();


// ledPattern testPattern = ledPattern(strip1);
// ledPattern testPattern = simpleLedPattern(strip1, rgbColor(0,0,0), rgbColor(255,0,0), 90000000UL, simpleLedPattern::patternModes::linear);

// ledPattern alarmPatternPhase1 = simpleLedPattern(strip1, rgbColor(0,0,0), rgbColor(255,0,0), (uint32_t)90000000, simpleLedPattern::patternModes::linear);
// ledPattern alarmPatternPhase2 = simpleLedPattern(strip2, rgbColor(255,0,0), rgbColor(0,0,255), (uint32_t)90000000, simpleLedPattern::patternModes::linear);
// ledPattern alarmPatternPhase3 = simpleLedPattern(strip3, rgbColor(0,0,255), rgbColor(255,255,255), (uint32_t)90000000, simpleLedPattern::patternModes::linear);

//alarm *testAlarm1 = new alarm(microTime((unsigned int)2017, (byte)8, (byte)18, (byte)6, (byte)0, (byte)0), 86400UL, testPattern);
//alarm *testAlarm2 = new alarm(microTime((unsigned int)2017, (byte)8, (byte)18, (byte)6, (byte)2, (byte)0), 86400UL, alarmPatternPhase2);
//alarm *testAlarm3 = new alarm(microTime((unsigned int)2017, (byte)8, (byte)18, (byte)6, (byte)4, (byte)0), 86400UL, alarmPatternPhase3);
//alarm *testAlarm4 = new alarm(microTime((unsigned int)2017, (byte)8, (byte)15, (byte)19, (byte)1, (byte)0), alarmPatternPhase1);
//alarm *testAlarm5 = new alarm(microTime((unsigned int)2017, (byte)8, (byte)15, (byte)19, (byte)4, (byte)0), alarmPatternPhase1);

// declaration to prevent undeclared function error
void setAllPixels(rgbColor color);

class serialCommandHandler: public serialCommandDelegate {
  void eventSetTime(microTime newTime) {
    activeTime = newTime;
  }

  void eventSetRunningPatterns(bool newRunningPatterns) {
    runningSinglePattern = newRunningPatterns;
  }

  void eventSetMinuteFlicker(bool newFlicker) {
    minuteFlicker = newFlicker;
  }

  void eventSetNumMinuteFlicker(int newFlicker) {
    numMinuteFlicker = newFlicker;
  }

  void eventSetLeds(bool newLeds) {
    autoTurnOff = newLeds;
  }
};

serialCommandHandler serialCH = serialCommandHandler();
serialCommand serialC = serialCommand(&serialCH);

void setup() {
  Serial.begin(19200);
  strip1.setBrightness(BRIGHTNESS);
  strip1.begin();
  strip1.show(); // Initialize all pixels to 'off'
  strip2.setBrightness(BRIGHTNESS);
  strip2.begin();
  strip2.show(); // Initialize all pixels to 'off'
  strip3.setBrightness(BRIGHTNESS);
  strip3.begin();
  strip3.show(); // Initialize all pixels to 'off'
  strip4.setBrightness(BRIGHTNESS);
  strip4.begin();
  strip4.show(); // Initialize all pixels to 'off'
  strip5.setBrightness(BRIGHTNESS);
  strip5.begin();
  strip5.show(); // Initialize all pixels to 'off'

  ledPattern* testPattern = new simpleLedPattern(strip1, rgbColor(0,0,0), rgbColor(255,0,0), 90000000UL, simpleLedPattern::patternModes::linear);
  testPattern->setSafeDelete(true);

  alarm *testAlarm1 = new alarm(microTime((unsigned int)2017, (byte)8, (byte)18, (byte)6, (byte)0, (byte)0), 86400UL, *testPattern);

  Serial.println("push alarms");
  alarms.empty();
  alarms.push_front(*testAlarm1);
//  alarms.push_front(*testAlarm2);
//  alarms.push_front(*testAlarm3);
//  alarms.push_front(*testAlarm4);
//  alarms.push_front(*testAlarm5);

  Serial.println("push patterns");
  ledPatterns.empty();
  ledPatterns.push_front(testPattern);
//  ledPatterns.push_front(alarmPatternPhase1);
//  ledPatterns.push_front(alarmPatternPhase2);
//  ledPatterns.push_front(alarmPatternPhase3);
}

void loop() {
  loopDeltaTime();
}

void loopDeltaTime() {
  currentMicros = micros();

  // micros overflow reset condition -> this happens about once every 70 minutes
  // this will mean we lose some time data because of the missing microseconds around the overflow condition.
  // In order to minimize the amount of microseconds lost keep REQUEST_DELAY as small as possible.
  if(previousMicros > currentMicros) {
    Serial.println("Micros overflow");
    previousMicros = currentMicros;
  }
  
  if(currentMicros - previousMicros > REQUEST_DELAY) {
    serialC.processCommands();
    // Serial.println("update time");
    updateTime(currentMicros - previousMicros);

    // Serial.println("check alarms");
    checkAlarms();

//    if(runningSinglePattern) {
//      if(singlePattern->isFinished()) {
//        numPatterns++;
//
//        byte newfRed = (byte)random(0,255);
//        byte newfBlue = (byte)random(0,255);
//        byte newfGreen = (byte)random(0,255);
//        unsigned long newTime = random(1000, 10000) * 500UL;
//        rgbColor newColor = singlePattern->getColor();
//        rgbColor endColor = rgbColor(newfRed, newfBlue, newfGreen);
//
//        // WARNING - we should not delete pattern if it is part of an alarm with an interval!!!
//        // delete pattern; // we really can't call delete on a pointer variable that was not created with new;
//        // TODO - implement way of determinging weither ledPattern can be removed
//        
//        if(singlePattern->isSafeDelete()) {
//          delete singlePattern;
//        }
//        singlePattern = new ledPattern(newColor, endColor, newTime, ledPattern::patternModes::linear);
//        singlePattern->setSafeDelete(true);
//      }
//      else {
//        singlePattern->update(currentMicros - previousMicros);
//        setAllPixels(singlePattern->getColor());
//      }
//    }

    // Serial.println("single pattern");
    if(runningSinglePattern) {
      for (std::list<ledPattern*>::iterator singlePattern = ledPatterns.begin(); singlePattern != ledPatterns.end(); singlePattern++) {
        ledPattern* castPattern = *singlePattern;
        // Serial.print("i");
        if(castPattern->isFinished()) {
          Serial.println("pattern finished");
          numPatterns++;

          Serial.println("cast pattern");
          simpleLedPattern* simpleSinglePattern = static_cast<simpleLedPattern*>(castPattern);

          if(simpleSinglePattern != NULL) {
            Serial.println("pattern not null");
            Adafruit_NeoPixel* strip = simpleSinglePattern->getStrip();
            byte newfRed = (byte)random(0,255);
            byte newfBlue = (byte)random(0,255);
            byte newfGreen = (byte)random(0,255);
            unsigned long newTime = random(1000, 10000) * 500UL;
            
            rgbColor newColor = simpleSinglePattern->getColor();
            rgbColor endColor = rgbColor(newfRed, newfBlue, newfGreen);
    
            // WARNING - we should not delete pattern if it is part of an alarm with an interval!!!
            // delete pattern; // we really can't call delete on a pointer variable that was not created with new;
            // TODO - implement way of determinging weither ledPattern can be removed

            if(simpleSinglePattern->isSafeDelete()) {
              Serial.println("delete pattern");
              // singlePattern = ledPatterns.erase(singlePattern);
              singlePattern = ledPatterns.erase(singlePattern);
              delete simpleSinglePattern;
            } else {
              // ledPatterns.erase(singlePattern);
            }
            
            
            Serial.println("create new pattern");
            simpleLedPattern* newSimplePattern = new simpleLedPattern(*strip, newColor, endColor, newTime, simpleLedPattern::patternModes::linear);
            newSimplePattern->setSafeDelete(true);

            Serial.println("assign new pattern");
            ledPatterns.push_front(newSimplePattern);
          }
        }
        else {
          // Serial.println("increment pattern");
          simpleLedPattern* simpleSinglePattern = static_cast<simpleLedPattern*>(castPattern);

          if(simpleSinglePattern != NULL) {
            simpleSinglePattern->increment(currentMicros - previousMicros);
          }
          // setAllPixels(singlePattern->getColor());
        }
      }
    }
    
    if(autoTurnOff) {
      setAllPixels(rgbColor(0,0,0));
    }
    
    if(lastMinute != activeTime.minute()) {
      lastMinute = activeTime.minute();
      Serial.print(activeTime.year());
      Serial.print("-");
      Serial.print(activeTime.month());
      Serial.print("-");
      Serial.print(activeTime.day());
      Serial.print(" ");
      Serial.print(activeTime.hour());
      Serial.print(":");
      Serial.print(activeTime.minute());
      Serial.print(":");
      Serial.print(activeTime.second());
      
      averageFramerate = floor(averageFramerate / 60);
      Serial.print(" averageFramerate: ");
      Serial.print(averageFramerate);
      averageFramerate = 0;

      Serial.print(" numPatterns: ");
      Serial.println(numPatterns);
      numPatterns = 0;

      // Only flicker if minuteFlicker is enabled
//      if(minuteFlicker) {
//        // flicker for numMinuteFlicker amount of times
//        for(unsigned int i = 0; i < numMinuteFlicker; i++) { 
//          setAllPixels(rgbColor(0,0,0));
//          delay(FLASH_DELAY);
//          setAllPixels(singlePattern->getColor());
//          delay(FLASH_DELAY);
//        }
//      }
    }

    averageFramerate++;
    previousMicros = currentMicros;
  }
}

/**
 * Update the system time using the Time library and the progress in microseconds
 * @Param timeBetweenUpdate amount of microseconds to update the current time
 */
void updateTime(unsigned long timeBetweenUpdate) {
  activeTime.update(timeBetweenUpdate);
}

/**
 * 
 */
void checkAlarms() {
  for (std::list<alarm>::iterator it = alarms.begin(); it != alarms.end(); it++) {
//      Serial.print("Checking alarm: ");
//      Serial.print(it->getTrigger()->year());
//      Serial.print("-");
//      Serial.print(it->getTrigger()->month());
//      Serial.print("-");
//      Serial.print(it->getTrigger()->day());
//      Serial.print(" ");
//      Serial.print(it->getTrigger()->hour());
//      Serial.print(":");
//      Serial.print(it->getTrigger()->minute());
//      Serial.print(":");
//      Serial.println(it->getTrigger()->second());
      if(it->check(activeTime) == true) {
        Serial.println("Sound the alarm");
        ledPatterns.push_front(it->getAlarmPattern());
        it->getAlarmPattern()->reset();

        // singlePattern = it->getAlarmPattern();
        // singlePattern->reset();

        // WARNING this could cause our ledPattern to be removed from existence since we pass it as an pointer
        // In this case it does not since the alarm is created during compile time and later assigned to the list.
        // Removing it from the list will not remove the alarm from memory and neither the ledPattern
        if(!it->getInterval()) {
          Serial.println("Erase the alarm");
          alarms.erase(it);
        }
      }
  }
}

/**
 * set the led strip to the desired color
 * @Param r amount of red in 256 steps
 * @Param g amount of green in 256 steps
 * @Param b amount of blue in 256 steps
 * @Param mulitplier multiplies all the color values by this amount leave 1 for as is
 */
void setPixel(rgbColor color, unsigned int pixel, Adafruit_NeoPixel strip) {
   strip.setPixelColor(pixel,
        (byte)((float)color.getRed()),
        (byte)((float)color.getGreen()),
        (byte)((float)color.getBlue()));
}

/**
 * set the led strip to the desired color
 */
void setAllPixels(rgbColor color) 
{
  for (int iPixel = 0; iPixel < LED_COUNT; iPixel++) {
    strip1.setPixelColor(iPixel,
                        (byte)((float)color.getRed()),
                        (byte)((float)color.getGreen()),
                        (byte)((float)color.getBlue()));
  }
  for (int iPixel = 0; iPixel < LED_COUNT; iPixel++) {
    strip2.setPixelColor(iPixel,
                        (byte)((float)color.getRed()),
                        (byte)((float)color.getGreen()),
                        (byte)((float)color.getBlue()));
  }
  for (int iPixel = 0; iPixel < LED_COUNT; iPixel++) {
    strip3.setPixelColor(iPixel,
                        (byte)((float)color.getRed()),
                        (byte)((float)color.getGreen()),
                        (byte)((float)color.getBlue()));
  }
  for (int iPixel = 0; iPixel < LED_COUNT; iPixel++) {
    strip4.setPixelColor(iPixel,
                        (byte)((float)color.getRed()),
                        (byte)((float)color.getGreen()),
                        (byte)((float)color.getBlue()));
  }
  for (int iPixel = 0; iPixel < LED_COUNT; iPixel++) {
    strip5.setPixelColor(iPixel,
                        (byte)((float)color.getRed()),
                        (byte)((float)color.getGreen()),
                        (byte)((float)color.getBlue()));
  }
  
  strip1.show();
  strip2.show();
  strip3.show();
  strip4.show();
  strip5.show();
}



