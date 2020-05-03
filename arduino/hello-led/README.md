# Hello LED

Show message with onboard LED and Morse code.

Result on [youtube](https://youtu.be/jcGyNiuIIEo)

## Equipments

- Arduino board

## Statemens
Based on Blink Arduino sketch.

Some C Statements was added:

- array with pseudo Morse code

- for loop

- if-else statement

## How it works

The "dot" has a smalest delay so call it "1 times".

The "dash" has big delay during 3 times.

We have 1 time pause between each dot or dash in word and 3 time pause between each worlds.

Pseudo Morse code array (msg) has dots, dashes and word pause.

Dots was written as 1, dashes as 3 and word pause as 0.

