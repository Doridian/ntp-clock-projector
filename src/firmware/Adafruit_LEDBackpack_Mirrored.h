#ifndef Adafruit_LEDBackpack_Mirrored_h
#define Adafruit_LEDBackpack_Mirrored_h

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Adafruit_LEDBackpack.h>

class Adafruit_7segment_Mirrored : public Adafruit_7segment {
    public:
    void writeDigitNumMirrored(uint8_t x, uint8_t num, boolean dot = false);
    void turnOffDigit(uint8_t x);
};

#endif
