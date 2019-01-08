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

#include "smoothmapread.h"

SmoothMapRead::SmoothMapRead(int input) {
  this->input = input;
}

uint16_t SmoothMapRead::read() {
  return read(this->numSamples);
}

uint16_t SmoothMapRead::read(uint8_t numSamples) {
  uint16_t sampleData = 0;
  
  // sample data from analog input and compute average
  for(uint8_t i = 0; i < numSamples; i++) {
    sampleData += analogRead(input);
  }
  sampleData = sampleData / numSamples;

  // lower deathzone
  if(sampleData < lowDeathzone) {
    return lowOutput;
  }
  // linear region 
  else if(sampleData < highDeathzone) {
    return map(sampleData, lowDeathzone, highDeathzone, lowOutput, highOutput);
  }
  // higher deathzone
  else {
    return highOutput;
  }
}


