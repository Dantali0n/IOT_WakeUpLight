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

#ifndef _Perm_Config_h
#ifdef __cplusplus
#define _Perm_Config_h

#include <Arduino.h>
#include <EEPROM.h>
#include "wultypedef.h"

class permConfig {
  private:
    const static int CREDENTIAL_OFFSET = 68;
    static char ssid[32];
    static char password[32];
  public:
  	static void loadCredentials();
    static void saveCredentials(String ssid, String password);
    static String getSsid();
    static String getPassword();
};

#endif /* __cplusplus */
#endif /* _Perm_Config_h */
