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

const String wulTestCases::MES_SPACE = " ";
const String wulTestCases::MES_ASSRT = "Assert ";
const String wulTestCases::MES_RES = " : ";
const String wulTestCases::MES_TESTING = "testing ";
const String wulTestCases::MES_EXPECT = "expect ";

const String wulTestCases::MES_MICROTIME = "microTime";
const String wulTestCases::MES_NTPCLIENT = "ntpClient";
const String wulTestCases::MES_LEDPATTERN = "ledPattern";

const String wulTestCases::MES_OPR_ABOVE = " > ";
const String wulTestCases::MES_OPR_BELOW = " < ";
const String wulTestCases::MES_OPR_ABV_EQ = " >= ";
const String wulTestCases::MES_OPR_BELOW_EQ = " <= ";
const String wulTestCases::MES_OPR_EQUAL = " == ";
const String wulTestCases::MES_OPR_NOT_EQUAL = " != ";

const String wulTestCases::MES_CONSTRAINT = " constraints ";
const String wulTestCases::MES_INCREASE = " increase ";
const String wulTestCases::MES_DECREASE = " decrease ";

const String wulTestCases::MES_SUC = "SUCCESS";
const String wulTestCases::MES_FAIL = "FAILED";

const String wulTestCases::MES_YEAR = "year ";
const String wulTestCases::MES_MONTH = "month ";
const String wulTestCases::MES_DAY = "day ";
const String wulTestCases::MES_HOUR = "hour ";
const String wulTestCases::MES_MINUTE = "minute ";
const String wulTestCases::MES_SECOND = "second ";

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
  serialRef->print(MES_TESTING);
  serialRef->println("begin...");
	testMicroTime();
	testNtpClient();
  testLedPattern();
}

/**
 *
 */
void wulTestCases::testMicroTime() {
	serialRef->print(MES_TESTING);
	serialRef->print(MES_MICROTIME);
  serialRef->println(MES_RES);

 // Each test creates its own object on the heap.
 // They are destroyed immediatly when they are no longer needed.
 // This is to safe memory space in these memory limited platforms.
 // Each microTime object is about ~ 150 bytes large

  // assert year value above / below
  microTime *mt1 = new microTime(1994, 1, 1, 0, 0, 0);
  microTime *mt2 = new microTime(1995, 1, 1, 0, 0, 0);
  
  serialRef->print(MES_ASSRT);
  serialRef->print(MES_YEAR);
  serialRef->print(1994);
  serialRef->print(MES_OPR_ABOVE);
  serialRef->print(MES_YEAR);
  serialRef->print(1995);
  serialRef->print(MES_RES);
    
  if(mt1 < mt2 && mt2 > mt1) {
    serialRef->println(MES_SUC);
  }
  else {
    //serialRef->println("Assert year 1994 < year 1995 : FAILED");
    serialRef->println(MES_FAIL);
    haltFurtherExecution();
  }
  delete mt1;
  delete mt2;

  // assert month value above / below
  microTime *mt3 = new microTime(1994, 1, 1, 0, 0, 0);
  microTime *mt4 = new microTime(1994, 2, 1, 0, 0, 0);

  serialRef->print(MES_ASSRT);
  serialRef->print(MES_MONTH);
  serialRef->print(1);
  serialRef->print(MES_OPR_ABOVE);
  serialRef->print(MES_MONTH);
  serialRef->print(2);
  serialRef->print(MES_RES);
  
  if(mt3 < mt4 && mt4 > mt3) {
    serialRef->println(MES_SUC);
  }
  else {
    serialRef->println(MES_FAIL);
    haltFurtherExecution();
  }
  delete mt3;
  delete mt4;

  // assert day value above / below
  microTime *mt5 = new microTime(1994, 1, 1, 0, 0, 0);
  microTime *mt6 = new microTime(1994, 1, 2, 0, 0, 0);

  serialRef->print(MES_ASSRT);
  serialRef->print(MES_DAY);
  serialRef->print(1);
  serialRef->print(MES_OPR_ABOVE);
  serialRef->print(MES_DAY);
  serialRef->print(2);
  serialRef->print(MES_RES);

  if(mt5 < mt6 && mt6 > mt5) {
    serialRef->println(MES_SUC);
  }
  else {
    serialRef->println(MES_FAIL);
    haltFurtherExecution();
  }
  delete mt5;
  delete mt6;

  // assert hour value above / below
  microTime *mt7 = new microTime(1994, 1, 1, 0, 0, 0);
  microTime *mt8 = new microTime(1994, 1, 1, 1, 0, 0);

  serialRef->print(MES_ASSRT);
  serialRef->print(MES_HOUR);
  serialRef->print(0);
  serialRef->print(MES_OPR_ABOVE);
  serialRef->print(MES_HOUR);
  serialRef->print(1);
  serialRef->print(MES_RES);
  
  if(mt7 < mt8 && mt8 > mt7) {
    serialRef->println(MES_SUC);
  }
  else {
    serialRef->println(MES_FAIL);
    haltFurtherExecution();
  }
  delete mt7;
  delete mt8;

  // assert minutes value above / below
  microTime *mt9 = new microTime(1994, 1, 1, 0, 0, 0);
  microTime *mt10 = new microTime(1994, 1, 1, 0, 1, 0);

  serialRef->print(MES_ASSRT);
  serialRef->print(MES_MINUTE);
  serialRef->print(0);
  serialRef->print(MES_OPR_ABOVE);
  serialRef->print(MES_MINUTE);
  serialRef->print(1);
  serialRef->print(MES_RES);
  
  if(mt9 < mt10 && mt10 > mt9) {
    serialRef->println(MES_SUC);
  }
  else {
    serialRef->println(MES_FAIL);
    haltFurtherExecution();
  }
  delete mt9;
  delete mt10;

  // assert seconds value above / below
  microTime *mt11 = new microTime(1994, 1, 1, 0, 0, 0);
  microTime *mt12 = new microTime(1994, 1, 1, 0, 0, 1);

  serialRef->print(MES_ASSRT);
  serialRef->print(MES_SECOND);
  serialRef->print(0);
  serialRef->print(MES_OPR_ABOVE);
  serialRef->print(MES_SECOND);
  serialRef->print(1);
  serialRef->print(MES_RES);
  
  if(mt11 < mt12 && mt12 > mt11) {
    serialRef->println(MES_SUC);
  }
  else {
    serialRef->println(MES_FAIL);
    haltFurtherExecution();
  }

  // assert ==, !=, >= and <= operators
  mt11->update(1000000);

  serialRef->print(MES_ASSRT);
  serialRef->print(MES_MICROTIME);
  serialRef->print(MES_OPR_EQUAL);
  serialRef->print(MES_RES);
  if(mt11 == mt11) {
    serialRef->println(MES_SUC);
  }
  else {
    serialRef->println(MES_FAIL);
    haltFurtherExecution();
  }

  serialRef->print(MES_ASSRT);
  serialRef->print(MES_MICROTIME);
  serialRef->print(MES_OPR_NOT_EQUAL);
  serialRef->print(MES_RES);
  if(!(mt11 != mt11)) {
    serialRef->println(MES_SUC);
  }
  else {
    serialRef->println(MES_FAIL);
    haltFurtherExecution();
  }

  serialRef->print(MES_ASSRT);
  serialRef->print(MES_MICROTIME);
  serialRef->print(MES_OPR_ABV_EQ);
  serialRef->print(MES_RES);
  if(mt11 >= mt11) {
    serialRef->println(MES_SUC);
  }
  else {
    serialRef->println(MES_FAIL);
    haltFurtherExecution();
  }

  serialRef->print(MES_ASSRT);
  serialRef->print(MES_MICROTIME);
  serialRef->print(MES_OPR_BELOW_EQ);
  serialRef->print(MES_RES);
  if(mt11 <= mt11) {
    serialRef->println(MES_SUC);
  }
  else {
    serialRef->println(MES_FAIL);
    haltFurtherExecution();
  }
  delete mt11;
  delete mt12;

  // constraint tests
  serialRef->print(MES_ASSRT);
  serialRef->print(MES_MICROTIME);
  serialRef->print(MES_CONSTRAINT);
  serialRef->print(MES_RES);
  microTime *mt13 = new microTime(1850, 13, 32, 24, 60, 60);
  if(mt13->year() == 1970) {  // lower year contraint
  }
  else {
    serialRef->print(MES_OPR_BELOW);
    serialRef->print(MES_YEAR);
    serialRef->print(MES_FAIL);
    serialRef->print(MES_EXPECT);
    serialRef->print(1970);
    serialRef->print(MES_RES);
    serialRef->print(mt13->year());
    haltFurtherExecution();
  }
  if(mt13->month() == 12) { // upper month constraint
  }
  else {
    serialRef->print(MES_OPR_ABOVE);
    serialRef->print(MES_MONTH);
    serialRef->print(MES_FAIL);
    serialRef->print(MES_EXPECT);
    serialRef->print(12);
    serialRef->print(MES_RES);
    serialRef->print(mt13->month());
    haltFurtherExecution();
  }
  if(mt13->day() == 31) { // upper day constraint
  }
  else {
    serialRef->print(MES_OPR_ABOVE);
    serialRef->print(MES_DAY);
    serialRef->print(MES_FAIL);
    serialRef->print(MES_EXPECT);
    serialRef->print(31);
    serialRef->print(MES_RES);
    serialRef->print(mt13->day());
    haltFurtherExecution();
  }
  serialRef->println(MES_SUC);
  
}

/**
 *
 */
void wulTestCases::testNtpClient() {
	serialRef->print(MES_TESTING);
  serialRef->print(MES_NTPCLIENT);
  serialRef->println(MES_RES);
}

/**
 *
 */
void wulTestCases::testLedPattern() {
  serialRef->print(MES_TESTING);
  serialRef->print(MES_LEDPATTERN);
  serialRef->println(MES_RES);

  rgbColor hiCol = rgbColor(250, 250, 250);
  rgbColor lowCol = rgbColor(100, 100, 100);

  // move from #646464 to #FAFAFA over a period of 1000 microseconds
  ledPattern ld1 = ledPattern(lowCol, hiCol, 1000, ledPattern::patternModes::linear);
  ld1.update(100); // update 100 microseconds so linear color shift should be at: #737373 (115, 115, 155)

  /**
   * MATH EXPLAIN
   * We transition from 100 to 250 giving a 150 difference. This transistion of 150 units happens over a period of 1000
   * we move 100 meaning we have transistioned 1/10 of the total way.
   * 150/10 gives 15 and 100 + 15 = 115
   * So in a linear transistion we expect to have moved from 100 to 115
   */

  serialRef->print(MES_ASSRT);
  serialRef->print(MES_LEDPATTERN);
  serialRef->print(MES_INCREASE);
  serialRef->print(MES_RES);
  
  if(ld1.getColor().getRed() == 115) {
    serialRef->println(MES_SUC);
  }
  else {
    serialRef->print(MES_FAIL);
    serialRef->print(MES_EXPECT);
    serialRef->print(115);
    serialRef->print(MES_RES);
    serialRef->println(ld1.getColor().getRed());
    haltFurtherExecution();
  }

  ledPattern ld2 = ledPattern(hiCol, lowCol, 1000, ledPattern::patternModes::linear);
  ld2.update(100);

  serialRef->print(MES_ASSRT);
  serialRef->print(MES_LEDPATTERN);
  serialRef->print(MES_DECREASE);
  serialRef->print(MES_RES);

  if(ld2.getColor().getRed() == 235) {
    serialRef->println(MES_SUC);
  }
  else {
    serialRef->print(MES_FAIL);
    serialRef->print(MES_EXPECT);
    serialRef->print(235);
    serialRef->print(MES_RES);
    serialRef->println(ld2.getColor().getRed());
    haltFurtherExecution();
  }
}

/**
 * Stop further execution of other processes
 * Used as a method to stop after an failed assertion
 */
void wulTestCases::haltFurtherExecution() {
  while(true)
    delay(2000);
}


