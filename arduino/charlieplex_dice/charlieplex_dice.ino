/**
* DIce variation fot ATTiny85 (board)
* On Board PB3 and PB4 connected to USB
* So you have only 4 pins for LEDs and Button
* Charlieplexed LED connection allows you use
* 3 pins to handle max 6 LEDs.
* For Dice you need only 4.
* Dice LEDs turns dinamically.
*
* Connections:
* PB0 - PB2 : LEDs
* PB5 : button
*/

#define DICE_0 5
#define DICE_1 6
#define DICE_2 7
#define DICE_BUTTON 2

uint8_t Dice_LEDS[4] = {
  0, // L1
  0, // L2
  0, // L4
  0  // L6
};
uint8_t currentIndex = 0;
uint8_t number = 1;

unsigned long previousRenderTime = 0;
unsigned long previousHandleTime = 0;
long renderPeriod = 1;
long handlePeriod = 100;

void setup()
{
  pinMode(DICE_0, INPUT);
  pinMode(DICE_1, INPUT);
  pinMode(DICE_2, INPUT);
  pinMode(DICE_BUTTON, INPUT_PULLUP);
}

void loop()
{
  unsigned long currentTime = millis();
  // Render time
  if(currentTime - previousRenderTime > renderPeriod) {
  	renderDice();
    previousRenderTime = currentTime;
  }
  
  // Check button time
  if(currentTime - previousHandleTime > handlePeriod 
     && (digitalRead(DICE_BUTTON) == LOW)) {
    previousHandleTime = currentTime;
    // If pressed generate new number
    number = random(1, 7);
    setDice(number);
  }	
}

void setDice(uint8_t number) {
  Dice_LEDS[0] = int(number % 2); // L1
  Dice_LEDS[1] = int(number > 1); // L2
  Dice_LEDS[2] = int(number > 3); // L4
  Dice_LEDS[3] = int(number >= 6); // L6
}

/**
* Dinamically turn LED
*/
uint8_t renderDice() {
  nextIndex();
  uint8_t currentLedState = Dice_LEDS[currentIndex];
  if (!currentLedState) {
    return 0;
  }
  shutdownDice();
  if (currentIndex == 0) {
    pinMode(DICE_1, OUTPUT);
    pinMode(DICE_2, OUTPUT);
    digitalWrite(DICE_1, HIGH);
    digitalWrite(DICE_2, LOW);
  }
  
  if (currentIndex == 1) {
    pinMode(DICE_0, OUTPUT);
    pinMode(DICE_1, OUTPUT);
    digitalWrite(DICE_0, HIGH);
    digitalWrite(DICE_1, LOW);
  }
  
  if (currentIndex == 2) {
    pinMode(DICE_0, OUTPUT);
    pinMode(DICE_1, OUTPUT);
    digitalWrite(DICE_0, LOW);
    digitalWrite(DICE_1, HIGH);    
  }
  
  if (currentIndex == 3) {
    pinMode(DICE_0, OUTPUT);
    pinMode(DICE_2, OUTPUT);
    digitalWrite(DICE_0, LOW);
    digitalWrite(DICE_2, HIGH);
  }
  return 0;
}

void shutdownDice() {
  pinMode(DICE_0, INPUT);
  pinMode(DICE_1, INPUT);
  pinMode(DICE_2, INPUT);
}

void nextIndex() {
  currentIndex++;
  if (currentIndex > 3) {
    currentIndex = 0;
  }
}
