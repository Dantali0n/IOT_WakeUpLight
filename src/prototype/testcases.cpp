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

#include "testcases.h"

/**
 *
 */
wulTestCases::wulTestCases(HardwareSerial *serialRef) {
  this->serialRef = serialRef;
	testWrapper();
}


/**
 * Wrapper to save some program space when executing tests using the multiple constructors
 */
void wulTestCases::testWrapper() {
  serialRef->println("Testing begin...");
	testMicroTime();
	testNtpClient();
}

/**
 *
 */
void wulTestCases::testMicroTime() {
	serialRef->println("Testing MicroTime:");
  microTime mt1 = microTime(1994, 1, 1, 0, 0, 0);
  microTime mt2 = microTime(1995, 1, 1, 0, 0, 0);
  if(mt1 < mt2) {
    serialRef->println("Assert year 1994 < year 1995 : SUCCESS");
  }
  else {
    serialRef->println("Assert year 1994 < year 1995 : FAILED");
  }
}

/**
 *
 */
void wulTestCases::testNtpClient() {
	serialRef->println("Testing NtpClient:");
}


