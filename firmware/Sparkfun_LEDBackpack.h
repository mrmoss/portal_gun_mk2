#ifndef Sparkfun_LEDBackpack_h
#define Sparkfun_LEDBackpack_h

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifdef __AVR_ATtiny85__
#include <TinyWireM.h>
#else
#include <Wire.h>
#endif

class Sparkfun_7segment {
  public:
    Sparkfun_7segment(void);

    void begin(uint8_t _addr);
    void setBrightness(uint8_t b);
    void blinkRate(uint8_t b);
    void writeDisplay(void);
    void clear(void);

    void print(int, int = DEC);
    void println(int, int = DEC);

  private:
    uint8_t i2c_addr;
    bool changed;
    char displaybuffer[5];
};
#endif // Sparkfun_LEDBackpack_h
