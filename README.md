![https://travis-ci.com/Dantali0n/IOT_WakeUpLight](https://api.travis-ci.com/Dantali0n/IOT_WakeUpLight.svg?branch=develop)
# GamHa
Harness the amazing power of wifi enabled microcontrollers for <em>true</em> home smart lighting control. Smart home Lighting is an expensive investment and often requires supplying personal information to untrustable third parties, but not any more! With GamHa you can make your own using off the shelf components and the user friendly Arduino IDE.

<p align="center" =>
  <img align="center" src="https://pbs.twimg.com/media/DyAJ4LYWoAAbudx.jpg:small" alt="Hardware on display">
</p>

| Meteor scanner | Fire | Rainbow |
|---|---|---|
| <img src="https://raw.githubusercontent.com/Dantali0n/IOT_WakeUpLight/develop/General/demo/meteor.gif"> | <img  src="https://raw.githubusercontent.com/Dantali0n/IOT_WakeUpLight/develop/General/demo/fire.gif"> |   <img  src="https://raw.githubusercontent.com/Dantali0n/IOT_WakeUpLight/develop/General/demo/rainbow.gif"> |
  
 
  

## Features

* Control up to 11 WS2812B / WS2812 / SK6812 LED strips.
* Control static LEDs brightness in 256 steps.
* 15 different animations including meteor scanner and sunrise.
* 8 different patterns including fade, fire and rainbow.
* Advanced wake up animation for a more natural morning wake up.
* 16.7 million different possible colors.
* 256 step brightness control.
* 65535 step speed control.
* Accurate offline time keeping with minimized drift.

## Requirements
The software consists of three different components each with its own set of requirements. When using the software only two of these requirements are necessary but for testing the additional third component is recommended.

### Mega2560
The first components is the firmware for the Arduino mega 2560. It requires the Arduino IDE and a set of libraries available through the library manager. 

* Arduino IDE 1.6.2 or later
* `ArduinoSTL` library
* `Adafruit GFX Library` library
* `Adafruit NeoPixel` library
* `CRC32` library

If you have the Arduino IDE system wide installed all libraries can be installed with one command.
```bash
arduino --install-library "Adafruit GFX Library,Adafruit NeoPixel,ArduinoSTL,CRC32"
```

### ESP8266
Second component is the ESP8266 firmware similary this required the Arduino IDE and a set libraries. However, a more recent version of the Arduino IDE is required! Additionaly, adding a board support url to the Arduino IDE is required. 

* Arduino IDE 1.6.4 or later
* Added http://arduino.esp8266.com/stable/package_esp8266com_index.json to board support urls

If the Arduino IDE is installed and you have no other board urls configured then the following commands can be execute to automate the installation.
```bash
arduino --pref "boardsmanager.additional.urls=http://arduino.esp8266.com/stable/package_esp8266com_index.json" --save-prefs
arduino --install-boards esp8266:esp8266
```

### Tests
Unit testing is done (not ready) through Catch2 and the setup and execution is managed through cmake.

* Cmake 2.8 or later
* Make
* GCC / Clang

```bash
mkdir build
cd build
cmake ..
make
make test
```

If certain CTest cases fails additional details are available by executing the specific Catch2 unit test. Below is an demonstration of this expansion.
```bash
$ make test
Running tests...
Test project /IOT_WakeUpLight/build
    Start 1: TestOne
1/1 Test #1: TestOne ..........................***Failed    0.00 sec

0% tests passed, 1 tests failed out of 1
```

```bash
$ ./test/testone 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
testone is a Catch v2.6.1 host application.
Run with -? for options

-------------------------------------------------------------------------------
Factorials are computed
-------------------------------------------------------------------------------
/IOT_WakeUpLight/test/test.cpp:4
...............................................................................

/IOT_WakeUpLight/test/test.cpp:8: FAILED:
  REQUIRE( Factorial(10) == 362880 )
with expansion:
  3628800 (0x375f00) == 362880 (0x58980)

===============================================================================
test cases: 1 | 1 failed
assertions: 4 | 3 passed | 1 failed

```

## Todo ESP8266
---
  - [X] Accuratly keep track of time without a RTC or internet connection -> see microTime class. (20 second drift per day)
  - [ ] Update local time.
    - [ ] With a internet connection and the NTP protocol.
  - [ ] Allow users to interface with the device.
    - [ ] By operating as a wireless access point.
    - [ ] By connecting to a nearby wireless access point.
      - [ ] Allow the user to configure and store (EEPROM) the nearby access point the device should look for.
      - [ ] Enable a fallback mode where the device goes back into being a access point if no connection could be established.
    - [ ] With a web control panel and user configurable options.
      - [ ] current local system time
      - [ ] alarms
      - [ ] timers
      - [ ] advanced options
        - [ ] ntp servers
        - [ ] ntp update interval

## Todo Mega 2560
---
  - [X] wake up lighting patterns & colors.
  - [ ] DSP based pattern see [#11](https://github.com/Dantali0n/IOT_WakeUpLight/issues/11).
  - [X] CRC32 checksumming for serial input and output.
  - [ ] Support mutiple indexes being specified at once through serial command interface.
  - [ ] Synchronization functionality and commands.
  - [ ] Desynchronization functionality and commands.
