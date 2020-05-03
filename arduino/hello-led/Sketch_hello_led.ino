/**
 Hello LED
Show message "Hello world!" 
with onboard LED and Morse code
*/

int msgSize = 54;

// Morze message.
// 3 = dash
// 1 = dot
// 0 = pause
int msg[54] = {
    1, 1, 1, 1, 0,     // "h"
    1, 0,              // "e"
    1, 3, 1, 1, 0,     // "l"
    1, 3, 1, 1, 0,     // "l"
    3, 3, 3, 0,        // "o"
    0,
    1, 3, 3, 0,        // "w"
    3, 3, 3, 0,        // "o"
    1, 3, 1, 0,        // "r"
    1, 3, 1, 1, 0,     // "l"
    3, 1, 1, 0,        // "d"
    3, 3, 1, 1, 3, 3, 0, // "!"
    0, 0, 0
 };

int speed = 150; // dot delay

void setup() {
  //Put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  
}

void loop() {
  //Put your main code here, to run repeatedly:

    for(int i = 0; i < msgSize; i++) {
        if(msg[i] != 0) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(msg[i] * speed);
        }
        else {
            digitalWrite(LED_BUILTIN, LOW);
            delay(2 * speed);
        }

        digitalWrite(LED_BUILTIN, LOW);
        delay(speed);
    }
}
