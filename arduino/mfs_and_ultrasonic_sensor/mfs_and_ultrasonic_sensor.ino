/**
 * @file mfs_and_ultrasonic.ino
 * @section description
 * Multifunctional Shield and HC-SR04 ultrasonic sensor
 * Pin 6 and 5 for SR04
 * Result on Display
 * Buttons:
 * 1) if pressed it show last measure
 * 2) switch to show measure in centimeters
 * 3) switch to show measure in inches
 * @section author PR0Maks
 * @section github
 * https://github.com/maks-ushakov/arduino-experiments/arduino/mfs_and_ultrasonic
 */

// Include section
#include "MFS_Display.h"

// Define section 
#define BUTTON_1_PIN  A1
#define BUTTON_2_PIN  A2
#define BUTTON_3_PIN  A3

#define LATCH_PIN     4
#define CLK_PIN       7
#define DATA_PIN      8

#define TRIG_PIN      6
#define ECHO_PIN      5

#define UNIT_CM       1
#define UNIT_INCH     2

#define SYMB_CM       0b10100111
#define SYMB_INCH     0b11011101

// Global variables

unsigned long previousTime = 0;

MFS_Display mfs_display(DATA_PIN, CLK_PIN, LATCH_PIN);

long duration, cm, inches;
uint8_t units = UNIT_CM;

void setup()
{
  //Define inputs and outputs
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  pinMode(BUTTON_1_PIN, INPUT);
  pinMode(BUTTON_2_PIN, INPUT);
  pinMode(BUTTON_3_PIN, INPUT);

  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
  OCR0A = 0xFF;
  TIMSK0 |= _BV(OCIE0A);

  // Display settings
  mfs_display.init();
  mfs_display.reset();
  mfs_display.on();
  mfs_display.setUpdateTime(100);

  // Serial console for test
  Serial.begin(9600);
  Serial.println(F("MF Shield Sensors"));
}

void loop()
{
  unsigned long currentMillis = millis();
  if(currentMillis - previousTime > 250) {
    uint8_t button1 = digitalRead(BUTTON_1_PIN);
    uint8_t button2 = digitalRead(BUTTON_2_PIN);
    uint8_t button3 = digitalRead(BUTTON_3_PIN);

    if (button1 == HIGH) {
      // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
      // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
      digitalWrite(TRIG_PIN, LOW);
      delayMicroseconds(5);
      digitalWrite(TRIG_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG_PIN, LOW);
    
      // Read the signal from the sensor: a HIGH pulse whose
      // duration is the time (in microseconds) from the sending
      // of the ping to the reception of its echo off of an object.
      duration = pulseIn(ECHO_PIN, HIGH);
    
      // Convert the time into a distance
      cm = (duration * 343.2 * 0.0001) / 2; // 343.2 m/s is a speed of sound
      inches = (duration / 2) / 74;   // Divide by 74 or multiply by 0.0135
    }

    if (button2 == LOW) {
      units = UNIT_CM;
    }
    if ((button3 == LOW)) {
      units = UNIT_INCH;
    }  

    if (units == UNIT_CM) {
      mfs_display.showUIntWithSuffix(cm, SYMB_CM);
    }
    
    if (units == UNIT_INCH) {
      mfs_display.showUIntWithSuffix(inches, SYMB_INCH);
    }
  }
  if(currentMillis - previousTime > 900) {
    Serial.print(inches);
    Serial.print("in, ");
    Serial.print(cm);
    Serial.print("cm");
    Serial.println();
  }
}

// Interrupt is called once a millisecond,
SIGNAL(TIMER0_COMPA_vect)
{
  unsigned long currentMillis = millis();
  mfs_display.update(currentMillis);
}
