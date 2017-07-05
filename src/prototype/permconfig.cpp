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

#include "permconfig.h"

char permConfig::ssid[] = {};
char permConfig::password[] = {};

void permConfig::loadCredentials() {
  EEPROM.begin(512);
  EEPROM.get(0, ssid);
  EEPROM.get(0+sizeof(ssid), password);
  char ok[2+1];
  EEPROM.get(0+sizeof(ssid)+sizeof(password), ok);
  EEPROM.end();
  if (String(ok) != String("OK")) {
    Serial.println("EEPROM does not have OK param present!");
    //ssid[0] = 0;
    //password[0] = 0;
  }
  Serial.println("Recovered credentials:");
  Serial.println(ssid);
  Serial.println(strlen(password)>0?"********":"<no password>");
}

void permConfig::saveCredentials(String ssid, String password) {

  if(ssid.length() > 32 || password.length() > 32) {
    Serial.println("permConfig::saveCredentials - credentials to long!");
    return;
  }
  
  EEPROM.begin(512);
  EEPROM.put(0, ssid);
  Serial.print("Put: ");
  Serial.print(ssid);
  Serial.print(",of size: ");
  Serial.println(sizeof(ssid));
  EEPROM.put(0+sizeof(ssid), password);
  char ok[2+1] = "OK";
  EEPROM.put(0+sizeof(ssid)+sizeof(password), ok);
  EEPROM.commit();
  EEPROM.end();
}

String permConfig::getSsid() {
  return String(ssid);
}


String permConfig::getPassword() {
  return String(password);
}

