# GamHa
Harness the amazing power of wifi enabled microcontrollers for <em>true</em> home smart lighting control. Smart home Lighting is an expensive investment and often requires supplying personal information to untrustable third parties, but not any more! With GamHa you can make your own using off the shelf components and the user friendly Arduino IDE.

![Hardware on display](https://pbs.twimg.com/media/DyAJ4LYWoAAbudx.jpg:large)

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
  - [ ] CRC32 checksumming for serial input and output.
  - [ ] Support mutiple indexes being specified at once through serial command interface.
  - [ ] Synchronization functionality and commands.
  - [ ] Desynchronization functionality and commands.
