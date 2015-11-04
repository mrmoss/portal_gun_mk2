#ifdef __AVR_ATtiny85__
#include <TinyWireM.h>
#define Wire TinyWireM
#else
#include <Wire.h>
#endif
#include "Sparkfun_LEDBackpack.h"

Sparkfun_7segment::Sparkfun_7segment(void): i2c_addr(0x71), changed(false)
{
  clear();
}

void Sparkfun_7segment::begin(uint8_t _addr)
{
  i2c_addr = _addr;
  Wire.begin();
  setBrightness(255);
}

void Sparkfun_7segment::setBrightness(uint8_t b)
{
  Wire.beginTransmission(i2c_addr);
  Wire.write(0x7A);
  Wire.write(b);
  Wire.endTransmission();
}

void Sparkfun_7segment::writeDisplay(void)
{
  if (changed)
  {
    Wire.beginTransmission(i2c_addr);

    for (int i = 0; i < 4; i++)
      Wire.write(displaybuffer[i]);
    Wire.endTransmission();
    changed = false;
  }
}

void Sparkfun_7segment::clear(void)
{
  for (int i = 0; i < 4; i++)
    displaybuffer[i] = ' ';
}

void Sparkfun_7segment::print(int n, int base)
{
  if (base == DEC)
    snprintf(displaybuffer, 5, "%4d", n);
  else if (base == HEX)
    snprintf(displaybuffer, 5, "%4X", n);
  changed = true;
}

void  Sparkfun_7segment::println(int n, int base)
{
  print(n, base);
}
