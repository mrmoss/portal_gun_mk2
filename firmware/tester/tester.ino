#include <Wire.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>

int xout_pin=A0;
int yout_pin=A1;
int sel_pin=A2;
int fx_pin=5;
int np_pin=10;
Adafruit_7segment display;
Adafruit_NeoPixel leds(np_pin,5,NEO_GRB+NEO_KHZ800);

void setup()
{
  pinMode(xout_pin,INPUT);
  pinMode(yout_pin,INPUT);
  pinMode(sel_pin,INPUT_PULLUP);
  pinMode(fx_pin,OUTPUT);

  leds.begin();
  display.begin(0x70);

  Serial.begin(57600);
}

void loop()
{
  int value=analogRead(xout_pin)+analogRead(yout_pin);
  display.println(value);
  //Serial.println(value);

  if(!digitalRead(sel_pin))
    for(int ii=0;ii<5;++ii)
      leds.setPixelColor(ii,leds.Color(0,255,0));
  else
    for(int ii=0;ii<5;++ii)
      leds.setPixelColor(ii,leds.Color(0,0,0));

  if(!digitalRead(sel_pin))
  {
    Serial.println("FIRE!");
    digitalWrite(fx_pin,LOW);
    delay(5);
  }
  else
  {
    digitalWrite(fx_pin,HIGH);
  }

  display.writeDisplay();
  leds.show();
}
