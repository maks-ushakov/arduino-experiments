#include "Arduino.h"
#include <WString.h>
#include "MFS_Display.h"

#define DIGIT_OFF 0xFF
#define MINUS_SIGN 0b10111111
#define OVER_SIGN 0b10111111
#define NUM_DIGIT 4

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

    // writeValue(0,255);
  };

  void MFS_Display::setUpdateTime(long time) {
    updateTime = time;
  }
  
  void MFS_Display::writeDigit(byte segment, byte data)
  {
    writeValue(segment, SEGMENT_MAP_DIGIT[data]);
    //setData(segment, SEGMENT_MAP_DIGIT[data]);
  };

  byte MFS_Display::showInt(long number, byte format, bool show_sign) {
    reset();
    uint8_t start = 3;
    uint8_t end = 0;

    long limit = long(format) * long(format) * long(format) * long(format) - 1;
    if (number > limit) {
      showOverValue();
      return 0;
    }
    if (show_sign) {
      end = 1;
      int8_t tmp = byte(number);
      if (tmp < 0) {
        setData(0, MINUS_SIGN);
        number = - tmp;
      }
    }
    for (int i = start; i >= end; i--) {
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
    	setData(i, OVER_SIGN);
    }
    updateOn();
  }

  byte MFS_Display::showNice(uint8_t number) {
    uint8_t limit = 99;
    uint8_t format = 10;
    if (number > limit) {
      number = number % 100;
    }
    setData(0, MINUS_SIGN);
    setData(3, MINUS_SIGN);
    uint8_t start = 2, end = 1;
    for (int i = start; i >= end; i--) {
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

  byte MFS_Display::showIntWithSuffix(int8_t number, uint8_t suffix) {
    showInt(number * 10,10 , true);
    setData(3, suffix);
    return 1;
  }

    byte MFS_Display::showUIntWithSuffix(uint8_t number, uint8_t suffix) {
    showInt(number * 10,10 , false);
    setData(3, suffix);
    return 1;
  }
  
  void MFS_Display::setData(byte index, byte data) {
  	displayData[index] = data;
  };
  
  void MFS_Display::show()
  {
    	writeValue(currentDigit, displayData[currentDigit]);
      nextDigit();
  };

  void MFS_Display::nextDigit()
  {
    currentDigit++;
    if (currentDigit > 3) {
      currentDigit = 0;
    }
  }
  
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

String MFS_Display::getStatus()
{
  return needUpdate ? "ON" : "OFF";
}

void MFS_Display::info()
{
  Serial.print(F("Display\n\rPINS:\r\nCLK: "));
  Serial.print(clkPin);
  Serial.print(F("; DATA: "));
  Serial.print(dataPin);
  Serial.print(F("; Latch: "));
  Serial.println(latchPin);
  Serial.print(F("Update: "));
  Serial.println(getStatus());
  Serial.print(F("Update time: "));
  Serial.println(updateTime);
  Serial.print(F("Current digit: "));
  Serial.println(currentDigit);
  Serial.println(F("Data by digit:"));
  Serial.print(displayData[0], HEX);
  Serial.print(F(" : "));
  Serial.print(displayData[1], HEX);
  Serial.print(F(" : "));
  Serial.print(displayData[2], HEX);
  Serial.print(F(" : "));
  Serial.println(displayData[3], HEX);
}