#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include "button.h"

class encoder_t
{
  public:
    void (*onleft)();
    void (*onright)();
    void (*onclick)();

    encoder_t(const int clk_pin, const int dt_pin, const int sw_pin): encoder_m(clk_pin, dt_pin), position_m(0), button_m(sw_pin)
    {}

    void setup()
    {
      button_m.setup();
    }

    void loop()
    {
      long position = encoder_m.read() / 2;

      if (position > position_m && onleft)
        onleft();
      if (position < position_m && onright)
        onright();

      position_m = position;
      button_m.onclick = onclick;
      button_m.loop();
    }

  private:
    Encoder encoder_m;
    long position_m;
    button_t button_m;
};

#endif
