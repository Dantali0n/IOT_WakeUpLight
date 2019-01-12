#include "neopatterns.h"
    
// Constructor - calls base-class constructor to initialize strip
NeoPatterns::NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)(NeoPatterns* stick)):Adafruit_NeoPixel(pixels, pin, type),Actor()
    , ActivePattern(NO_PATTERN)
    , Direction(FORWARD)
    , Trace(DUAL)
    , Interval(0)
    , Color1(0)
    , Color2(0)
    , TotalSteps(1)
    , Index(0)
{
    OnComplete = callback;
}
    
// Update the pattern
void NeoPatterns::update(uint32_t deltaTime) {
    lastUpdate += deltaTime;
    if(lastUpdate > Interval*1000) { // time to update
        
        float steps = lastUpdate / (Interval*1000.0);
        if(steps > 1.25 && PERFORMANCE_PROFILE) {
          Serial.print("lagg: ");
          Serial.println(steps);
        }
        
        lastUpdate = 0;
        switch(ActivePattern) {
            case RAINBOW_CYCLE:
                RainbowCycleUpdate();
                break;
            case THEATER_CHASE:
                TheaterChaseUpdate();
                break;
            case COLOR_WIPE:
                ColorWipeUpdate();
                break;
            case SCANNER:
                ScannerUpdate();
                break;
            case FADE:
                FadeUpdate();
                break;
            case NO_PATTERN:
                ColorSet(Color1);
            default:
                break;
        }
    }
}
  
// Increment the Index and reset at the end
void NeoPatterns::Increment() {
    if (Direction == FORWARD) {
       Index++;
       if (Index >= TotalSteps) {
            Index = 0;
            if (OnComplete != NULL) {
                OnComplete(this); // call the comlpetion callback
            }
        }
    }
    else { // Direction == REVERSE
        --Index;
        if (Index <= 0) {
            Index = TotalSteps-1;
            if (OnComplete != NULL) {
                OnComplete(this); // call the comlpetion callback
            }
        }
    }
}
    
// Reverse pattern direction
void NeoPatterns::Reverse() {
    if (Direction == FORWARD) {
        Direction = REVERSE;
        Index = TotalSteps-1;
    }
    else {
        Direction = FORWARD;
        Index = 0;
    }
}
    
// Initialize for a RainbowCycle
void NeoPatterns::RainbowCycle(uint8_t interval, direction dir) {
    ActivePattern = RAINBOW_CYCLE;
    Interval = interval;
    TotalSteps = 255;
    Index = 0;
    Direction = dir;
}
    
// Update the Rainbow Cycle Pattern
void NeoPatterns::RainbowCycleUpdate(){
    for(int i=0; i< numPixels(); i++) {
        setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
    }
    show();
    Increment();
}
 
// Initialize for a Theater Chase
void NeoPatterns::TheaterChase(uint32_t color1, uint32_t color2, uint8_t interval, direction dir) {
    ActivePattern = THEATER_CHASE;
    Interval = interval;
    TotalSteps = numPixels();
    Color1 = color1;
    Color2 = color2;
    Index = 0;
    Direction = dir;
}
    
// Update the Theater Chase Pattern
void NeoPatterns::TheaterChaseUpdate() {
    for(int i=0; i< numPixels(); i++) {
        if ((i + Index) % 3 == 0) {
            setPixelColor(i, Color1);
        }
        else {
            setPixelColor(i, Color2);
        }
    }
    show();
    Increment();
}
 
// Initialize for a ColorWipe
void NeoPatterns::ColorWipe(uint32_t color, uint8_t interval, direction dir) {
    ActivePattern = COLOR_WIPE;
    Interval = interval;
    TotalSteps = numPixels();
    Color1 = color;
    Index = 0;
    Direction = dir;
}
    
// Update the Color Wipe Pattern
void NeoPatterns::ColorWipeUpdate() {
    setPixelColor(Index, Color1);
    show();
    Increment();
}
    
// Initialize for a SCANNNER
void NeoPatterns::Scanner(uint32_t color1, uint8_t interval, trace trc, direction dir) {
    ActivePattern = SCANNER;
    Interval = interval;
    Trace = trc;
    switch(Trace){
        case SINGLE:
        TotalSteps = (numPixels() * 2);
        break;
        case DUAL:
        TotalSteps = numPixels();
        break;
    }
    Color1 = color1;
    Index = 0;
    Direction = dir;
    // set all colors briefly to prevent trailing of different colors.
    ColorSet(this->Color1); 
}
 
// Update the Scanner Pattern
void NeoPatterns::ScannerUpdate() { 
    if(Trace == DUAL) {  
        for (int i = 0; i < numPixels(); i++) {
            if (i == Index) { // Scan Pixel to the right
                 setPixelColor(i, Color1);
            }
            else if (i == TotalSteps - 1 - Index) { // Scan Pixel to the left
                 setPixelColor(i, Color1);
            }
            else { // Fading tail
                 setPixelColor(i, DimColor(getPixelColor(i)));
            }
        }
    }
    else {
        for (int i = 0; i < numPixels(); i++) {
            if (i == Index && Index < numPixels()) { // Scan Pixel to the right
                 setPixelColor(i, Color1);
            }
            else if (i == TotalSteps - 1 - Index && Index > numPixels()) { // Scan Pixel to the left
                 setPixelColor(i, Color1);
            }
            else { // Fading tail
                 setPixelColor(i, DimColor(getPixelColor(i)));
            }
        }
    }

    show();
    Increment();
}
    
// Initialize for a Fade
void NeoPatterns::Fade(uint32_t color1, uint32_t color2, uint16_t steps, uint8_t interval, direction dir) {
    ActivePattern = FADE;
    Interval = interval;
    TotalSteps = steps;
    Color1 = color1;
    Color2 = color2;
    Index = 0;
    Direction = dir;
}
    
// Update the Fade Pattern
void NeoPatterns::FadeUpdate() {
    // Calculate linear interpolation between Color1 and Color2
    // Optimize order of operations to minimize truncation error
    uint8_t red = ((Red(Color1) * (TotalSteps - Index)) + (Red(Color2) * Index)) / TotalSteps;
    uint8_t green = ((Green(Color1) * (TotalSteps - Index)) + (Green(Color2) * Index)) / TotalSteps;
    uint8_t blue = ((Blue(Color1) * (TotalSteps - Index)) + (Blue(Color2) * Index)) / TotalSteps;
    
    ColorSet(Color(red, green, blue));
    show();
    Increment();
}

void NeoPatterns::NoPattern() {
  ActivePattern = NO_PATTERN;
}
   
// Calculate 50% dimmed version of a color (used by ScannerUpdate)
uint32_t NeoPatterns::DimColor(uint32_t color) {
    // Shift R, G and B components one bit to the right
    uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
    return dimColor;
}
 
// Set all pixels to a color (synchronously)
void NeoPatterns::ColorSet(uint32_t color) {
    for (int i = 0; i < numPixels(); i++) {
        setPixelColor(i, color);
    }
    show();
}
 
// Returns the Red component of a 32-bit color
uint8_t NeoPatterns::Red(uint32_t color) {
    return (color >> 16) & 0xFF;
}

// Returns the Green component of a 32-bit color
uint8_t NeoPatterns::Green(uint32_t color) {
    return (color >> 8) & 0xFF;
}

// Returns the Blue component of a 32-bit color
uint8_t NeoPatterns::Blue(uint32_t color) {
    return color & 0xFF;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t NeoPatterns::Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
        return Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else if(WheelPos < 170) {
        WheelPos -= 85;
        return Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    else {
        WheelPos -= 170;
        return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
}