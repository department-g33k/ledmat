/* 
  Stackmat Display using Arduino Nano.
*/

#include <SoftwareSerial.h>

//Libraries for display:
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include<SPI.h>
#include "font.h"

// The magical defines:
#define state_is_valid(s) (s=='I' || s=='A' || s==' ' || s=='S' || s=='L' || s=='R' || s=='C')
#define checksum(data) ( (data[1]-'0')+(data[2]-'0')+(data[3]-'0')+(data[4]-'0')+(data[5]-'0')+(data[6]-'0')+64 )
#define append(array, length, item) { for(uint8_t i=0; i<length-1; i++) { array[i] = array[i+1]; };  array[length-1] = item; }

//Constants for display bits:
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10
#define BRIGHT_PIN A0

MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

int dispBright = 8; // default display brightness
char disp[10];       // display buffer
const int ledmatRXPin = 2;        // RX pin from ledmat.
const int ledmatTXPin = 3;        // Dont connect anything to pin 3 of the Arduino.
const int ledmatInvertSerial = 1; // ledmat uses inverted serial - this needs to be 1.

// Setup software serial:
SoftwareSerial ledMatSerial(ledmatRXPin, ledmatTXPin, ledmatInvertSerial); 

// Input buffer for the last seven received bytes
// state, min, sec1, sec2, msec1, msec2, msec3, checksum
char buf[9]={0,0,0,0,0,0,0,0,0};
byte inCount=0; // Counter for how many bytes we have received.

void setup() {
  ledMatSerial.begin(1200); // enable the ledmat serial at 1200bps.
                            // We are using the software serial library because it supports
                            // inverted serial, while the hardware serial requires an
                            // additional ic.

  // Intialize the LED Display object:
  myDisplay.begin();
  myDisplay.setFont(ledmat);    // Use our special font.
  myDisplay.setCharSpacing(0);  // Spacing is already included in the font, so we don't need
                                // automatic spacing between characters.

  // Set the intensity (brightness) of the display (0-15)
  myDisplay.setIntensity(8);

  // Clear the display
  myDisplay.displayClear();

}

void loop() {
  
  while(ledMatSerial.available()) {
    char inData=ledMatSerial.read();
    if(inData != 10 && inData != 13) {  // as long as it's not a cr or lf
      append(buf, 8, inData);           // add the data to the buffer,
                                        // using the fancy 'append' define
                                        // at the top of the sketch.
      inCount++;                        // increment the counter
    }
    // Now we check to see if we have 8 or more characters, and
    // if the computer checksum matches what's stored in the pos 7 of the 
    // buffer and if the state of the first character is valid.  I am using
    // the defines 'checksum' and 'state_is_valid' from the top of the
    // sketch to simplify the following if statement.
    if(inCount > 7 && checksum(buf) == buf[7] && state_is_valid(buf[0])) {
      inCount=0;            // we're going to print something, so reset the
                            // count of the buffer and print it.
 
      disp[0]=' ';          // To stabilize the display, we only update it IF
      disp[1]=buf[1];       // we have a good checksum and state.
      disp[2]=':';          // We are creating a new 'disp' array from our
      disp[3]=buf[2];       // 'buf' array and adding the extra characters 
      disp[4]=buf[3];       // we want to display.
      disp[5]=buf[4]+128;
      disp[6]=buf[5]+128;
      disp[7]=buf[6]+128;
      myDisplay.print(disp);  // print it to the LED display.
    }
  }
  changeBrightness(); // Call the changeBrightness() function.
}

void changeBrightness() {
  int bright=map(analogRead(BRIGHT_PIN), 0, 1023, 0, 15); // Read in the brightness 
                                        // from analog BRIGHT_PIN.  Map
                                        // it to a value from 0-15.
  if (dispBright != bright) {           // has it changed?
    dispBright = bright;                // if so, store the new value
    myDisplay.setIntensity(bright);     // and change the brightness.
  }
}
