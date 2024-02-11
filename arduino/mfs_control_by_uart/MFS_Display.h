/**
* Display driver
* In Multifunctional shield display 
* controlled by 2 74hc595.
* This driver can help to deal with them. 
*/
#include <WString.h>

#ifndef MFS_DISPLAY_H_
#define MFS_DISPLAY_H_ 300

#define NUM_DIGIT 4
/**
 * Display class
 */
class MFS_Display
{
  private:
    byte latchPin;
    byte clkPin;
    byte dataPin;
    int8_t currentDigit = 0;
    bool needUpdate = false;
    long updateTime = 50;
    unsigned long previousMillis;
    // Store for command symbol's code for each digit
    byte displayData[NUM_DIGIT];
    // update current digit number
    void nextDigit(void);
  public:
  /*!
    @brief Constructor.
    @param data - Pin number dor serial data
    @param clk  - Pin number for strobe impulse
    @param latch - Pin number for latch
   */
  MFS_Display(byte data, byte clk, byte latch);
    /**
     * Init pins for display
     */
    void init(void);
    /*!
      @brief Show digit directly, without digit store
      @param byte digit - Digit number
      @param byte value - value to show (0 - 15)
    */
    void writeDigit(byte, byte);
    /*!
      @brief Communicate with 74HC595 to write value to digit directly
      @param byte digit - Digit number
      @param byte value - & segment Code to show
     */
    void writeValue(byte segment, byte value);
    /*!
      @brief Put 7-segment code value to digit store
      @param byte digit - Digit number
      @param byte value - 7-segment code value
     */
    void setData(byte, byte);
    /*!
     @brief Show symbol code on current digit
     */
    void show(void);
    /*!
      @brief Parse number and fill digit store
      @param long number - number to show
      @param byte base - Number base (10 or 16)
      @param bool show_sign - Show minus symbol for 1 byte number
     */
    byte showInt(long, byte, bool);
    /**
     * Show symbols if number value is bigger than 4 digit.
     */
    void showOverValue(void);
    /**
     * Clean digit store and off display
     */
    void reset(void);
    /**
      @brief Update current digit in update time
      @param time - Current time in ms
     */
    void update(unsigned long);
    /*!
      @brief Change update time
      @param time - New update time in ms
     */
    void setUpdateTime(long);
    /**
     * Allow update process
     */
    void updateOn();
    /**
     * Deny update process
     */
    void updateOff();
    /**
     * Turn off display
     */
    void off();
    /**
     * Turn on display
     */
    void on();
    /**
     * Get readable on/off display status
     */ 
    String getStatus(void);
    /**
     * Print information about settings and digit store
     */
    void info(void);
};

#endif

