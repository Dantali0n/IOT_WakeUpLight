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

// Not available on ESP8266
// #include <ArduinoSTL.h>
// #include <list>

#include <ESP8266HTTPClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

#include <Adafruit_NeoPixel.h> // library to control rgb neopixels

#include "alarm.h"
#include "microtime.h" // library to keep track of time 
#include "ntpclient.h" // library to communicate with ntp servers
#include "rgbcolor.h" // library to hold a rgb color
#include "springyvalue.h" // library to interpolate values as a spring
#include "testcases.h"
#include "ledpattern.h"
#include "permconfig.h"
#include "serialcommand.h"

/* ========================== */
/* === NOTE ABOUT PROGMEM === */
/* ========================== */
/* PROGMEM is not available on the ESP82 but for all we know it could be ported eventually
 * using progmem while it is not available on the ESP82 will result in a segfault.
 * The esp will terminate the program and reset.
 */

static const int  BUTTON_PIN = D1;  // hardware pin for the snooze / reset button
static const int LED_PIN = D2;  // hardware pin for the led strip serial data
static const int LED_COUNT =  6;  // amount of leds on the led strip

static const String SRL_INFO_RESET = "IOT wakeuplight reset";
static const String SRL_INFO_IDN_ID = "Last 2 bytes of chip ID: ";

static const String WIFI_NAME_CONCAT = "WakeUpLight_"; // String to prepend to the wifi name
static const int FLASH_DELAY = 125;
static const int OSCILLATION_TIME = 500; // used to oscillate the rgb led strip
static const unsigned long REQUEST_DELAY = 3900; // minimum time between alarm checks and clock updates in micro seconds - 3900 microseconds = 255 fps
// static const unsigned long REQUEST_DELAY = 500; // minimum time between alarm checks and clock updates in micro seconds - 500 microseconds = 2000 fps

static const unsigned long MICROS_TO_SECONDS = 1000000; // constant value to convert micro seconds to seconds

const static rgbColor SITAHS_FAVORITE_MINT_GREEN = rgbColor(0, 255, 75);
const static rgbColor MARKS_FAVORITE_PINK = rgbColor(255, 100, 83);
const static rgbColor RICOS_FAVORITE_BLUE = rgbColor(0, 72, 251);

// Default available NTP Servers:
// pool.ntp.org servers determine closest available server automaticly. 
static const String ntpServerNames[] = { 
  "0.pool.ntp.org",
  "1.pool.ntp.org",
  "2.pool.ntp.org",
  "3.pool.ntp.org",
  "time-a.timefreq.bldrdoc.gov",
  "time-b.timefreq.bldrdoc.gov",
  "time-c.timefreq.bldrdoc.gov",
  "time.nist.gov"
};

//static const char ntpServerName[] = "time-c.timefreq.bldrdoc.gov";

/* ===================== Constants end here ============================== */

String chipID; // used to store the chip id - used in wifi name if configured as access point

bool buttonBounce = false; // boolean to bounce button so we prevent multiple triggers
bool hasNotFlashed = false; // boolean to flash when the pattern is complete
unsigned long currentMicros = 0; // used in loop to store micros
unsigned long previousMicros = 0; // used in loop to store micros
String curNtpServer = ntpServerNames[0]; // current ntp timeserver 
int curTimeZone = 1; // timeZone for the current time
int lastMinute = 0;
unsigned int averageFramerate = 0;
unsigned int numPatterns = 0;

// std::list<alarm> alarms = std::list<alarm>();
ledPattern alarmPattern = ledPattern(MARKS_FAVORITE_PINK, RICOS_FAVORITE_BLUE, 90000000UL, ledPattern::patternModes::linear);
alarm testAlarm1 = alarm(microTime(2017, 8, 15, 19, 1, 0), 180, alarmPattern);
alarm testAlarm2 = alarm(microTime(2017, 8, 15, 19, 0, 0), alarmPattern);

/* ====================== Settings ==================== */
bool autoTurnOff = false; // always turn lights off when their is no active pattern
bool runningPatterns = true; // continious running patterns
bool minuteFlicker = true; // boolean to enable or disable flashing every minute
unsigned int numMinuteFlicker = 3; // amount of times to flicker every minute

/* ====================== Instances ====================== */

// Timer timeKeeper; // our continous time keeper
// microTime activeTime = microTime(999, 12, 30, 23 , 59, 59); // current system time
microTime activeTime = microTime(2010, 8, 18, 15 , 00, 00); // current system time
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ400); // lights to wake up the user
ledPattern *pattern = new ledPattern(rgbColor(250, 0, 0), rgbColor(0, 250, 0), 10000000UL, ledPattern::patternModes::linear);

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

/**
 * 
 */
void setup() 
{

  // disable wifi
  WiFi.forceSleepBegin(); 

  chipID = configureChipID();
  Serial.begin(230400);

  //wulTestCases wtc = wulTestCases(&Serial);

  strip.begin();
  strip.setBrightness(255);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  rgbColor red = rgbColor(254,0,0);
  rgbColor purple = rgbColor(254,0,254);

  // Reset condition if the button is held down on power up
  int counter = 0;
  while(digitalRead(BUTTON_PIN) == LOW)
  {
    counter++;
    delay(10);

    if(counter > 500)
    {
      Serial.println(SRL_INFO_RESET);
      // fade red to indicate reset
      setAllPixels(red, 1.0);
      fadeBrightness(red, 255, 0, (OSCILLATION_TIME*2));
      ESP.reset();
    }
  }

  permConfig::saveCredentials("Fam.Lukken","D3rpPr%veM#Wrong");

  // alarms.push_front(testAlarm1);
  // alarms.push_front(testAlarm2);
  
  delay(2000);

  Serial.println();
  Serial.print(SRL_INFO_IDN_ID);
  Serial.println(chipID);

  // setup wifi name based on chip id
  String wifiNameConcat = WIFI_NAME_CONCAT + chipID;
  char wifiName[19] = {};
  wifiNameConcat.toCharArray(wifiName, 19);

  // iniate time
  // TODO: read last stored time from EEPROM 
  // TODO: If wifi connected use ntp to configure initial time
  
  // fade to purple to indicate successful boot 
  setAllPixels(purple, 1.0);
  fadeBrightness(purple, 0, 1, (OSCILLATION_TIME*4));

  permConfig::loadCredentials();

  // run alarmUpdate at the rate specified by REQUEST_DELAY
  // timeKeeper.every(REQUEST_DELAY, alarmUpdate);
}

/**
 * 
 */
void loop() 
{

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
      
      byte newfRed = (byte)random(0,255);
      byte newfBlue = (byte)random(0,255);
      byte newfGreen = (byte)random(0,255);
      unsigned long newTime = random(1000, 10000) * 500UL;
      rgbColor newColor = pattern->getColor();
      rgbColor endColor = rgbColor(newfRed, newfBlue, newfGreen);

      delete pattern;
      pattern = new ledPattern(newColor, endColor, newTime, ledPattern::patternModes::linear);
    } else {
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
        setAllPixels(rgbColor(0,0,0), 1.0);
        delay(FLASH_DELAY);
        setAllPixels(pattern->getColor(), 1.0);
        delay(FLASH_DELAY);
        setAllPixels(rgbColor(0,0,0), 1.0);
        delay(FLASH_DELAY);
        setAllPixels(pattern->getColor(), 1.0);
        delay(FLASH_DELAY);
        setAllPixels(rgbColor(0,0,0), 1.0);
        delay(FLASH_DELAY);
        setAllPixels(pattern->getColor(), 1.0);
      }
    }

    averageFramerate++;
    previousMicros = currentMicros;
  }
}
 
//void loop() 
//{
//  // Check for button press and bounce
//  // make sure a button press only calls buttonPress() once
//  if(digitalRead(BUTTON_PIN) == LOW && buttonBounce == false)
//  {
//    Serial.println("Button press");    
//    buttonBounce = true;
//  }
//  else if(buttonBounce == true && digitalRead(BUTTON_PIN) == HIGH) {
//    Serial.println("Button bounce back");
//    buttonPress();
//    buttonBounce = false; // reset bounce condition
//  }
//
//  currentMicros = micros();
//
//  // micros overflow reset condition -> this happens about once every 70 minutes
//  // this will mean we lose some time data because of the missing microseconds around the overflow condition.
//  // In order to minimize the amount of microseconds lost keep REQUEST_DELAY as small as possible.
//  if(previousMicros > currentMicros) {
//    Serial.println("Micros overflow");
//    previousMicros = currentMicros;
//  }
//  
//  if(currentMicros - previousMicros > REQUEST_DELAY) {
//    updateTime(currentMicros - previousMicros);
//    checkAlarms();
//
//    if(!pattern->isFinished()) {
//      hasNotFlashed = true;
//      Serial.print(pattern->getColor().getRed());
//      Serial.print(',');
//      Serial.print(pattern->getColor().getGreen());
//      Serial.print(',');
//      Serial.print(pattern->getColor().getBlue());
//      Serial.print(',');
//      Serial.print(pattern->getCurrentDuration());
//      Serial.print(',');
//      Serial.println(pattern->getFinalDuration());
//      pattern->update(currentMicros - previousMicros);
//      setAllPixels(pattern->getColor(), 1.0);
//    }
//    // flash shortly twice to indicate the pattern has finished
//    else if(hasNotFlashed) {
//      hasNotFlashed = false;
//      setAllPixels(rgbColor(0,0,0), 1.0);
//      delay(FLASH_DELAY);
//      setAllPixels(pattern->getColor(), 1.0);
//      delay(FLASH_DELAY);
//      setAllPixels(rgbColor(0,0,0), 1.0);
//      delay(FLASH_DELAY);
//      setAllPixels(pattern->getColor(), 1.0);
//    }
//    
//    previousMicros = currentMicros;
//  }
//}

/**
 * Update the system time using the Time library and the progress in microseconds
 * @Param timeBetweenUpdate amount of microseconds to update the current time
 */
void updateTime(unsigned long timeBetweenUpdate) {
  activeTime.update(timeBetweenUpdate);
//  Serial.println(timeBetweenUpdate);
//  Serial.print(activeTime.year());
//  Serial.print("-");
//  Serial.print(activeTime.month());
//  Serial.print("-");
//  Serial.print(activeTime.day());
//  Serial.print(" ");
//  Serial.print(activeTime.hour());
//  Serial.print(":");
//  Serial.print(activeTime.minute());
//  Serial.print(":");
//  Serial.print(activeTime.second());
//  Serial.print(":");
//  Serial.println(activeTime.microSecond());
}

/**
 * 
 */
void checkAlarms() {

// std::list unavailable on ESP8266 :(
//  for (std::list<alarm>::iterator it = alarms.begin(); it != alarms.end(); it++) {
//    if(it->check(activeTime)) {
//      Serial.println("Sound the alarm");
//      pattern = it->getAlarmPattern();
//      pattern->reset();
//
//      // WARNING this could cause our ledPattern to be removed from existence since we pass it as an pointer
//      // In this case it does not since the alarm is created during compile time and later assigned to the list.
//      // Removing it from the list will not remove the alarm from memory and neither the ledPattern
//      if(!it->getInterval()) {
//        Serial.println("Erase the alarm");
//        alarms.erase(it);
//      }
//    }
//  }
}

/**
 * Called when the button is pressed.
 * Used to disable triggered alarms
 */
void buttonPress() {
//  int value = 0;
//  int red = random(0,255);
//  int green = random(0,255);
//  int blue = random(0,255);
//  
//  value += (red << 16) & 0xff;
//  value += (green << 8) & 0xff;
//  value += blue & 0xff;
  
//oscillate(random(0,100), (float)random(0,1000)/1000, rgbColor((int)random(0,255), (int)random(0,255), (int)random(0,255)));

  delete pattern;
//  byte newsRed = 0;
//  byte newsBlue = 0;
//  byte newsGreen = 0;
//  byte newfRed = 255;
//  byte newfBlue = 255;
//  byte newfGreen = 255;
//  unsigned long newTime = 10000000UL;

  byte newsRed = (byte)random(0,255);
  byte newsBlue = (byte)random(0,255);
  byte newsGreen = (byte)random(0,255);
  byte newfRed = (byte)random(0,255);
  byte newfBlue = (byte)random(0,255);
  byte newfGreen = (byte)random(0,255);
  unsigned long newTime = random(1000, 10000) * 500UL;
  rgbColor newColor = rgbColor(newsRed, newsBlue, newsGreen);
  rgbColor endColor = rgbColor(newfRed, newfBlue, newfGreen);
  
  Serial.print(newColor.getRed());
  Serial.print(',');
  Serial.print(newColor.getGreen());
  Serial.print(',');
  Serial.print(newColor.getBlue());
  Serial.print(',');
  Serial.print(endColor.getRed());
  Serial.print(',');
  Serial.print(endColor.getGreen());
  Serial.print(',');
  Serial.print(endColor.getBlue());
  Serial.print(',');
  Serial.println(newTime);
  
  pattern = new ledPattern(newColor, endColor, newTime, ledPattern::patternModes::linear);
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
  for (int iPixel = 0; iPixel < LED_COUNT; iPixel++)
    strip.setPixelColor(iPixel,
                        (byte)((float)color.getRed() * multiplier),
                        (byte)((float)color.getGreen() * multiplier),
                        (byte)((float)color.getBlue() * multiplier));
  strip.show();
}

/*
 * Starts an oscillation movement in both the LED strip for a specific color
 * @Param springConstant how springy the oscillation is
 * @Param dampConstant how damped the oscillation is
 * @Param color rgbColor instance containing all the base colors of the color
 */
void oscillate(float springConstant, float dampConstant, rgbColor color)
{
  SpringyValue spring;
  
  spring.c = springConstant;
  spring.k = dampConstant / 100;
  spring.perturb(255);

  //Start oscillating
  for(int i = 0; i < OSCILLATION_TIME; i++)
  {
    spring.update(0.01);
    setAllPixels(color, abs(spring.x) / 255.0);

    //Check for button press and cancel if pressed
//    if(digitalRead(BUTTON_PIN) == LOW)
//    {
//      //Fade the current color out
//      fadeBrightness(color, abs(spring.x) / 255.0);
//      return;
//    }

    delay(10);
  }
  fadeBrightness(color, abs(spring.x) / 255.0, 0, 500);
}

/*
 * Grabs the current RGB values and current brightness and fades the colors to a given value
 * @Param color rgbColor class containing all three base color parameters
 * @Param currentBrightness how bright the current color is
 */
void fadeBrightness(rgbColor color, float currentBrightness, float value, int animTime)
{
    float toMove = value - currentBrightness;
    float stepSize = toMove / (animTime / 20);
    
//    Serial.print("From brightness: ");
//    Serial.print(currentBrightness);
//    Serial.print(" To: ");
//    Serial.println(value);
//    Serial.print("toMove: ");
//    Serial.print(toMove);
//    Serial.print(" stepSize: ");
//    Serial.println(stepSize);

    // brighten
    if(toMove > 0) {
      for(float j = currentBrightness; j < value; j+=stepSize)
      {
//            Serial.print("stepping: ");
//            Serial.println(j);
            setAllPixels(color, j);
            delay(20);
      }
    }
    // darken
    else {
      for(float j = currentBrightness; j > value; j+=stepSize)
      {
//            Serial.print("stepping: ");
//            Serial.println(j);
            setAllPixels(color, j);
            delay(20);
      }
    }
    
    hideColor();
}

/**
 * turn the entire led strip off
 */
void hideColor() 
{
  colorWipe(strip.Color(0, 0, 0));
}

/**
 * Set the entire led strip to the specified color
 */
void colorWipe(uint32_t c) 
{
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

/**
 * Get the ESP chip id
 */
String configureChipID()
{
  String tmpID;
  char chipIdArray[5] = {};
  uint32_t id = ESP.getChipId();
  byte lower = id & 0xff;
  byte upper = (id >> 8) & 0xff;

  String l = "";
  String u = "";

  if(lower < 10)
  {
    l = "0" + String(lower, HEX);
  }
  else
  {
    l = String(lower, HEX);
  }

  if(upper < 10)
  {
    u = "0" + String(upper, HEX);
  }
  else
  {
    u = String(upper, HEX);
  }
 
  tmpID = u + l;
  tmpID.toUpperCase();
  tmpID.toCharArray(chipIdArray, 5);
  return tmpID;
}

//void sendButtonPress()
//{
//    Serial.println("Sending button press to server");
//    HTTPClient http;
//    http.begin(webURL + "/api.php?t=sqi&d=" + chipID);
//    uint16_t httpCode = http.GET();      
//    http.end();
//}

//void requestMessage()
//{
//  Serial.println("Sending request to server");
//  hideColor();
//      
//  HTTPClient http;
//  http.begin(webURL + "/api.php?t=gqi&d=" + chipID + "&v=2");
//  uint16_t httpCode = http.GET();
//
//  if (httpCode == 200) 
//  {
//    String response;
//    response = http.getString();
//    //Serial.println(response);
//
//    if(response == "-1")
//    {
//      Serial.println("There are no messages waiting in the queue");
//    }
//    else
//    {
//      //Get the indexes of some commas, will be used to split strings
//      int firstComma = response.indexOf(',');
//      int secondComma = response.indexOf(',', firstComma+1);
//      int thirdComma = response.indexOf(',', secondComma+1);
//      
//      //Parse data as strings
//      String hexColor = response.substring(0,7);
//      String springConstant = response.substring(firstComma+1,secondComma);
//      String dampConstant = response.substring(secondComma+1,thirdComma);;
//      String message = response.substring(thirdComma+1,response.length());;
//
//      Serial.println("Message received from server: \n");
//      Serial.println("Hex color received: " + hexColor);
//      Serial.println("Spring constant received: " + springConstant);
//      Serial.println("Damp constant received: " + dampConstant);
//      Serial.println("Message received: " + message);
//
//      //Extract the hex color and fade the led strip
//      int number = (int) strtol( &response[1], NULL, 16);
//      oscillate(springConstant.toFloat(), dampConstant.toFloat(), number);
//    }
//  }
//  else
//  {
//    ESP.reset(); 
//  }
//    
//  http.end();
//}


