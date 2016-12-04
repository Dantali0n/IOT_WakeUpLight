#include <ESP8266HTTPClient.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h> 

class SpringyValue 
{
  public:
    float x = 0, v = 0, a = 0, // x (value), v (velocity) and a (acceleration) define the state of the system
          o = 0, // o defines the temporary spring offset w.r.t. its resting position
          c = 20.0, k = 1.5, m = 1.0; // c (spring constant), k (damping constant) and m (mass) define the behavior of the system

    // Perturb the system to change the "length" of the spring temporarlily
    void perturb(float offset) {
      this->o = offset;
    }

    // Call "update" every now and then to update the system
    // parameter dt specifies the elapsed time since the last update
    void update(float dt) {
      a = (-c * x - k * v ) / m;
      v += a * dt;
      x += v * dt + o;
      o = 0; // a spring offet only takes one frame
    }
};


#define BUTTON_PIN  D1
#define PIN         D2   
#define LED_COUNT    6

#define fadeInDelay  5
#define fadeOutDelay 8

#define requestDelay 2000

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ400);
Servo myServo;

int oldTime = 0;
int oscillationTime = 500;
String chipID;
char chipIdArray[5] = {};
String webURL = "http://thingscon16.futuretechnologies.nl";

void setAllPixels(uint8_t r, uint8_t g, uint8_t b, float multiplier);

void setup() 
{
  configureChipID();
  strip.begin();
  strip.setBrightness(255);
  WiFiManager wifiManager;
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  int counter = 0;
  while(digitalRead(BUTTON_PIN) == LOW)
  {
    counter++;
    delay(10);

    if(counter > 500)
    {
      wifiManager.resetSettings();
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

  String wifiNameConcat = "ConnectiKlaas_" + chipID;
  char wifiName[19] = {};
  wifiNameConcat.toCharArray(wifiName, 19);
  
  setAllPixels(0,255,255,1.0);
  wifiManager.autoConnect(wifiName);
  fadeBrightness(0,255,255,1.0);
  myServo.attach(D7);
}

void setAllPixels(uint8_t r, uint8_t g, uint8_t b, float multiplier = 1.0) 
{
  for (int iPixel = 0; iPixel < LED_COUNT; iPixel++)
    strip.setPixelColor(iPixel,
                        (byte)((float)r * multiplier),
                        (byte)((float)g * multiplier),
                        (byte)((float)b * multiplier));
  strip.show();
}

//This method starts an oscillation movement in both the LED and servo
void oscillate(float springConstant, float dampConstant, int c)
{
  SpringyValue spring;
  
  byte red = (c >> 16) & 0xff;
  byte green = (c >> 8) & 0xff;
  byte blue = c & 0xff;
  
  spring.c = springConstant;
  spring.k = dampConstant / 100;
  spring.perturb(255);

  //Start oscillating
  for(int i = 0; i < oscillationTime; i++)
  {
    spring.update(0.01);
    setAllPixels(red, green, blue, abs(spring.x) / 255.0);
    myServo.write(90 + spring.x/4); 

    //Check for button press
    if(digitalRead(BUTTON_PIN) == LOW)
    {
      //Fade the current color out
      fadeBrightness(red, green, blue, abs(spring.x) / 255.0);
      return;
    }
    delay(10);
  }
  fadeBrightness(red, green, blue, abs(spring.x) / 255.0);
}

//This method grabs the current RGB values and current brightness and fades the colors to black
void fadeBrightness(uint8_t r, uint8_t g, uint8_t b, float currentBrightness)
{
    for(float j = currentBrightness; j > 0.0; j-=0.01)
    {
          setAllPixels(r, g, b, j);
          delay(20);
    }
    hideColor();
}

void loop() 
{
  //Check for button press
  if(digitalRead(BUTTON_PIN) == LOW)
  {    
    sendButtonPress();
    delay(250);
  }

  //Every requestDelay, send a request to the server
  if(millis() > oldTime + requestDelay)
  {
    requestMessage();
    oldTime = millis();
  }
}

void sendButtonPress()
{
    Serial.println("Sending button press to server");
    HTTPClient http;
    http.begin(webURL + "/api.php?t=sqi&d=" + chipID);
    uint16_t httpCode = http.GET();      
    http.end();
}

void requestMessage()
{
  Serial.println("Sending request to server");
  hideColor();
      
  HTTPClient http;
  http.begin(webURL + "/api.php?t=gqi&d=" + chipID + "&v=2");
  uint16_t httpCode = http.GET();

  if (httpCode == 200) 
  {
    String response;
    response = http.getString();
    //Serial.println(response);

    if(response == "-1")
    {
      Serial.println("There are no messages waiting in the queue");
    }
    else
    {
      //Get the indexes of some commas, will be used to split strings
      int firstComma = response.indexOf(',');
      int secondComma = response.indexOf(',', firstComma+1);
      int thirdComma = response.indexOf(',', secondComma+1);
      
      //Parse data as strings
      String hexColor = response.substring(0,7);
      String springConstant = response.substring(firstComma+1,secondComma);
      String dampConstant = response.substring(secondComma+1,thirdComma);;
      String message = response.substring(thirdComma+1,response.length());;

      Serial.println("Message received from server: \n");
      Serial.println("Hex color received: " + hexColor);
      Serial.println("Spring constant received: " + springConstant);
      Serial.println("Damp constant received: " + dampConstant);
      Serial.println("Message received: " + message);

      //Extract the hex color and fade the led strip
      int number = (int) strtol( &response[1], NULL, 16);
      oscillate(springConstant.toFloat(), dampConstant.toFloat(), number);
    }
  }
  else
  {
    ESP.reset(); 
  }
    
  http.end();
}

void hideColor() 
{
  colorWipe(strip.Color(0, 0, 0));
}

void colorWipe(uint32_t c) 
{
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

void configureChipID()
{
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


