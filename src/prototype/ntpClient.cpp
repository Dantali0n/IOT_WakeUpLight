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

#include "ntpClient.h"

/**
 * 
 */
ntpClient::ntpClient(IPAddress address, int timeZone) {
  this->ntpServerIP = address;
  this->timeZone = timeZone;
  udpHandler.begin(NTP_PORT);
}

/**
 * 
 */
unsigned long ntpClient::updateTime() {
  while (udpHandler.parsePacket() > 0) ; // discard any previously received packets
  // get a random server from the pool

  sendNTPpacket();
  uint32_t beginWait = millis();

  // loop until 1500 milliseconds pass or millis overflows
  while (millis() - beginWait < 1500 && millis() > beginWait) { 
    int size = udpHandler.parsePacket();

    if (size >= NTP_PACKET_SIZE) {
      udpHandler.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0UL; // return 0 if unable to get the time
}


/** 
 * send an NTP request to the time server at the given address 
 */
void ntpClient::sendNTPpacket()
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  if(this->ntpServerIP != NULL) {
    udpHandler.beginPacket(ntpServerIP, 123); //NTP requests are to port 123
    udpHandler.write(packetBuffer, NTP_PACKET_SIZE);
    udpHandler.endPacket();
  }
}
