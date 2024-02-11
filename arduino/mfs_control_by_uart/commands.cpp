#include "Arduino.h"
#include <WString.h>
#include "commands.h"
#include "pitches.h"

void ledTest(MFS_LED mfs_led) {
  for(int i = 0; i < mfs_led.size(); i++) {
    mfs_led.setLed(i);
    delay(100);
  }

  for(int i = mfs_led.size() - 1; i >= 0 ; i--) {
    mfs_led.clearLed(i);
    delay(100);
  }
}

int ledHandler(String command, MFS_LED mfs_led) {
  int size = command.length();
  if (size == 2 && isDigit(command[1])) {
    return ledGetter(command, mfs_led);
  }
  if (size == 2 && command[1] == '!') {
    ledTest(mfs_led);
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
    return ledSetter(command, mfs_led);
  }
  Serial.println("Wrong format. use L? for help");
  return 0;
}

int ledSetter(String command, MFS_LED mfs_led) {
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

int ledGetter(String command, MFS_LED mfs_led) {
    String sLed = String(command[1]);
    int led = sLed.toInt();
    Serial.print(F("LED"));
    Serial.print(led);
    Serial.print(F(" is "));
    Serial.println(mfs_led.getPrintState(led));
    return 1;
}
int displayHandler(String command, MFS_Display * display) {
  int size = command.length();
  if (size == 2 && command[1] == '?') {
    Serial.println(F("D<Format>:<number> - show number"));
    Serial.println(F("  Formats:\r\n    D - unsinged decimal\r\n    S - signed decimal byte \r\n    H - unsigned hexadecimal"));
    Serial.println(F("D! - run display test\r\nD% - reset all digits (OFF)\r\nD? - help"));
    return 1;
  }
    if (size == 2 && command[1] == '!') {
    displayTest(display);
    Serial.println("Display Test");
    return 1;
  }
  if (size == 2 && command[1] == '%') {
    display->reset();
    Serial.println(F("Display have turned OFF"));
    return 1;
  }
  if (size == 2 && command[1] == '@') {
    display->update(millis());
    Serial.println(F("Display have been updated"));
    return 1;
  }
  if (size > 3 && command[2] == ':') {
    return displaySetter(command, display);
  }
  Serial.println(F("Wrong format. use D? for help"));
  return 0;
}

void displayTest(MFS_Display * mfs_display) {
  mfs_display->reset();
  for (int digit = 0; digit < 4; digit++) {
    mfs_display->writeValue(digit, 0);
    for (int count = 0; count < 6; count++) {
      mfs_display->writeValue(digit, (0x01 << count)^0xff);
      delay(50);
    }
  }
  delay(50);
    for (int digit = 0; digit < 4; digit++) {
    mfs_display->writeValue(digit, 0);
    for (int count = 0; count < 16; count++) {
      mfs_display->writeDigit(digit, count);
      delay(50);
    }
  }
  mfs_display->reset();
}

int displaySetter(String command, MFS_Display * display) {
  int size = command.length();
  String sNumber = command.substring(3);
  long number = sNumber.toInt();
  int format = 2;
  bool has_sign = false;
    if (command[1] == 'D' || command[1] == 'd') {
      format = 10;
    }
    if (command[1] == 'H' || command[1] == 'h') {
      format = 16;
    }
    if (command[1] == 'S' || command[1] == 's') {
      format =10;
      has_sign = true;
    }
  display->showInt(number, format, has_sign);
  return 1;
}

// From Tone Example
// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

int melodyHandler(String command) {
  //TODO: Add more melodies
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
