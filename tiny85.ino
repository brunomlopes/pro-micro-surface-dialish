

/*
  Copyright (c) 2017 wind-rider
  See the readme for credit to other people.
  Surface dial example
  
  Use an encoder and a button to create a Surface Dial-compatible device.
  See the connection diagram how to wire it up.
  Please note that:
   - I tested it using an Arduino Pro Micro; TinkerCad didn't have that in its component library
   - you obviously don't need a motor, but TinkerCad didn't have a separate encoder
  The encoder processing code is coming from https://www.allwinedesigns.com/blog/pocketnc-jog-wheel
*/

#include "HID-Project.h"

// input pins for encoder channel A and channel B
int pinA = 2;
int pinB = 3;

// input pin for pushbutton
int pinButton = 16;
int pinSecondButton = 15;

bool previousButtonValue = false;
bool previousSecondButtonValue = false;

volatile int previous = 0;
volatile int counter = 0;

void setup()
{
   pinMode(pinA, INPUT);
   pinMode(pinB, INPUT);
   pinMode(pinButton, INPUT);
   // pinMode(pinSecondButton, INPUT);

   digitalWrite(pinA, HIGH); //turn pullup resistor on
   digitalWrite(pinB, HIGH); //turn pullup resistor on

   attachInterrupt(digitalPinToInterrupt(pinA), changed, CHANGE);
   attachInterrupt(digitalPinToInterrupt(pinB), changed, CHANGE);
   Serial.begin(9600);

   SurfaceDial.begin();
}

void changed()
{

   int MSB = digitalRead(pinA); //MSB = most significant bit
   int LSB = digitalRead(pinB); //LSB = least significant bit

   int encoded = (MSB << 1) | LSB;      //converting the 2 pin value to single number
   int sum = (previous << 2) | encoded; //adding it to the previous encoded value

   if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
      counter--;
   if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
      counter++;

   previous = encoded; //store this value for next time
}

#define DialRotateDelta 25
void loop()
{
   bool buttonValue = digitalRead(pinButton);
   if (buttonValue != previousButtonValue)
   {
      if (buttonValue)
      {
         SurfaceDial.press();
      }
      else
      {
         SurfaceDial.release();
      }
      previousButtonValue = buttonValue;
   }
   
   if (counter >= 1)
   {
      SurfaceDial.rotate(DialRotateDelta);
      counter -= 1;
   }
   else if (counter <= -1)
   {
      SurfaceDial.rotate(-DialRotateDelta);
      counter += 1;
   }
}
