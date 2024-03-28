/**
 * @file matrix_dice.info
 * @section description
 * Dice with matrix NAX7219
 * @section author PR0Maks
 * @section github
 * https://github.com/maks-ushakov/arduino-experiments/arduino/matrix_dice
 */
#include <MATRIX7219.h>

#define CS_PIN 10
#define DATA_PIN 11
#define CLK_PIN 13
#define COUNT 2

#define INT0_PIN 2

uint8_t dice_numbers [6][8] = {
  {0x00, 0x00, 0x00, 0x18,0x18, 0x00, 0x00, 0x00},
  {0xc0, 0xc0, 0x00, 0x00,0x00, 0x00, 0x03, 0x03},
  {0xc0, 0xc0, 0x00, 0x18,0x18, 0x00, 0x03, 0x03},
  {0xc3, 0xc3, 0x00, 0x00,0x00, 0x00, 0xc3, 0xc3},
  {0xc3, 0xc3, 0x00, 0x18,0x18, 0x00, 0xc3, 0xc3},
  {0xdb, 0xdb, 0x00, 0x00,0x00, 0x00, 0xdb, 0xdb}
};

uint8_t dice_numbers2 [6][8] = {
  {0x80, 0x80, 0x80, 0x88,0x80, 0x80, 0x80, 0xff},
  {0x80, 0x82, 0x80, 0x80,0x80, 0xA0, 0x80, 0xff},
  {0x80, 0x82, 0x80, 0x88,0x80, 0xA0, 0x80, 0xff},
  {0x80, 0xA2, 0x80, 0x80,0x80, 0xA2, 0x80, 0xff},
  {0x80, 0xA2, 0x80, 0x88,0x80, 0xA2, 0x80, 0xff},
  {0x80, 0xa2, 0x80, 0xa2,0x80, 0xa2, 0x80, 0xff}
};

uint8_t counter = 1;
uint8_t matrix_number = COUNT;
volatile uint8_t buttonState = LOW;
unsigned long previousTime = 0;
MATRIX7219 matrix(DATA_PIN, CS_PIN, CLK_PIN, COUNT);


void setup() {
  // put your setup code here, to run once:
  matrix.begin();
  matrix.clear();
  matrix.setBrightness(3);
  matrix.setReverse(true);
  diceTest();
  matrix.clear();
  randomSeed(analogRead(A5));
  pinMode(INT0_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INT0_PIN), switchState, FALLING);
  Serial.begin(9600);
}

void loop() {
   unsigned long currentMillis = millis();
  if(currentMillis - previousTime > 250) {
    if (buttonState) {
        matrix.clear();
        for(uint8_t i = 1; i <= matrix_number; i++ ) {
          int randNumber = 1 + random(5);
          showDiceNumber(randNumber-1, i);
        }
        buttonState = LOW;
      }
    }
  }

uint8_t showDiceNumber(uint8_t number, uint8_t matrix_no) {
  if (matrix_number > 1) {
    showDiceNumber2(number, matrix_no);
    return 0;
  }
  showDiceNumber1(number, matrix_no);
}

void showDiceNumber1(uint8_t number, uint8_t matrix_no) {
  for(uint8_t i = 0; i < 8; i++) {
    matrix.setRow(i + 1, dice_numbers[number][i], matrix_no);
  }
}

void showDiceNumber2(uint8_t number, uint8_t matrix_no) {
  for(uint8_t i = 0; i < 8; i++) {
    matrix.setRow(i + 1, dice_numbers2[number][i], matrix_no);
  }
}

void diceTest() {
  for(uint8_t i = 1; i <= 6; i++) {
    showDiceNumber(i-1, 1);
    delay(250);
  }
}

void switchState() {
  buttonState = HIGH;
}
