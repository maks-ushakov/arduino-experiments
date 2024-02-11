#include "MFS_LED.h"
#include "Arduino.h"
#include <WString.h>

MFS_LED::MFS_LED(int n)
  {
    if (n > 4) {
      n = 4;
    }
    numLED = n;
    for (byte i = 0; i < n; i++) {
      leds[i] = startLED + i;
    }
  };

void MFS_LED::init()
  {
    for (byte i = 0; i < numLED; i++) {
      pinMode(leds[i], OUTPUT);
    }
  };

int MFS_LED::size() {
    return numLED;
  };

void MFS_LED::setLed(int ledIndex)
  {
    digitalWrite(leds[ledIndex], LOW);
  };

void MFS_LED::clearLed(int ledIndex)
  {
    if (checkIndex(ledIndex)) {
      digitalWrite(leds[ledIndex], HIGH);
    }
  };

void MFS_LED::toggleLed(int ledIndex)
  {
    if (checkIndex(ledIndex)) {
      int status = digitalRead(leds[ledIndex]);
      digitalWrite(leds[ledIndex], !status);
    }
  };

void MFS_LED::reset()
  {
    for (byte i = 0; i < numLED; i++) {
      clearLed(i);
    }
  };
int MFS_LED::getState(int ledIndex) {
  if (checkIndex(ledIndex)) {
    return digitalRead(leds[ledIndex]);
  }
}

String MFS_LED::getPrintState(int ledIndex) {
  if (checkIndex(ledIndex)) {
    return digitalRead(leds[ledIndex]) == LOW ? "ON" : "OFF";
  }
  return "NONE";
}

int MFS_LED::checkIndex(int ledIndex) {
  return ledIndex < numLED ? 1 : 0;
}