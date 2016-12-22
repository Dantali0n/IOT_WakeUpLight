# IOT-wakeuplight

Harness the amazing power of wifi enabled microcontrollers such as the ESP8266 to create a web enabled wakeuplight.

## Todo
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
      - [ ] wake up lighting patterns & colors
      - [ ] timers
      - [ ] advanced options
        - [ ] ntp servers
        - [ ] ntp update interval
