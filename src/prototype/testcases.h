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
#include "wultypedef.h"
#include "microtime.h"
#include "ledpattern.h"

class wulTestCases {
  	private:
  		HardwareSerial *serialRef; // used to product output

  		void testWrapper(); 

  		void testMicroTime();
  		void testNtpClient();
      void testLedPattern();

      void haltFurtherExecution();

      static const String MES_SPACE;
      static const String MES_ASSRT;
      static const String MES_RES;
      static const String MES_TESTING;
      static const String MES_EXPECT;

      static const String MES_MICROTIME;
      static const String MES_NTPCLIENT;
      static const String MES_LEDPATTERN;
      
      static const String MES_OPR_ABOVE;
      static const String MES_OPR_BELOW;
      static const String MES_OPR_ABV_EQ;
      static const String MES_OPR_BELOW_EQ;
      static const String MES_OPR_NOT_EQUAL;
      static const String MES_OPR_EQUAL;
      
      static const String MES_CONSTRAINT;
      static const String MES_INCREASE;
      static const String MES_DECREASE;
      
      static const String MES_SUC;
      static const String MES_FAIL;
      
      static const String MES_YEAR;
      static const String MES_MONTH;
      static const String MES_DAY;
      static const String MES_HOUR;
      static const String MES_MINUTE;
      static const String MES_SECOND;
      
  	public:
  		wulTestCases(HardwareSerial *serialRef);
};

#endif /* __cplusplus */
#endif /* _Wul_Test_Cases_h */
