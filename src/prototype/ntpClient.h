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

#include <Arduino.h>
#include <WiFiUdp.h>
#include "timelib.h"

typedef unsigned char byte;

class ntpClient {
  private:
    IPAddress ntpServerIP;
    WiFiUDP udpHandler;

    static const unsigned int NTP_PORT = 8888; // port for WiFiUDP 
    static const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message

    int timeZone; // system timeZone
    byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

    void sendNTPpacket();
  public:
    ntpClient(IPAddress address, int timeZone);
    bool updateTime();
};

