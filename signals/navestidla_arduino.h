#ifndef NAVESTIDLA_ARDUINO_H
#define NAVESTIDLA_ARDUINO_H

#ifndef ESP8266

#include <Arduino.h>
#include <arduino-timer.h>
#include <jled.h>
#include <Wire.h>

#include "navestidla_config.h"

#define LIVE_BIT 0

#define TURN_ON_SERIAL false
// #define TURN_ON_SERIAL true

const int fade_on_time = 150; // doba zhasinani
const int fade_off_time = 150;  // doba rozsveceni
const int delay_fade_off = 60;  // doba odlozeni rozsveceni, aby drive zacal zhasinat minuly znak

const int slow_period = 556; // 54 kmitu za minutu = perioda 1111 ms
const int fast_period = 278;  // 108 kmitu za minutu = perioda 556 ms

const int LIVE_BIT_SLACK = 1000;

void turn_on(JLed* led);
void turn_off(JLed* led);
void breathe_slow(JLed* led);
void breathe_fast(JLed* led);


bool handle_live_bit(void*);
void requestEvent();
void receiveEvent(int howMany);

void navestidla_setup(uint8_t bus_address);
void navestidla_loop();

#endif // ESP8266

#endif // NAVESTIDLA_ARDUINO_H