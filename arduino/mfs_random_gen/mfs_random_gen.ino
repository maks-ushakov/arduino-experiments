/**
 * @file mfs_control_by_uart.info
 * @section description
 * Control Multifunctional Shield with buttons
 * - LEDs test: on and off LEDs
 * - Display test: All segments and Hexadecimal digits
 * - Random number on Display
 * @section author PR0Maks
 * @section github
 * https://github.com/maks-ushakov/arduino-experiments/arduino/mfs_random_gen
 * @section simulation
 * https://www.tinkercad.com/things/2toP4bNdBqG-mfun-sield-display-and-buttons
 */
#include "MFS_LED.h"
#include "MFS_Display.h"


// Board Definitions
#define BUTTON_1_PIN  A1
#define BUTTON_2_PIN  A2
#define BUTTON_3_PIN  A3

#define LATCH_PIN     4
#define CLK_PIN       7
#define DATA_PIN      8

// Global variables

unsigned long previousTime = 0;
unsigned long previousLedTime = 0;
unsigned long previousDisplayTime = 0;

MFS_LED mfs_led(4);
MFS_Display mfs_display(DATA_PIN, CLK_PIN, LATCH_PIN);

bool isLedTest = true;
uint8_t ledCurrentTest = 0;
uint8_t ledTestCounter = 0;
uint8_t ledOrigTests = 2;
long ledTestPeriod = 250;

bool isDisplayTest = false;
int8_t displayCurrentTest = -1;
uint8_t displayTestCounter = 0;
uint8_t displayTestDigit = 0;
long displayTestPeriod = 100;

void setup()
{
  pinMode(BUTTON_1_PIN, INPUT);
  pinMode(BUTTON_2_PIN, INPUT);
  pinMode(BUTTON_3_PIN, INPUT);

  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
  OCR0A = 0xFF;
  TIMSK0 |= _BV(OCIE0A);

  mfs_led.init();
  mfs_led.reset();
  //ledTest(0);
  mfs_display.init();
  mfs_display.reset();
  mfs_display.on();
  mfs_display.setUpdateTime(500);
  //displayTest();
  randomSeed(analogRead(A5));
  Serial.begin(9600);
  Serial.println(F("MF Shield Tests"));
}

void loop()
{
  unsigned long currentMillis = millis();
  // mfs_display.update(currentMillis);
  if(currentMillis - previousTime > 250) {
    uint8_t button1 = digitalRead(BUTTON_1_PIN);
    uint8_t button2 = digitalRead(BUTTON_2_PIN);
    uint8_t button3 = digitalRead(BUTTON_3_PIN);

    if ((button1 == LOW)&& !isLedTest) {
      isLedTest = true;
      Serial.println("Led Test");
    }
    if ((button3 == LOW) && !isDisplayTest) {
      int randNumber = random(99);
      mfs_display.showNice(randNumber);
    } 
    else if ((button2 == LOW ) && !isDisplayTest ) {
      isDisplayTest = true;
      Serial.println("Display Test Press");
    }
  }
}


// Interrupt is called once a millisecond,
SIGNAL(TIMER0_COMPA_vect)
{
  unsigned long currentMillis = millis();
  mfs_display.update(currentMillis);
  if (isDisplayTest) displayTest(currentMillis);
  if (isLedTest) ledTest(currentMillis);
}

int ledTest(unsigned long currentMillis) {
    if (ledCurrentTest > 3 * ledOrigTests) {
      isLedTest = false;
      ledCurrentTest = 0;
      ledTestCounter = 0;
      mfs_led.reset();
      Serial.println("Led Test - done");
      return 0;
    }
    if (currentMillis - previousLedTime > ledTestPeriod) {
      previousLedTime = currentMillis;
      if (ledCurrentTest % ledOrigTests == 0) {
        if (ledTestCounter < mfs_led.size()) {
          mfs_led.setLed(ledTestCounter);
          ledTestCounter++;
        } else {
          ledCurrentTest++;
          ledTestCounter = 0;
        }
        return 0;
      }

      if (ledCurrentTest % ledOrigTests == 1) {
        if (ledTestCounter < mfs_led.size()) {
          mfs_led.clearLed(mfs_led.size() - ledTestCounter - 1);
          ledTestCounter++;
        } else {
          ledCurrentTest++;
          ledTestCounter = 0;
        }
        return 0;
      }
    }
};

uint8_t displayTest(unsigned long currentMillis) {
  if (displayCurrentTest < 0) {
    displayCurrentTest = 0;
    mfs_display.reset();
    mfs_display.updateOff();
    Serial.println("Display Test");
    return 0;
  }
  if (displayCurrentTest > 0) {
    isDisplayTest = false;
    mfs_display.updateOn();
    mfs_display.reset();
    Serial.println("Display Test - done");
    displayCurrentTest = -1;
    displayTestDigit = 0;
    displayTestCounter = 0;
    return 0;
  }
  if (currentMillis - previousDisplayTime > displayTestPeriod) {
    previousDisplayTime = currentMillis;
    if (displayCurrentTest == 0) {
      if (displayTestDigit < 4) {
        mfs_display.writeValue(displayTestDigit, 0);
        if (displayTestCounter < 6) {
          mfs_display.writeValue(displayTestDigit, (0x01 << displayTestCounter)^0xff);
          displayTestCounter++;
        } else {
          displayTestDigit++;
          displayTestCounter = 0;
        }
      } else {
        displayCurrentTest++;
      }
      return 0;
    }
  }
}
