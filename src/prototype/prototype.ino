#include <ESP8266HTTPClient.h>
#include <Adafruit_NeoPixel.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include "rgbColor.h"
#include "springyValue.h"

#define BUTTON_PIN     D1  // hardware pin for the snooze / reset button
#define LED_PIN        D2  // hardware pin for the led strip serial data
const int LED_COUNT =  6;  // amount of leds on the led strip

const String WIFI_NAME_CONCAT = "WakeUpLight_"; // String to prepend to the wifi name
const int OSCILLATION_TIME = 500; // used to oscillate the rgb led strip
const int REQUEST_DELAY = 2000; // minimum time between alarm checks

int oldTime = 0; // used in loop to store millis
String chipID; // used to store the chip id

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ400);

// declaration to prevent undeclared function error
void setAllPixels(uint8_t r, uint8_t g, uint8_t b, float multiplier);

/**
 * 
 */
void setup() 
{
  configureChipID();
  strip.begin();
  strip.setBrightness(255);
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  int counter = 0;
  while(digitalRead(BUTTON_PIN) == LOW)
  {
    counter++;
    delay(10);

    if(counter > 500)
    {
      Serial.println("Remove all wifi settings!");
      setAllPixels(255,0,0,1.0);
      fadeBrightness(255,0,0,1.0);
      ESP.reset();
    }
  }
  delay(1000);

  Serial.println();
  Serial.print("Last 2 bytes of chip ID: ");
  Serial.println(chipID);

  String wifiNameConcat = WIFI_NAME_CONCAT + chipID;
  char wifiName[19] = {};
  wifiNameConcat.toCharArray(wifiName, 19);
  
  setAllPixels(0,255,255,1.0);
  fadeBrightness(0,255,255,1.0);
}

/**
 * 
 */
void loop() 
{
  //Check for button press
  if(digitalRead(BUTTON_PIN) == LOW)
  {    
    buttonPress();
    delay(250);
  }

  //Every requestDelay, send a request to the server
  if(millis() > oldTime + REQUEST_DELAY)
  {
    checkAlarms();
    oldTime = millis();
  }
}

/**
 * 
 */
void checkAlarms() {
  
}

/**
 * Called when the button is pressed.
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
 * 
 * @Param r
 * @Param g
 * @Param b
 * @Param mulitplier
 */
void setAllPixels(uint8_t r, uint8_t g, uint8_t b, float multiplier = 1.0) 
{
  for (int iPixel = 0; iPixel < LED_COUNT; iPixel++)
    strip.setPixelColor(iPixel,
                        (byte)((float)r * multiplier),
                        (byte)((float)g * multiplier),
                        (byte)((float)b * multiplier));
  strip.show();
}

/*
 * Starts an oscillation movement in both the LED strip
 * @Param springConstant
 * @Param dampConstant
 * @Param c
 */
void oscillate(float springConstant, float dampConstant, rgbColor c)
{
  SpringyValue spring;
  
  byte red = c.getRed();
  byte green = c.getGreen();
  byte blue = c.getBlue();
  
  spring.c = springConstant;
  spring.k = dampConstant / 100;
  spring.perturb(255);

  //Start oscillating
  for(int i = 0; i < OSCILLATION_TIME; i++)
  {
    spring.update(0.01);
    setAllPixels(red, green, blue, abs(spring.x) / 255.0);

    //Check for button press and cancel if pressed
//    if(digitalRead(BUTTON_PIN) == LOW)
//    {
//      //Fade the current color out
//      fadeBrightness(red, green, blue, abs(spring.x) / 255.0);
//      return;
//    }

    delay(10);
  }
  fadeBrightness(red, green, blue, abs(spring.x) / 255.0);
}

/*
 * Grabs the current RGB values and current brightness and fades the colors to black
 * @Param r
 * @Param g
 * @Param b
 * @Param currentBrightness
 */
void fadeBrightness(uint8_t r, uint8_t g, uint8_t b, float currentBrightness)
{
    for(float j = currentBrightness; j > 0.0; j-=0.01)
    {
          setAllPixels(r, g, b, j);
          delay(20);
    }
    hideColor();
}

/**
 * 
 */
void hideColor() 
{
  colorWipe(strip.Color(0, 0, 0));
}

/**
 * 
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
void configureChipID()
{
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
 
  chipID = u + l;
  chipID.toUpperCase();
  chipID.toCharArray(chipIdArray, 5);
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


