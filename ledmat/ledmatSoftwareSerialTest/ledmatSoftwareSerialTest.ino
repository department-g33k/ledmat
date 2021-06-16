/* 
  Software Serial test of LEDMAT protocol - number 2!
  
  Second attempt - now we are going to make sure the output data always starts with a ascii
  control character, followed by the remaining 7 digits.  

  It will read in the data from the ledmat on arduino pin 2, and print it out to the usb
  serial to the serial monitor in the arduino ide.
*/
#include <SoftwareSerial.h>
const int ledmatRXPin = 2;
const int ledmatTXPin = 3; // I don't think we need this pin, but software serial expects 
                           // to talk to an RX and TX pin, so we include it below.  Don't
                           // connect anything to pin 3 on the arduino or change this to another
                           // arduino pin.
const int ledmatInvertSerial = 1; // I think the ledmat uses inverted serial - i.e. a low
                                  // to represent a one and a high to represent a 0
                                  // set this value to 0 if that's not true.
SoftwareSerial ledMatSerial(ledmatRXPin, ledmatTXPin, ledmatInvertSerial); // Setup software serial

char timeResult[7]; // variable for the time results
char state; // variable for the state (alpha character or space)

void setup() {
  Serial.begin(115200); // Set up serial communication to computer at 115200 bps.
  ledMatSerial.begin(1200); // enable the ledmat serial at 1200bps
}

void loop() {

  while( ledMatSerial.available() > 0 ) { // Is data available from ledmat? if so, do this loop.
    char firstByte=ledMatSerial.read(); //save it in firstByte.
    if (isAlpha(firstByte) || isWhitespace(firstByte)) { // Check to make sure the first character is an
                                                         // alpha or a space.
      state = firstByte; // we don't want to loose the state byte - so we save it.
      for ( int i=0; i<7; i++ ) { // We already got our alpha character at the beginning, so now we read
                             // in the remaining characters. (7 remaining)
        timeResult[i]=ledMatSerial.read(); // put it into timeResult variable.
      }
    }
    else {
      break; // If the first character isn't alpha or space, break out of the while loop and start over.
             // This ensures each line will start with only an alpha character or a space.
    }
  }

  // Print what we have, whether it changed or not.  We will have a newline of output every time the
  // main loop 'loops around'.  If we need to make a decision on the state, this is where we do it.
  // Expected output should look like the following on the serial monitor:
  // I- 0125610
  Serial.print(state);
  Serial.print("- ");
  Serial.println(timeResult);
}
