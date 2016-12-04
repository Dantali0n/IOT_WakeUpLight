# IOT-wakeuplight

Harness the amazing power of wifi enabled microcontrollers such as the ESP8266 to create a web enabled wakeuplight.

## Requirements
* Arduino IDE 1.6.9 or higher
* ESP board manager
* Adafruit neopixel library
* WifiManager library

**ESP board manager**

Start by opening the arduino IDE
under the tab files goto ```files -> preferences -> additional boards manager urls``` and add a line with the following url ```http://arduino.esp8266.com/stable/package_esp8266com_index.json```

The ESP development boards and there configuration properties can now be found under ```tools -> board -> Boards manager``` be sure to download and install ```esp8266``` by ESP8266Community.

**Adafruit neopixel**

Goto ```Sketch -> include libraries -> manage libraries``` and search for ```neopixel``` be sure to install the library with both adafruit and neopixel in the name.

**WifiManager**

Be sure to have added the esp8266 board manager before searching for this library otherwise it will bot be available. Once more goto ```sketch -> include libraries -> manage libraries``` and this time search for ```wifimanager``` install the library made by ```tzapu``` 

Congratulations you now have all the requirements installed in order to compile this project!
