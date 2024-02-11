#include <WString.h>

#ifndef MFS_LED_H_
#define MFS_LED_H_ 300

#define NUM_LEDS 4
#define START_LED_PIN 10
/**
* @class
* dealing with board LEDs by its number
*/
class MFS_LED
{
  private:
    //LED pins
    int leds[NUM_LEDS];
    int numLED;
    // 
    int startLED = START_LED_PIN;
    int checkIndex(int ledIndex);
    //@TODO: add state for fast led update
  public:
    /**
     * Constructor.
     * @param numLEDs - Number of used LEDs (1 - 4)
     */
    MFS_LED(int numLEDs);
    void init();
    int size();
    /**
     * Make LED active (ON)
     *
     * @param ledIndex - Number of led
     */
    void setLed(int ledIndex);
    /**
     * Make LED unactive (OFF)
     *
     * @param ledIndex - Number of led
     */
    void clearLed(int ledIndex);
    /**
     * Toggle LED state (ON/OFF)
     *
     * @param ledIndex - Number of led
     */
    void toggleLed(int ledIndex);
    /**
     * Get state of LED
     *
     * @param ledIndex - Number of led
     */
    int getState(int ledIndex);
    /**
     * Get readable state of LED
     *
     * @param ledIndex - Number of led
     * @return string ON or OFF, NONE if wrong index
     */
    String getPrintState(int ledIndex);
    /**
    * Shut down all LEDs
    */
    void reset();
    //@TODO: add method for fast led update
};

#endif