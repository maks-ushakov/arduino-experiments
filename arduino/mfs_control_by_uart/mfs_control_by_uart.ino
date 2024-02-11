#include "pitches.h"

#define LATCH_PIN  4
#define CLK_PIN    7
#define DATA_PIN   8

#define TONE_PIN   3

class MFS_LED
{
  private:
    int leds[4];
    int numLED;
    int startLED = 10;
    int checkIndex(int ledIndex);
  public:
    MFS_LED(int n);
    void init();
    int size();
    void setLed(int ledIndex);
    void clearLed(int ledIndex);
    void toggleLed(int ledIndex);
    int getState(int ledIndex);
    String getPrintState(int ledIndex);
    void reset();
};
const byte SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};
const byte SEGMENT_MAP_DIGIT[] = {
  //HGFEDCBA
  0xC0, // 0
  0xF9, // 1
  0xA4, // 2
  0xB0, // 3
  0x99, // 4
  0x92, // 5
  0x82, // 6
  0xF8, // 7
  0X80, // 8
  0X90, // 9
  0b10001000, // A
  0b10000011, // B
  0b11000110, // C
  0b10100001, // D
  0b10000110, // E
  0b10001110, // F
  }; 

class MFS_Display
{
  private:
    byte latchPin;
    byte clkPin;
    byte dataPin;
    bool needUpdate = false;
    long updateTime = 50;
    unsigned long previousMillis;
    byte displayData[4];
  public:
  MFS_Display(byte data, byte clk, byte latch);
    void init();
    void writeDigit(byte index, byte data);
    void setData(byte index, byte data);
    void show();
    byte showInt(long number, byte format);
    void showOverValue();
    void writeValue(byte segment, byte value);
    void reset();
    void update(unsigned long);
    void setUpdateTime(long);
    void updateOn();
    void updateOff();
    void off();
    void on();
};
unsigned long previousTime = 0;
bool showPrompt = true;
String prompt = "> ";
MFS_LED mfs_led(4);
MFS_Display mfs_display(DATA_PIN, CLK_PIN, LATCH_PIN);
String cmdstr = "";

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

void setup() {
  mfs_led.init();
  mfs_led.reset();
  ledTest();
  mfs_display.init();
  // displayTest();
  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
//  OCR0A = 0xAF;
//  TIMSK0 |= _BV(OCIE0A);
  mfs_display.reset();
  mfs_display.setUpdateTime(100);
  Serial.begin(115200);
  Serial.println(F("MF Shield controlled by Serial"));
}

bool result = 0;

void loop() {
  result = 0;
  unsigned long currentMillis = millis();
  mfs_display.update(currentMillis);
  if (showPrompt) {
    Serial.println();
    Serial.print(prompt);
    showPrompt = false;
  }

  if (Serial.available() > 0) {

    char incoming = char(Serial.read());  //read until timeout
    Serial.print(incoming);
    cmdstr.concat(incoming);

    if (incoming == '\n' || incoming == '\r' || cmdstr.length() > 7) {
      cmdstr.trim();
      mfs_display.off();
      Serial.println();
      if ((cmdstr[0] == 'l' || cmdstr[0] == 'L')) {
        result = ledHandler(cmdstr);
      }
      else if ((cmdstr[0] == 'd' || cmdstr[0] == 'D')) {
        result = displayHandler(cmdstr);
      }
      else if ((cmdstr[0] == 'm' || cmdstr[0] == 'M')) {
        result = melodyHandler(cmdstr);
      }    else if ((cmdstr[0] == '?')) {
        Serial.println(F("Start with:\r\nD - display (D?)\n\rL - LED (L?)\n\rM - melody\r\n"));
      }
        else
      {
        Serial.println(F("Unknown"));
      }
      mfs_display.on();
      if (result == 1) {
        Serial.println(F("Done"));
      }
      showPrompt = true;
      cmdstr = "";
    }
  }
}

// Interrupt is called once a millisecond,
SIGNAL(TIMER0_COMPA_vect)
{
unsigned long currentMillis = millis();
 mfs_display.update(currentMillis);
}
void ledTest() {
  for(int i = 0; i < mfs_led.size(); i++) {
    mfs_led.setLed(i);
    delay(100);
  }

  for(int i = mfs_led.size() - 1; i >= 0 ; i--) {
    mfs_led.clearLed(i);
    delay(100);
  }
}

int ledHandler(String command) {
  int size = command.length();
  if (size == 2 && isDigit(command[1])) {
    return ledGetter(command);
  }
  if (size == 2 && command[1] == '!') {
    ledTest();
    Serial.println(F("LED Test"));
    return 1;
  }
  if (size == 2 && command[1] == '%') {
    mfs_led.reset();
    Serial.println(F("All LEDs have turned OFF"));
    return 1;
  }
  if (size == 2 && command[1] == '?') {
    Serial.println(F("L<led_number 0-4>:<STATE 0 or 1> - ON and OFF LEDs"));
    Serial.println(F("L<led_number 0-4> -  get state of LEDs"));
    Serial.println(F("L! - run led test\r\nL% - reset all LEDs (OFF)\r\nL? - help"));
    return 1;
  }
  if (size > 3 && command[2] == ':') {
    return ledSetter(command);
  }
  Serial.println("Wrong format. use L? for help");
  return 0;
}

int ledSetter(String command) {
    String sLed = String(command[1]);
    String sState = String(command[3]);

    int led = sLed.toInt();
    int state = sState.toInt();
    Serial.print("LED");
    Serial.print(led);
    Serial.print(" have  turned ");
    if (state > 0) {
      mfs_led.setLed(led);
      Serial.println("ON");
    } else {
      mfs_led.clearLed(led);
      Serial.println("OFF");
    }
    return 1;
}

int ledGetter(String command) {
    String sLed = String(command[1]);
    int led = sLed.toInt();
    Serial.print(F("LED"));
    Serial.print(led);
    Serial.print(F(" is "));
    Serial.println(mfs_led.getPrintState(led));
    return 1;
}

int displayHandler(String command) {
  int size = command.length();
  if (size == 2 && command[1] == '?') {
    Serial.println(F("D<Format>:<number> - show number"));
    Serial.println(F("  Formats:\r\n    D - decimal\r\n    H - hexadecimal"));
    Serial.println(F("D! - run led test\r\nD% - reset all LEDs (OFF)\r\nD? - help"));
    return 1;
  }
    if (size == 2 && command[1] == '!') {
    displayTest();
    Serial.println("Display Test");
    return 1;
  }
  if (size == 2 && command[1] == '%') {
    mfs_display.reset();
    Serial.println(F("Display have turned OFF"));
    return 1;
  }
  if (size == 2 && command[1] == '@') {
    mfs_display.update(millis());
    Serial.println(F("Display have been updated"));
    return 1;
  }
  if (size > 3 && command[2] == ':') {
    return displaySetter(command);
  }
  Serial.println(F("Wrong format. use D? for help"));
  return 0;
}

void displayTest() {
  mfs_display.reset();
  for (int digit = 0; digit < 4; digit++) {
    mfs_display.writeValue(digit, 0);
    for (int count = 0; count < 6; count++) {
      mfs_display.writeValue(digit, (0x01 << count)^0xff);
      delay(100);
    }
  }
  delay(100);
    for (int digit = 0; digit < 4; digit++) {
    mfs_display.writeValue(digit, 0);
    for (int count = 0; count < 16; count++) {
      mfs_display.writeDigit(digit, count);
      delay(100);
    }
  }
  mfs_display.reset();
}

int displaySetter(String command) {
  int size = command.length();
  String sNumber = command.substring(3);
  int number = sNumber.toInt();
  int format = 2;
    if (command[1] == 'D' || command[1] == 'd') {
      format = 10;
    }
    if (command[1] == 'H' || command[1] == 'h') {
      format = 16;
    }
  mfs_display.showInt(number, format);
  return 1;
}

int melodyHandler(String command) {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(TONE_PIN, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(TONE_PIN);
  }
}
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
    for (byte i = 0; i < 4; i++) {
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
    for (byte i = 0; i < 4; i++) {
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

MFS_Display::MFS_Display(byte data, byte clk, byte latch)
  {
    latchPin = latch;
    dataPin = data;
    clkPin = clk;
    updateTime = 50;
    previousMillis = 0;
  };
  
  void MFS_Display::init(){
    pinMode(latchPin,OUTPUT);
    pinMode(clkPin,OUTPUT);
    pinMode(dataPin,OUTPUT); 

    writeValue(0,255);
  };

  void MFS_Display::setUpdateTime(long time) {
    updateTime = time;
  }
  
  void MFS_Display::writeDigit(byte segment, byte data)
  {
    writeValue(segment, SEGMENT_MAP_DIGIT[data]);
  };

  byte MFS_Display::showInt(long number, byte format) {
    reset();
    long limit = long(format) * long(format) * long(format) * long(format) - 1;
    if (number > limit) {
      showOverValue();
      return 0;
    }
    for (int i = 3; i >= 0; i--) {
      int last = number % format;
      setData(i, SEGMENT_MAP_DIGIT[last]);
      number /= format;
      if (number == 0) {
        break;
      }
    }
    updateOn();
    return 1;
  }

  void MFS_Display::showOverValue() {
    for (byte i = 0; i < 4; i++){
    	setData(i, 0b10111111);
    }
    updateOn();
  }
  
  void MFS_Display::setData(byte index, byte data) {
  	displayData[index] = data;
  };
  
  void MFS_Display::show()
  {
    for (byte i = 0; i < 4; i++){
    	writeValue(i, displayData[i]);
    }
  };
  
  void MFS_Display::writeValue(byte segment, byte value)
  {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clkPin, MSBFIRST, value);
    shiftOut(dataPin, clkPin, MSBFIRST, SEGMENT_SELECT[segment]);
    digitalWrite(latchPin, HIGH);
  };

  void MFS_Display::reset()
  {
    updateOff();
    for (int digit = 0; digit < 4; digit++) {
      writeValue(digit, 0xff);
      displayData[digit] = 0xff;
    }
  };

  void MFS_Display::off()
  {
    updateOff();
    for (int digit = 0; digit < 4; digit++) {
      writeValue(digit, 0xff);
    }
  };

  void MFS_Display::on()
  {
    updateOn();
  };

void MFS_Display::updateOn() {
  needUpdate = true;
}

void MFS_Display::updateOff() {
  needUpdate = false;
}
  void MFS_Display::update(unsigned long currentMillis)
  {
    // check to see if it's time to change the state of the LED
    if (needUpdate && (currentMillis - previousMillis >= updateTime))
    {
      show();
    }
};