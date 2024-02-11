#include <WString.h>
#include "MFS_LED.h"
#include "MFS_Display.h"

#define TONE_PIN   3


int ledSetter(String, MFS_LED);
int ledGetter(String, MFS_LED);
/**
 * Parse command and execute
 */
int ledHandler(String, MFS_LED);
/**
 * Run test pattern
 */
void ledTest(MFS_LED);

/**
 * Run test pattern
 */
void displayTest(MFS_Display*);
int displaySetter(String, MFS_Display*);
/**
 * Parse command and execute
 */
int displayHandler(String, MFS_Display*);
/**
 * Parse command and execute
 */
int melodyHandler(String);