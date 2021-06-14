/* 
  Software Serial test of LEDMAT protocol.

  I think we can simplyify the code by trying to get the built-in software serial library
  to do the actual reading from the ledmat.

  This is a first attempt.

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

void setup() {
  Serial.begin(115200); // Set up serial communication to computer at 115200 bps.
  ledMatSerial.begin(1200); // enable the ledmat serial at 1200bps
}

void loop() {
  Serial.println("Received data from ledmat:");  // Print to USB Serial
  while (ledMatSerial.available() > 0 ) { // Is data available from ledmat?
    char inbyte=ledMatSerial.read(); //save it in inbyte
    Serial.print(inbyte, DEC); // print it as a decimal number
    Serial.print(": "); // print a colon seperator
    Serial.print(inbyte); // print it as a character
    Serial.print(", "); // add marker for end of character
  }
}
