#include "rgbrltypedef.h"
#include "microtime.h" // library to keep track of time 
#include "rgbcolor.h" // library to hold a rgb color
#include "alarm.h"
#include "ledpattern.h"
#include "simpleledpattern.h"
#include "templateneopixel.h"

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

std::list<alarm> alarms = std::list<alarm>();
std::list<ledPattern*> ledPatterns = std::list<ledPattern*>();

templateNeopixel strip1 = templateNeopixel();

std::random_device rd; // obtain a random number from hardware
std::mt19937 eng(rd()); // seed the generator
std::uniform_int_distribution<> colorRand(0, 256);
std::uniform_int_distribution<> timeRand(1000, 10000);

// declaration to prevent undeclared function error
 void setAllPixels(rgbColor color);
void setup();
void loop();
void loopDeltaTime();
void updateTime(unsigned long timeBetweenUpdate);
void checkAlarms();

/*
 * 
 */
int main(int argc, char** argv) {
    setup();
    loop();
    return 0;
}

void setup() {
  ledPattern* testPattern = new simpleLedPattern(strip1, rgbColor(0,0,0), rgbColor(255,0,0), 90000000UL, simpleLedPattern::patternModes::linear);
  testPattern->setSafeDelete(true);
  
  alarm *testAlarm1 = new alarm(microTime((unsigned int)2017, (byte)8, (byte)18, (byte)6, (byte)0, (byte)0), 86400UL, *testPattern);

  std::cout << "push alarms" << std::endl;
  alarms.push_front(*testAlarm1);
//  alarms.push_front(*testAlarm2);
//  alarms.push_front(*testAlarm3);
//  alarms.push_front(*testAlarm4);
//  alarms.push_front(*testAlarm5);

  std::cout << "push patterns"  << std::endl;
  ledPatterns.push_front(testPattern);
//  ledPatterns.push_front(alarmPatternPhase1);
//  ledPatterns.push_front(alarmPatternPhase2);
//  ledPatterns.push_front(alarmPatternPhase3);
}

void loop() {
    while(true) {
        loopDeltaTime();
    }
}

void loopDeltaTime() {
  currentMicros = currentMicros + 200;

  // micros overflow reset condition -> this happens about once every 70 minutes
  // this will mean we lose some time data because of the missing microseconds around the overflow condition.
  // In order to minimize the amount of microseconds lost keep REQUEST_DELAY as small as possible.
  if(previousMicros > currentMicros) {
    std::cout << "Micros overflow" << std::endl;
    previousMicros = currentMicros;
  }
  
  if(currentMicros - previousMicros > REQUEST_DELAY) {
    // Serial.println("update time");
    updateTime(currentMicros - previousMicros);

    // Serial.println("check alarms");
    checkAlarms();

    // Serial.println("single pattern");
    if(runningSinglePattern) {
      std::cout << ledPatterns.size() << std::endl;
      for (std::list<ledPattern*>::iterator singlePattern = ledPatterns.begin(); singlePattern != ledPatterns.end(); singlePattern++) {
        // std::cout << 'i';
        ledPattern* castPattern = *singlePattern;
        if(castPattern->isFinished()) {
          std::cout << "pattern finished" << std::endl;
          numPatterns++;

          simpleLedPattern* simpleSinglePattern = static_cast<simpleLedPattern*>(castPattern);

          if(simpleSinglePattern != NULL) {
            std::cout << "pattern not null" << std::endl;
            templateNeopixel* strip = simpleSinglePattern->getStrip();
            byte newfRed = (byte)colorRand(eng);
            byte newfBlue = (byte)colorRand(eng);
            byte newfGreen = (byte)colorRand(eng);
            uint32_t newTime = timeRand(eng) * 500UL;
            
            rgbColor newColor = simpleSinglePattern->getColor();
            rgbColor endColor = rgbColor(newfRed, newfBlue, newfGreen);
    
            // WARNING - we should not delete pattern if it is part of an alarm with an interval!!!
            // delete pattern; // we really can't call delete on a pointer variable that was not created with new;
            // TODO - implement way of determinging weither ledPattern can be removed

            if(simpleSinglePattern->isSafeDelete()) {
              std::cout << "delete pattern" << std::endl;
              // singlePattern = ledPatterns.erase(singlePattern);
              singlePattern = ledPatterns.erase(singlePattern);
              delete simpleSinglePattern;
            } else {
                
              // ledPatterns.erase(singlePattern);
            }
            
            std::cout << "create new pattern" << std::endl;
            simpleLedPattern* newSimplePattern = new simpleLedPattern(*strip, newColor, endColor, newTime, simpleLedPattern::patternModes::linear);
            newSimplePattern->setSafeDelete(true);

            std::cout << "assign new pattern" << std::endl;
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
      std::cout << activeTime.year() << std::endl;
      std::cout << "-" << std::endl;
      std::cout << activeTime.month() << std::endl;
      std::cout << "-" << std::endl;
      std::cout << activeTime.day() << std::endl;
      std::cout << " " << std::endl;
      std::cout << activeTime.hour() << std::endl;
      std::cout << ":" << std::endl;
      std::cout << activeTime.minute() << std::endl;
      std::cout << ":" << std::endl;
      std::cout << activeTime.second() << std::endl;
      
      averageFramerate = floor(averageFramerate / 60);
      std::cout << " averageFramerate: " << std::endl;
      std::cout << averageFramerate << std::endl;
      averageFramerate = 0;

      std::cout << " numPatterns: " << std::endl;
      std::cout << numPatterns << std::endl;
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
        std::cout << "Sound the alarm" << std::endl;
        ledPatterns.push_front(it->getAlarmPattern());
        it->getAlarmPattern()->reset();

        // singlePattern = it->getAlarmPattern();
        // singlePattern->reset();

        // WARNING this could cause our ledPattern to be removed from existence since we pass it as an pointer
        // In this case it does not since the alarm is created during compile time and later assigned to the list.
        // Removing it from the list will not remove the alarm from memory and neither the ledPattern
        if(!it->getInterval()) {
          std::cout << "Erase the alarm" << std::endl;
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
//void setPixel(rgbColor color, unsigned int pixel, Adafruit_NeoPixel strip) {
//   strip.setPixelColor(pixel,
//        (byte)((float)color.getRed()),
//        (byte)((float)color.getGreen()),
//        (byte)((float)color.getBlue()));
//}

/**
 * set the led strip to the desired color
 */
void setAllPixels(rgbColor color) 
{
//  for (int iPixel = 0; iPixel < LED_COUNT; iPixel++) {
//    strip1.setPixelColor(iPixel,
//                        (byte)((float)color.getRed()),
//                        (byte)((float)color.getGreen()),
//                        (byte)((float)color.getBlue()));
//  }
//  for (int iPixel = 0; iPixel < LED_COUNT; iPixel++) {
//    strip2.setPixelColor(iPixel,
//                        (byte)((float)color.getRed()),
//                        (byte)((float)color.getGreen()),
//                        (byte)((float)color.getBlue()));
//  }
//  for (int iPixel = 0; iPixel < LED_COUNT; iPixel++) {
//    strip3.setPixelColor(iPixel,
//                        (byte)((float)color.getRed()),
//                        (byte)((float)color.getGreen()),
//                        (byte)((float)color.getBlue()));
//  }
//  for (int iPixel = 0; iPixel < LED_COUNT; iPixel++) {
//    strip4.setPixelColor(iPixel,
//                        (byte)((float)color.getRed()),
//                        (byte)((float)color.getGreen()),
//                        (byte)((float)color.getBlue()));
//  }
//  for (int iPixel = 0; iPixel < LED_COUNT; iPixel++) {
//    strip5.setPixelColor(iPixel,
//                        (byte)((float)color.getRed()),
//                        (byte)((float)color.getGreen()),
//                        (byte)((float)color.getBlue()));
//  }
//  
//  strip1.show();
//  strip2.show();
//  strip3.show();
//  strip4.show();
//  strip5.show();
}

