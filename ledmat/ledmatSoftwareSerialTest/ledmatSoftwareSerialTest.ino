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

char t[100]={}; // variable for the time results - giving it more space just in case.
int output=0;
void setup() {
  Serial.begin(115200); // Set up serial communication to computer at 115200 bps.
  ledMatSerial.begin(1200); // enable the ledmat serial at 1200bps
}

void loop() {
  memset(t, 0x00, sizeof(t)); // chear the array we are using as a serial buffer.
  output=0; // I'm setting this variable to 0.  Only print output if this is a 1.
  for (int i=0; ledMatSerial.available(); i++) { // Create a loop that will count only when there
                                                 // are characters available from the serial buffer.
                                                 // Hopefully there is enough of a break between character 
                                                 // groups that this won't just fill up
    output=1; // We received some data, so set this to a one
    if (i > 99) { break; } // kill this loop if i>99.  Otherwise we'll never get any other output.
                           // If we keep hitting this, then the ledmat doesn't have any breaks between
                           // character groups.  Have to find another way to synchronize to the data in that case.
    t[i]=ledMatSerial.read();
    Serial.print("."); // We'll print a period for every character received.  Shouldn't be more than 7.
  }
  if (output) { // If we gathered anything from the above loop, output will be 1, so do the following:
    Serial.println(); // Now we print a newline -to seperate it from the ".......".
    Serial.println(t); // these are the characters we gathered in the for loop.
    output=0; // clear the output flag, so we don't print the data more than once.
  }
}
