#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define ADAFRUIT_7SEGMENT
//#define SPARKFUN_7SEGMENT

#ifdef ADAFRUIT_7SEGMENT
  #include <Adafruit_LEDBackpack.h>
  #include <Adafruit_GFX.h>
  typedef Adafruit_7segment display_t;
  uint8_t display_i2c=0x70;
#endif

#ifdef SPARKFUN_7SEGMENT
  #include "Sparkfun_LEDBackpack.h"
  typedef Sparkfun_7segment display_t;
  uint8_t display_i2c=0x71;
#endif

int xout_pin=A0;
int yout_pin=A1;
int sel_pin=A2;
int fx_pin=5;
int front_pin=10;
int front_count=3;
int dome_pin=12;
int dome_count=1;
display_t display;
Adafruit_NeoPixel front(front_count,front_pin,NEO_GRB+NEO_KHZ800);
Adafruit_NeoPixel dome(dome_count,dome_pin,NEO_GRB+NEO_KHZ800);

void setup()
{
  pinMode(xout_pin,INPUT);
  pinMode(yout_pin,INPUT);
  pinMode(sel_pin,INPUT_PULLUP);
  pinMode(fx_pin,OUTPUT);

  front.begin();
  dome.begin();
  display.begin(display_i2c);

  Serial.begin(57600);
}

void loop()
{
  int value=analogRead(xout_pin)+analogRead(yout_pin);
  display.println(value);

  if(!digitalRead(sel_pin))
  {
    for(int ii=0;ii<front_count;++ii)
      front.setPixelColor(ii,front.Color(0,255,0));
    for(int ii=0;ii<dome_count;++ii)
      dome.setPixelColor(ii,dome.Color(0,255,0));

    digitalWrite(fx_pin,LOW);
    delay(5);
  }
  else
  {
    for(int ii=0;ii<front_count;++ii)
      front.setPixelColor(ii,front.Color(0,0,0));
    for(int ii=0;ii<dome_count;++ii)
      dome.setPixelColor(ii,dome.Color(0,0,0));

    digitalWrite(fx_pin,HIGH);
  }

  display.writeDisplay();
  front.show();
  dome.show();
}
