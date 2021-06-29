/* 
  Software Serial test of LEDMAT protocol - number 4!
  
  Fourth attempt:

  I discovered putting whole tests and functions into a define - this allows he compilier to substitute
  complex checks and features into other parts of the program.  awesome! Discovered from https://github.com/fw42
  in this repo: https://github.com/fw42/atmel/tree/master/stackmat

  I pulled alot of concepts from what he is doing and simplified it for arduino.
*/

// The magical defines:
#define state_is_valid(s) (s=='I' || s=='A' || s==' ' || s=='S' || s=='L' || s=='R' || s=='C')
#define checksum(data) ( (data[1]-'0')+(data[2]-'0')+(data[3]-'0')+(data[4]-'0')+(data[5]-'0')+64 )
#define append(array, length, item) { for(uint8_t i=0; i<length-1; i++) { array[i] = array[i+1]; };  array[length-1] = item; }

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

// Buffer for the last seven received bytes
// state, min, sec1, sec2, msec1, msec2, checksum
char buf[7]={0,0,0,0,0,0,0};
byte inCount=0; // Counter for how many bytes we have received.

void setup() {
  Serial.begin(115200);     // Set up serial communication to computer at 115200 bps.
  ledMatSerial.begin(1200); // enable the ledmat serial at 1200bps
  Serial.println("Here we go again!");
}

void loop() {
  while(ledMatSerial.available()) {
    char inData=ledMatSerial.read();
    if(inData != 10 && inData != 13) {  // as long as it's not a cr or lf
      append(buf, 7, inData);           // add the data to the buffer,
                                        // using the fancy 'append' define
                                        // at the top of the sketch.
      inCount++;                        // increment the counter
    }
    // Now we check to see if we have 7 or more characters, and
    // if the computer checksum matches what's stored in the pos 6 of the 
    // buffer and if the state of the first character is valid.  I am using
    // the defines 'checksum' and 'state_is_valid' from the top of the
    // sketch to simplify the following if statement.
    // if(inCount >= 7 && checksum(buf) == buf[6] && state_is_valid(buf[0])) {
    if(inCount >= 7 && checksum(buf) == buf[6]) {
      inCount=0;            // we're going to print something, so reset the
                            // count of the buffer and print it.
      Serial.print(buf[0]);
      Serial.print("-");
      Serial.print(":");
      Serial.print(buf[1]);
      Serial.print(buf[2]);
      Serial.print(".");
      Serial.print(buf[3]);
      Serial.print(".");
      Serial.print(buf[4]);
      Serial.print(buf[5]);
    } else if(inCount >= 7 && checksum(buf) != buf[6]) {
      inCount=0;
      Serial.print("CKsum Err: ");
      Serial.print(checksum(buf));
      Serial.print("!=");
      Serial.println(buf[6]);

    }
  }  
  
}
