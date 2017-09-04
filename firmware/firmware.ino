#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define ADAFRUIT_7SEGMENT
//#define SPARKFUN_7SEGMENT

#define JOYSTICK
//#define ENCODER

//NOTE - THE KEEPALIVE BUTTON NEEDS TO BE A PULL DOWN STYLE BUTTON!!!
//#define USE_KEEP_ALIVE_BUTTON

//#define DEBUG

#define SOUND

#ifdef ADAFRUIT_7SEGMENT
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>
typedef Adafruit_7segment display_t;
uint8_t display_i2c = 0x70;
#endif

#ifdef SPARKFUN_7SEGMENT
#include "Sparkfun_LEDBackpack.h"
typedef Sparkfun_7segment display_t;
uint8_t display_i2c = 0x71;
#endif

int xout_pin = A0;     //also CLK
int yout_pin = A1;     //also DT
int sel_pin = A2;      //also SW
int keep_alive_pin = A3;
int fx_pin = 5;
int front_pin = 10;
int front_count = 3;
int dome_pin = 12;
int dome_count = 1;
long keep_alive_timer = 0;

void show_dimension();
void shoot();
void onleft();
void onright();
void onclick();
void update_keep_alive();

#ifdef JOYSTICK
#include "joystick.h"
typedef joystick_t controller_t;
#endif

#ifdef ENCODER
#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>
#include "encoder.h"
typedef encoder_t controller_t;
#endif

#ifdef USE_KEEP_ALIVE_BUTTON
  controller_t controller(xout_pin, yout_pin, sel_pin, true);
#else
  controller_t controller(xout_pin, yout_pin, sel_pin, false);
#endif

display_t display;
Adafruit_NeoPixel front(front_count, front_pin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel dome(dome_count, dome_pin, NEO_GRB + NEO_KHZ800);
uint16_t dimension = 0xC132;

void setup()
{
#ifdef SOUND
  pinMode(fx_pin, OUTPUT);
#endif

#ifdef USE_KEEP_ALIVE_BUTTON
  pinMode(keep_alive_pin, OUTPUT);
  digitalWrite(keep_alive_pin, HIGH);
  update_keep_alive();
#endif

  controller.onleft = onleft;
  controller.onright = onright;
  controller.onclick = onclick;
  controller.setup();

  front.begin();
  dome.begin();

  dome.setPixelColor(0, dome.Color(0, 255, 0));
  dome.show();

#ifdef DEBUG
  Serial.begin(57600);
#endif

  display.begin(display_i2c);
  show_dimension();
}

void loop()
{
#ifdef SOUND
  digitalWrite(fx_pin, HIGH);
#endif

  controller.loop();

#ifdef USE_KEEP_ALIVE_BUTTON
  if(millis() > keep_alive_timer)
  {
  #ifdef DEBUG
    Serial.println("Auto-off");
    delay(100);
  #endif
    digitalWrite(keep_alive_pin, LOW);
    delay(10000);
  }
#endif
}

void show_dimension()
{
#ifdef DEBUG
  Serial.print("Dimension:  ");
  Serial.println(dimension, HEX);
#endif

  display.println(dimension, HEX);
  display.writeDisplay();
}

void shoot()
{
#ifdef DEBUG
  Serial.println("SHOOT!");
#endif

#ifdef SOUND
  digitalWrite(fx_pin, LOW);
  delay(320);
#endif

  for (int brightness = 0; brightness <= 255; ++brightness)
  {
    for (int ii = 0; ii < front_count; ++ii)
      front.setPixelColor(ii, front.Color(0, brightness, 0));

    front.show();
    delay(1);
  }

  for (int brightness = 255; brightness >= 0; --brightness)
  {
    for (int ii = 0; ii < front_count; ++ii)
      front.setPixelColor(ii, front.Color(0, brightness, 0));

    front.show();
    delay(1);
  }

#ifdef SOUND
  digitalWrite(fx_pin, HIGH);
#endif
}

void onleft()
{
  --dimension;
  show_dimension();
  update_keep_alive();
}

void onright()
{
  ++dimension;
  show_dimension();
  update_keep_alive();
}

void onclick()
{
  shoot();
  update_keep_alive();
}

void update_keep_alive()
{
#ifdef USE_KEEP_ALIVE_BUTTON
  keep_alive_timer = millis() + 10000;
#endif
}












