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

char t[10]={}; // variable for the time results - giving it more space just in case.
char state=0x00; // variable for the state (alpha character or space)
char out[10]={}; // variable for a formatted string to output the time.  It also has more space.

void setup() {
  Serial.begin(115200); // Set up serial communication to computer at 115200 bps.
  ledMatSerial.begin(1200); // enable the ledmat serial at 1200bps
}

void loop() {

  while( ledMatSerial.available() > 0 ) { // Is data available from ledmat? if so, do this loop.
    char firstByte=ledMatSerial.read(); //save it in firstByte.
    int chksum=0; // start the loop with checksum cleared.
    if (isAlpha(firstByte) || isWhitespace(firstByte)) { // Check to make sure the first character is an
                                                         // alpha or a space.
      memset(t, 0x00, sizeof(t)); // Clear the timeResult array by setting each entry to 0
      state = firstByte; // we don't want to loose the state byte - so we save it.
      int readInBytes = 5; // Number of bytes to read in after we receive an alpha character or a space.
                           // This might need to be adjusted.  
      for ( int i=0; i<readInBytes; i++ ) { // We already got our alpha character at the beginning, so now we 
                                            // read in the remaining characters.
        t[i]=ledMatSerial.read(); // put it into the 't' variable.
        chksum+=(t[i]-48); // subtract 48 from the 't' variable to conver the ascii number to an actual number
                           // and add it to the chksum variable.
      }
      int readChksum=ledMatSerial.read(); // read in final checksum byte
      if (readChksum != chksum) { // If checksum doesn't match
        Serial.print("Error! Invalid Checksum!!  "); // print the error
        Serial.print(readChksum, DEC);  
        Serial.print(" != ");
        Serial.println(chksum, DEC);
        break; // and start the loop over again.
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
  memset(out, 0x00, sizeof(out));    
  // Arduino doesn't have very good string handling, so I'm formatting the output one character at a time.  
  // Very sucky.  Hopefully I have broken out the correct values to the proper times below:
  out[0] = t[0]; // Minutes
  out[1] = ':';
  out[2] = t[1]; // 10's digit Seconds
  out[3] = t[2]; // 1's digit Seconds
  out[4] = ':';
  out[5] = t[3]; // 10's digit milliseconds
  out[6] = t[4]; // 1's digit milliseconds
  Serial.print(out); // hopefully this prints the elapsed time and not random crap.
  Serial.print(" | ");
  delay(500); // added a delay to reduce the amount of output.  This should be the first thing to remove 
              // if the output isn't what you expect.
}
