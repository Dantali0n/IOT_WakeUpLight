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

static const unsigned long MICROS_TO_SECONDS = 1000000; // constant value to convert micro seconds to seconds

const static rgbColor SITAHS_FAVORITE_MINT_GREEN = rgbColor(0, 255, 75);
const static rgbColor MARKS_FAVORITE_PINK = rgbColor(255, 100, 83);
const static rgbColor RICOS_FAVORITE_BLUE = rgbColor(0, 72, 251);

/* ===================== Constants end here ============================== */

bool buttonBounce = false; // boolean to bounce button so we prevent multiple triggers
bool hasNotFlashed = false; // boolean to flash when the pattern is complete
unsigned long currentMicros = 0; // used in loop to store micros
unsigned long previousMicros = 0; // used in loop to store micros
int curTimeZone = 1; // timeZone for the current time
int lastMinute = 0;
unsigned int averageFramerate = 0;
unsigned int numPatterns = 0;

std::list<alarm> alarms = std::list<alarm>();
ledPattern alarmPatternPhase1 = ledPattern(rgbColor(0,0,0), rgbColor(255,0,0), 90000000UL, ledPattern::patternModes::linear);
ledPattern alarmPatternPhase2 = ledPattern(rgbColor(255,0,0), rgbColor(0,0,255), 90000000UL, ledPattern::patternModes::linear);
ledPattern alarmPatternPhase3 = ledPattern(rgbColor(0,0,255), rgbColor(255,255,255), 90000000UL, ledPattern::patternModes::linear);
alarm *testAlarm1 = new alarm(microTime((unsigned int)2017, (byte)8, (byte)18, (byte)6, (byte)0, (byte)0), 86400UL, alarmPatternPhase1);
alarm *testAlarm2 = new alarm(microTime((unsigned int)2017, (byte)8, (byte)18, (byte)6, (byte)2, (byte)0), 86400UL, alarmPatternPhase2);
alarm *testAlarm3 = new alarm(microTime((unsigned int)2017, (byte)8, (byte)18, (byte)6, (byte)4, (byte)0), 86400UL, alarmPatternPhase3);
alarm *testAlarm4 = new alarm(microTime((unsigned int)2017, (byte)8, (byte)15, (byte)19, (byte)1, (byte)0), alarmPatternPhase1);
alarm *testAlarm5 = new alarm(microTime((unsigned int)2017, (byte)8, (byte)15, (byte)19, (byte)4, (byte)0), alarmPatternPhase1);

/* ====================== Settings ==================== */
bool autoTurnOff = false; // always turn lights off when their is no active pattern
bool runningPatterns = true; // continious running patterns
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
ledPattern *pattern = new ledPattern(rgbColor(255, 0, 0), rgbColor(0, 255, 0), 10000000UL, ledPattern::patternModes::linear);

// declaration to prevent undeclared function error
void setAllPixels(rgbColor color, float multiplier);

class serialCommandHandler: public serialCommandDelegate {
  void eventSetTime(microTime newTime) {
    activeTime = newTime;
  }

  void eventSetRunningPatterns(bool newRunningPatterns) {
    runningPatterns = newRunningPatterns;
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
  Serial.begin(9600);
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

  alarms.push_front(*testAlarm1);
  alarms.push_front(*testAlarm2);
  alarms.push_front(*testAlarm3);
  alarms.push_front(*testAlarm4);
  alarms.push_front(*testAlarm5);
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
    updateTime(currentMicros - previousMicros);

    checkAlarms();

    if(pattern->isFinished()) {
      numPatterns++;

      if(runningPatterns) {
        byte newfRed = (byte)random(0,255);
        byte newfBlue = (byte)random(0,255);
        byte newfGreen = (byte)random(0,255);
        unsigned long newTime = random(1000, 10000) * 500UL;
        rgbColor newColor = pattern->getColor();
        rgbColor endColor = rgbColor(newfRed, newfBlue, newfGreen);
  
        // WARNING - we should not delete pattern if it is part of an alarm with an interval!!!
        // delete pattern; // we really can't call delete on a pointer variable that was not created with new;
        // TODO - implement way of determinging weither ledPattern can be removed
  
        if(pattern->isSafeDelete()) {
          delete pattern;
        }
        pattern = new ledPattern(newColor, endColor, newTime, ledPattern::patternModes::linear);
        pattern->setSafeDelete(true);
      }
      else if(autoTurnOff) {
        setAllPixels(rgbColor(0,0,0), 1.0);
      }
    } 
    else {
      pattern->update(currentMicros - previousMicros);
      setAllPixels(pattern->getColor(), 1.0);
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
      if(minuteFlicker) {
        // flicker for numMinuteFlicker amount of times
        for(unsigned int i = 0; i < numMinuteFlicker; i++) { 
          setAllPixels(rgbColor(0,0,0), 1.0);
          delay(FLASH_DELAY);
          setAllPixels(pattern->getColor(), 1.0);
          delay(FLASH_DELAY);
        }
      }
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
        pattern = it->getAlarmPattern();
        pattern->reset();

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
void setAllPixels(rgbColor color, float multiplier = 1.0) 
{
  for (int iPixel = 0; iPixel < LED_COUNT; iPixel++) {
    strip1.setPixelColor(iPixel,
                        (byte)((float)color.getRed() * multiplier),
                        (byte)((float)color.getGreen() * multiplier),
                        (byte)((float)color.getBlue() * multiplier));
  }
  for (int iPixel = 0; iPixel < LED_COUNT; iPixel++) {
    strip2.setPixelColor(iPixel,
                        (byte)((float)color.getRed() * multiplier),
                        (byte)((float)color.getGreen() * multiplier),
                        (byte)((float)color.getBlue() * multiplier));
  }
  for (int iPixel = 0; iPixel < LED_COUNT; iPixel++) {
    strip3.setPixelColor(iPixel,
                        (byte)((float)color.getRed() * multiplier),
                        (byte)((float)color.getGreen() * multiplier),
                        (byte)((float)color.getBlue() * multiplier));
  }
  for (int iPixel = 0; iPixel < LED_COUNT; iPixel++) {
    strip4.setPixelColor(iPixel,
                        (byte)((float)color.getRed() * multiplier),
                        (byte)((float)color.getGreen() * multiplier),
                        (byte)((float)color.getBlue() * multiplier));
  }
  for (int iPixel = 0; iPixel < LED_COUNT; iPixel++) {
    strip5.setPixelColor(iPixel,
                        (byte)((float)color.getRed() * multiplier),
                        (byte)((float)color.getGreen() * multiplier),
                        (byte)((float)color.getBlue() * multiplier));
  }
  
  strip1.show();
  strip2.show();
  strip3.show();
  strip4.show();
  strip5.show();
}



