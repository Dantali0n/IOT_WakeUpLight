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

#include <ESP8266HTTPClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h> // library to control rgb neopixels
#include "timelib.h" // library to keep track of time 
#include "ntpClient.h" // library to communicate with ntp servers
#include "rgbColor.h" // library to hold a rgb color
#include "springyValue.h" // library to interpolate values as a spring

static const int  BUTTON_PIN = D1;  // hardware pin for the snooze / reset button
static const int LED_PIN = D2;  // hardware pin for the led strip serial data
static const int LED_COUNT =  6;  // amount of leds on the led strip

static const String SRL_INFO_RESET = "IOT wakeuplight reset";
static const String SRL_INFO_IDN_ID = "Last 2 bytes of chip ID: ";

static const String WIFI_NAME_CONCAT = "WakeUpLight_"; // String to prepend to the wifi name
static const int OSCILLATION_TIME = 500; // used to oscillate the rgb led strip
static const int REQUEST_DELAY = 2000000; // minimum time between alarm checks and clock updates in micro seconds


static const int MICROS_TO_SECONDS = 1000000; // constant value to convert micro seconds to seconds

// Default available NTP Servers:
static const String ntpServerNames[] = { 
  "us.pool.ntp.org",
  "time.nist.gov",
  "time-a.timefreq.bldrdoc.gov",
};

//static const char ntpServerName[] = "time-b.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-c.timefreq.bldrdoc.gov";

/* ===================== Constants end here ============================== */

String chipID; // used to store the chip id - used in wifi name if configured as access point

bool buttonBounce = false; // boolean to bounce button so we prevent multiple triggers
unsigned long currentMicros = 0; // used in loop to store micros
unsigned long previousMicros = 0; // used in loop to store micros
String curNtpServer = ntpServerNames[0]; // current ntp timeserver 
int curTimeZone = 1; // timeZone for the current time

// Timer timeKeeper; // our continous time keeper
time_t activeTime; // current system time
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ400); // lights to wake up the user

// declaration to prevent undeclared function error
void setAllPixels(rgbColor color, float multiplier);

/**
 * 
 */
void setup() 
{
  chipID = configureChipID();
  Serial.begin(115200);

  strip.begin();
  strip.setBrightness(255);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  rgbColor red = rgbColor(255,0,0);
  rgbColor purple = rgbColor(0,255,255);

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
  delay(1000);

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
  setTime(0, 0, 0, 0, 0, 0);
  activeTime = now();
  
  // fade to purple to indicate successful boot 
  setAllPixels(purple, 1.0);
  fadeBrightness(purple, 0, 1, (OSCILLATION_TIME*4));

  // run alarmUpdate at the rate specified by REQUEST_DELAY
  // timeKeeper.every(REQUEST_DELAY, alarmUpdate);
}

/**
 * 
 */
void loop() 
{
  //Check for button press and bounce
  if(digitalRead(BUTTON_PIN) == LOW && buttonBounce == false)
  {
    Serial.println("Button press");    
    buttonBounce = true;
    buttonPress();
  }
  else if(buttonBounce == true) {
    Serial.println("Button bounce back");
    buttonBounce = false; // reset bounce condition
  }

  currentMicros = micros();

  // micros overflow reset condition -> this happens about once every 70 minutes
  // this will mean we lose some time data because of the missing microseconds around the overflow condition.
  if(previousMicros > currentMicros) {
    Serial.println("Micros overflow");
    previousMicros = currentMicros;
  }
  
  // timeKeeper.update();
  if(currentMicros - previousMicros > REQUEST_DELAY) {
    updateTime(currentMicros - previousMicros);
    checkAlarms();
    previousMicros = micros();
  }
}

/**
 * Update the system time using the Time library and the progress in microseconds
 * @Param timeBetweenUpdate amount of microseconds to update the current time
 */
void updateTime(unsigned long timeBetweenUpdate) {
  int timeUpdate = timeBetweenUpdate / MICROS_TO_SECONDS;
  Serial.println(timeUpdate);
  activeTime += timeUpdate;
  Serial.print(hour(activeTime));
  Serial.print(":");
  Serial.print(minute(activeTime));
  Serial.print(":");
  Serial.println(second(activeTime));
}

/**
 * 
 */
void checkAlarms() {
  
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
  
  oscillate(random(0,100), (float)random(0,1000)/1000, rgbColor((int)random(0,255), (int)random(0,255), (int)random(0,255)));
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


