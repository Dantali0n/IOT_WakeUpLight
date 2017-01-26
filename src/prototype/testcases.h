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

#ifndef _Wul_Test_Cases_h
#ifdef __cplusplus
#define _Wul_Test_Cases_h

#include <Arduino.h>
#include "wulTypedef.h"
#include "microTime.h"

class wulTestCases {
  	private:
  		HardwareSerial *serialRef; // used to product output

  		void testWrapper(); 

  		void testMicroTime();
  		void testNtpClient();
  	public:
  		wulTestCases(HardwareSerial *serialRef);
};

#endif /* __cplusplus */
#endif /* _Wul_Test_Cases_h */
