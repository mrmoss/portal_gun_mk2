#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class button_t
{
  public:
    void (*onclick)();

    button_t(const int pin, const bool keep_alive_setup = false): pin_m(pin), average_m(0), count_m(0), samples_m(20), down_m(false), keep_alive_setup_m(keep_alive_setup)
    {}

    void setup()
    {
      if(keep_alive_setup_m)
        pinMode(pin_m, INPUT);
      else
        pinMode(pin_m, INPUT_PULLUP);
    }

    void loop()
    {
      if (keep_alive_setup_m)
      {
        if (analogRead(pin_m) > 40)
          average_m += 0;
        else
          average_m += 1;
      }
      else
      {
        average_m += digitalRead(pin_m);
      }

      ++count_m;

      if (count_m >= samples_m)
      {
        average_m /= samples_m;

        if (down_m && average_m > 0)
          down_m = false;

        if (!down_m && average_m <= 0)
        {
          if (onclick)
            onclick();

          down_m = true;
        }

        average_m = 0;
        count_m = 0;
      }
    }

  private:
    int pin_m;
    int average_m;
    int count_m;
    int samples_m;
    bool down_m;
    bool keep_alive_setup_m;
};

#endif
