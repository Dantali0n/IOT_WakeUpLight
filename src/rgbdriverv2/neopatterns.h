#ifndef _NEO_PATTERNS_h
#ifdef __cplusplus
#define _NEO_PATTERNS_h

#include <Adafruit_NeoPixel.h>
#include "actor.h"

// Pattern types supported:
enum  pattern { RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE, NO_PATTERN };
const static char *PATTERN_STRING[] = { "RAINBOW_CYCLE", "THEATER_CHASE", "COLOR_WIPE", "SCANNER", "FADE", "NO_PATTERN" };

// Pattern directions supported:
enum  direction { FORWARD, REVERSE };
const static char *DIRECTION_STRING[] = { "FORWARD", "REVERSE" };

// Pattern trace type, determines amount of simultaneous bright emissions
enum  trace { SINGLE, DUAL };
const static char *TRACE_STRING[] = { "SINGLE", "DUAL" };
 
// NeoPattern Class - derived from the Adafruit_NeoPixel class
class NeoPatterns : public Adafruit_NeoPixel, public Actor {
public:
    // Member Variables:  
    pattern  ActivePattern;  // which pattern is running
    direction Direction;     // direction to run the pattern
    trace Trace;
    
    unsigned long Interval;   // milliseconds between updates
    
    uint32_t Color1, Color2;  // What colors are in use
    uint16_t TotalSteps;  // total number of steps in the pattern
    uint16_t Index;  // current step within the pattern
    
    void (*OnComplete)(NeoPatterns*);  // Callback on completion of pattern
    
    // Constructor - calls base-class constructor to initialize strip
    NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)(NeoPatterns* stick));
    
    // Update the pattern
    void update(uint32_t deltaTime);
  
    // Increment the Index and reset at the end
    void Increment();
    
    // Reverse pattern direction
    void Reverse();
    
    // Initialize for a RainbowCycle
    void RainbowCycle(uint8_t interval, direction dir = FORWARD);
    
    // Update the Rainbow Cycle Pattern
    void RainbowCycleUpdate();
 
    // Initialize for a Theater Chase
    void TheaterChase(uint32_t color1, uint32_t color2, uint8_t interval, direction dir = FORWARD);
    
    // Update the Theater Chase Pattern
    void TheaterChaseUpdate();
 
    // Initialize for a ColorWipe
    void ColorWipe(uint32_t color, uint8_t interval, direction dir = FORWARD);
    
    // Update the Color Wipe Pattern
    void ColorWipeUpdate();
    
    // Initialize for a SCANNNER
    void Scanner(uint32_t color1, uint8_t interval, trace trc = DUAL, direction dir = FORWARD);
 
    // Update the Scanner Pattern
    void ScannerUpdate();
    
    // Initialize for a Fade
    void Fade(uint32_t color1, uint32_t color2, uint16_t steps, uint8_t interval, direction dir = FORWARD);
    
    // Update the Fade Pattern
    void FadeUpdate();

    // Disable patterns
    void NoPattern();
   
    // Calculate 50% dimmed version of a color (used by ScannerUpdate)
    uint32_t DimColor(uint32_t color);
 
    // Set all pixels to a color (synchronously)
    void ColorSet(uint32_t color);
 
    // Returns the Red component of a 32-bit color
    uint8_t Red(uint32_t color);
 
    // Returns the Green component of a 32-bit color
    uint8_t Green(uint32_t color);
 
    // Returns the Blue component of a 32-bit color
    uint8_t Blue(uint32_t color);
    
    // Input a value 0 to 255 to get a color value.
    // The colours are a transition r - g - b - back to r.
    uint32_t Wheel(byte WheelPos);
};

#endif
#endif