/**
 * @file mfs_control_by_uart.info
 * @section description
 * Control Multifunctional Shield outputs and inputs 
 * by command through TTY (Serial Console) in interactive mode
 * @section author PR0Maks
 * @section github
 * https://github.com/maks-ushakov/arduino-experiments/arduino/mfs_control_by_uart
 */
#include "MFS_LED.h"
#include "MFS_Display.h"
#include "commands.h"

#define LATCH_PIN  4
#define CLK_PIN    7
#define DATA_PIN   8

unsigned long previousTime = 0;
// Prompt symbols to interactive 
bool showPrompt = true;
String prompt = "> ";

MFS_LED mfs_led(4);
MFS_Display mfs_display(DATA_PIN, CLK_PIN, LATCH_PIN);

String cmdstr = ""; // command receive buffer

// 0 - manual by human, 1 - by device or if app send full string
uint8_t mode = 1;

void setup() {
  mfs_led.init();
  mfs_led.reset();
  ledTest(mfs_led);
  mfs_display.init();
  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
  OCR0A = 0xFF;
  TIMSK0 |= _BV(OCIE0A);

  mfs_display.setUpdateTime(500);
  mfs_display.reset();
  mfs_display.on();
  Serial.begin(115200);
  Serial.println(F("MF Shield controlled by Serial"));
}

bool result = 0;

void loop() {
  result = 0;
  unsigned long currentMillis = millis();
 // mfs_display.update(currentMillis);
  if (showPrompt) {
    Serial.println();
    Serial.print(prompt);
    showPrompt = false;
  }

  if (Serial.available() > 0) {
    if (mode) {
      // Do not update display for "device" (fast) mode
      // because serial receive process has errors when display update
      mfs_display.off();
    }
    char incoming = char(Serial.read());  //read until timeout
    Serial.print(incoming);
    cmdstr.concat(incoming);

    if (incoming == '\n' || incoming == '\r' || cmdstr.length() > 7) {
      cmdstr.trim();
      if (mode) {
        // Start updating display digits after stop simbols 
        mfs_display.on();
      }
      Serial.println();
      if ((cmdstr[0] == 'l' || cmdstr[0] == 'L')) {
        result = ledHandler(cmdstr, mfs_led);
      }
      else if ((cmdstr[0] == 'd' || cmdstr[0] == 'D')) {
        mfs_display.off(); // temporary off display
        result = displayHandler(cmdstr, &mfs_display);
        mfs_display.on();
      }
      else if ((cmdstr[0] == 'm' || cmdstr[0] == 'M')) {
        result = melodyHandler(cmdstr);
        }
      else if ((cmdstr[0] == '@')) {
        result = modeHandler(cmdstr);
      }
      else if ((cmdstr[0] == '?')) {
        Serial.println(F("Start with:\r\nD - display (D?)\n\rL - LED (L?)\n\rM - melody\r\n@ - Set mode"));
      }
      else {
        Serial.println(F("Unknown"));
      }

      //@TODO: Add commands for servo pins
      //@TODO: Add commands for PWM on "servo" pins
      //@TODO: Add commands to check "servo" pins
      //@TODO: Add command to check "sensors"
      //@TODO: Add command to check buttons
      //@TODO: Add command to get potenciometer value

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

int modeHandler(String command) {
  int size = command.length();
  if (size == 2 && command[1] == '?') {
    Serial.println(F("@<Source> - set mode"));
    Serial.println(F("  Source:\r\n    D - sent by device\r\n    M - manualy by human"));

    return 1;
  }
  if (size == 2 && (command[1] == 'd' || command[1] == 'D')) {
    mode = 1;
    Serial.println(F("Set mode: device (fast)"));
  }
  if (size == 2 && (command[1] == 'm' || command[1] == 'M')) {
    mode = 0;
    Serial.println(F("Set mode: human (by hand)"));
  }
  //@TODO: Add non interactive mode
}
