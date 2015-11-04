#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <Arduino.h>
#include "button.h"

class axis_t
{
  public:
    void (*onnegative)();
    void (*onpositive)();

    axis_t(const int pin, const int center = 500, const int deadzone = 100): pin_m(pin), center_m(center), deadzone_m(deadzone), average_m(0), count_m(0), samples_m(20), moved_m(0)
    {}

    void setup()
    {
      pinMode(pin_m, INPUT);
    }

    void loop()
    {
      average_m += analogRead(pin_m);
      ++count_m;

      if (count_m >= samples_m)
      {
        average_m /= samples_m;

        if (moved_m != 0 && average_m >= center_m - deadzone_m / 2 && average_m <= center_m + deadzone_m / 2)
          moved_m = 0;

        if (moved_m > -1 && average_m < center_m - deadzone_m / 2)
        {
          if (onnegative)
            onnegative();

          moved_m = -1;
        }

        if (moved_m < 1 && average_m > center_m + deadzone_m / 2)
        {
          if (onpositive)
            onpositive();

          moved_m = 1;
        }

        average_m = 0;
        count_m = 0;
      }
    }

  private:
    int pin_m;
    int center_m;
    int deadzone_m;
    int average_m;
    int count_m;
    int samples_m;
    int moved_m;
};

class joystick_t
{
  public:
    void (*onleft)();
    void (*onright)();
    void (*onup)();
    void (*ondown)();
    void (*onclick)();

    joystick_t(const int xout_pin, const int yout_pin, const int sel_pin): x_axis_m(xout_pin), y_axis_m(yout_pin), button_m(sel_pin)
    {}

    void setup()
    {
      x_axis_m.setup();
      y_axis_m.setup();
      button_m.setup();
    }

    void loop()
    {
      x_axis_m.onnegative = onleft;
      x_axis_m.onpositive = onright;
      x_axis_m.loop();

      y_axis_m.onnegative = ondown;
      y_axis_m.onpositive = onup;
      y_axis_m.loop();

      button_m.onclick = onclick;
      button_m.loop();
    }

  private:
    axis_t x_axis_m;
    axis_t y_axis_m;
    button_t button_m;
};

#endif
