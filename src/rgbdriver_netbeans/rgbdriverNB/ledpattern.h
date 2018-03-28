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

#ifndef _Led_Pattern_h
#ifdef __cplusplus
#define _Led_Pattern_h

#include <list>
#include "rgbrltypedef.h"
#include "rgbcolor.h"
#include "templateneopixel.h"

/**
 * Statefull lighting pattern for led strip.
 * Supports incremental state updates using deltaTime in microseconds
 * @TODO add none linear interpolation for color transistions
 */
class ledPattern {
    protected:
      bool finished; // if the ledPattern has finished animating
      bool finishes; // if the ledPattern ever finishes animating
      bool paused; // pause the progression of the animation
      bool disable; // disable the ledStrip
      bool safeDelete; // if the ledPattern is created with the new keyword and can safely be called by delete

      std::list<templateNeopixel> strips; // the neopixel strips controlled by ledPattern
      
    private:
    public:

        ledPattern(templateNeopixel &strip);
   
	void increment(uint32_t deltaTime);
        void reset();

        templateNeopixel* getStrip();
    
        bool isFinished();
        bool doesFinish();
        bool isPaused();
        void setPaused(bool paused);
        bool isDisabled();
        void setDisabled(bool disabled);
        bool isSafeDelete();
        void setSafeDelete(bool safeDelete);
};

#endif /* __cplusplus */
#endif /* _Led_Pattern_h */

