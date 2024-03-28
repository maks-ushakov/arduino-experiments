/**
* Mastermind LCDKeypad version
*
* https://en.wikipedia.org/wiki/Mastermind_(board_game)
*/
#include <LiquidCrystal.h>

#define LCD_RS 8
#define LCD_EN 9
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
#define KEYPAD A0

#define MAX_CODE_LENGTH 4

enum {
  KEY_SEL,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_UP,
  KEY_DOWN,
  KEY_UNPRESS
};

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
unsigned long previousCheckTime = 0;
unsigned long previousRenderTime = 0;
uint8_t codeLength = 4;
int maxCodeLength = MAX_CODE_LENGTH;
int8_t codeSource[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int8_t code[MAX_CODE_LENGTH];
int8_t guess[MAX_CODE_LENGTH];
int8_t previous[MAX_CODE_LENGTH];
int8_t currentDigit = 0;
uint8_t present = 0;
uint8_t rightGuess = 0;
bool win = false;

long renderTime = 100;
long checkTime = 200;
int sensorValue = KEY_UNPRESS;
int previousValue = KEY_UNPRESS;
int outValue = KEY_UNPRESS;
bool is_changed = false;

void setup()
{
  lcd.begin(16,2);
  lcd.print("Mastermind");
  lcd.setCursor(0, 1);
  lcd.print("Press any key");
  pinMode(KEYPAD, INPUT);
  while(sensorValue == KEY_UNPRESS) {
    sensorValue = parseKey(analogRead(KEYPAD));
  }
  randomSeed(analogRead(A5));
  shuffle();
  for (uint8_t i = 0; i < maxCodeLength; i++) {
    guess[i] = i;
    code[i] = codeSource[i];
  }
  lcd.cursor();
  lcd.blink();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mastermind");
  lcd.setCursor(5, 1);
  lcd.print("> ");
  lcd.print(printGuess());
  lcd.print(" <");
  lcd.setCursor(7, 1);
}

void loop()
{
  unsigned long currentTime = millis();
  if (currentTime - previousCheckTime > checkTime && !is_changed) {
    sensorValue = parseKey(analogRead(KEYPAD));
    if(sensorValue != outValue) {
      previousValue = outValue;
      outValue = sensorValue;
      is_changed = true;
      handleKey(outValue);
    }
  }

  if (currentTime - previousRenderTime > renderTime && is_changed) {
    lcd.clear();
    lcd.setCursor(0, 0);
    if(!win) {
      lcd.print(printPrevious());
      lcd.setCursor(9, 0);
      lcd.print("r:");
      lcd.print(rightGuess);
      lcd.print(" p:");
      lcd.print(present);
      lcd.setCursor(3, 1);
      lcd.print("> ");
      lcd.print(printGuess());
      lcd.print(" <");
      lcd.setCursor(5 + currentDigit, 1);
      is_changed = false;
    } else {
      lcd.noCursor();
      lcd.noBlink();
      lcd.print("Mastermind");
      lcd.setCursor(5, 1);
      lcd.print("WIN");
      is_changed = false;
    }
  }
}

void shuffle() {
  for (uint8_t i = 0; i < 10; i++) {
    uint8_t replace_index = random(0, 10);
    uint8_t temp = codeSource[i];
    codeSource[i] = codeSource[replace_index];
    codeSource[replace_index] = temp;
  }
}

int parseKey (int keyValue) {
  if (keyValue > 1020) {
    return KEY_UNPRESS;
  }
  if (keyValue > 720) {
    return KEY_SEL;
  }
  if (keyValue > 500) {
    return KEY_LEFT;
  }
  if (keyValue > 340) {
    return KEY_DOWN;
  }
  if (keyValue > 140) {
    return KEY_UP;
  }
  return KEY_RIGHT;
}

String printGuess() {
  String str = "";
  for (uint8_t i = 0; i < maxCodeLength; i++) {
    str.concat(guess[i]);
  }
  return str;
}

String printPrevious() {
  String str = "";
  for (uint8_t i = 0; i < maxCodeLength; i++) {
    str.concat(previous[i]);
  }
  return str;
}

String printKey(int key) {
  switch(key) {
    case KEY_SEL:   return "SELECT"; break;
    case KEY_LEFT:  return "LEFT"; break;
    case KEY_UP:    return "UP"; break;
    case KEY_DOWN:  return "DOWN"; break;
    case KEY_RIGHT:  return "RIGHT"; break;
    default: return "unpressed";
  }
}

void nextDigit() {
  currentDigit++;
  if (currentDigit > maxCodeLength - 1) {
    currentDigit = maxCodeLength - 1;
  }
}

void prevDigit() {
  currentDigit--;
  if (currentDigit < 0) {
    currentDigit = 0;
  }
}

void incDigit() {
  guess[currentDigit]++;
  if(guess[currentDigit] > 9) {
    guess[currentDigit] = 0;
  }
}

void decDigit() {
  guess[currentDigit]--;
  if(guess[currentDigit] < 0) {
    guess[currentDigit] = 9;
  }
}

void check() {
  rightGuess = 0;
  present = 0;
  for (int8_t i = 0; i < maxCodeLength; i++) {
    previous[i] = guess[i];
    for(int8_t j = 0; j < maxCodeLength; j++) {
      if(code[j] != guess[i]) continue;
      if(j == i) {
        rightGuess++;
        continue;
      }
      present++;
    }
  }
  if (rightGuess == maxCodeLength) {
    win = true;
  }
}

void pass() {};

void handleKey(uint8_t key) {
  switch(key) {
    case KEY_SEL:   check(); break;
    case KEY_LEFT:  prevDigit(); break;
    case KEY_RIGHT: nextDigit(); break;
    case KEY_UP:    incDigit(); break;
    case KEY_DOWN:  decDigit(); break;

    default: pass();
  }
}