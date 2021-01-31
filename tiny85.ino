

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
   // int A = digitalRead(pinA);
   // int B = digitalRead(pinB);

   // int current = (A << 1) | B;
   // int combined = (previous << 2) | current;

   // if (combined == 0b0010 ||
   //     combined == 0b1011 ||
   //     combined == 0b1101 ||
   //     combined == 0b0100)
   // {
   //    counter++;
   // }

   // if (combined == 0b0001 ||
   //     combined == 0b0111 ||
   //     combined == 0b1110 ||
   //     combined == 0b1000)
   // {
   //    counter--;
   // }

   // previous = current;

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
   
   // bool secondButtonValue = digitalRead(pinSecondButton);
   // Serial.println(secondButtonValue);
   // if (secondButtonValue != previousSecondButtonValue)
   // {
   //    if (secondButtonValue)
   //    {
   //       SurfaceDial.press();
   //    }
   //    else
   //    {
   //       SurfaceDial.release();
   //    }
   //    previousSecondButtonValue = secondButtonValue;
   // }

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

// int RXLED = 17; // The RX LED has a defined Arduino pin

// int encoderPinA = 2;
// int encoderPinB = 3;
// int buttonPin = 4;

// volatile int lastEncoded = 0;
// volatile long encoderValue = 0;

// long lastencoderValue = 0;

// int lastMSB = 0;
// int lastLSB = 0;

// long readEncoderValue(void)
// {
//    return encoderValue / 4;
// }

// boolean isButtonPushDown(void)
// {
//    if (!digitalRead(buttonPin))
//    {
//       delay(5);
//       if (!digitalRead(buttonPin))
//          return true;
//    }
//    return false;
// }

// void setup()
// {

//    Serial.begin(9600); //This pipes to the serial monitor

//    pinMode(encoderPinA, INPUT);
//    pinMode(encoderPinB, INPUT);
//    pinMode(buttonPin, INPUT);

//    digitalWrite(encoderPinA, HIGH); //turn pullup resistor on
//    digitalWrite(encoderPinB, HIGH); //turn pullup resistor on

//    //call updateEncoder() when any high/low changed seen
//    //on interrupt 0 (pin 2), or interrupt 1 (pin 3)
//    attachInterrupt(digitalPinToInterrupt(encoderPinA), updateEncoder, CHANGE);
//    attachInterrupt(digitalPinToInterrupt(encoderPinB), updateEncoder, CHANGE);
// }

// void loop()
// {
//    //Do stuff here

//    if (isButtonPushDown())
//    {
//       Serial.println("you push button down!!!");
//    }
//    Serial.println(readEncoderValue());
//    delay(50); //just here to slow down the output, and show it will work  even during a delay
// }

// void updateEncoder()
// {
//    int MSB = digitalRead(encoderPinA); //MSB = most significant bit
//    int LSB = digitalRead(encoderPinB); //LSB = least significant bit

//    int encoded = (MSB << 1) | LSB;         //converting the 2 pin value to single number
//    int sum = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

//    if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
//       encoderValue--;
//    if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
//       encoderValue++;

//    lastEncoded = encoded; //store this value for next time
// }