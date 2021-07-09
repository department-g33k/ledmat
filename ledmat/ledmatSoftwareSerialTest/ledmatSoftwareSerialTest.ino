/* 
  Software Serial test of LEDMAT protocol - number 5!
  
  Fifth and final test before integrating the LED display!
-------------------
  I discovered that the Stackmat has changed the number of bytes they transmit.  Older StackMat's transmit:
  minutes, 10seconds, seconds, 10milliseconds, milliseconds, checksum

  Newer ones transmit an extra byte for the milliseconds:
  minutes, 10seconds, seconds, 100milliseconds, 10milliseconds, milliseconds, checksum

  because of this, what I thought was the checksum byte was almost always wrong, hence all of the checksum errors.
  I purchased a Stackmat to discover this.  I added the extra byte to the buffer and changed the define that checks
  the checksum to add in the additional byte.  Now I am getting a reliable display on the serial monitor.

  Next is integration of the LED display!!
-------------------
  I discovered putting whole tests and functions into a define - this allows he compilier to substitute
  complex checks and features into other parts of the program.  awesome! Discovered from https://github.com/fw42
  in this repo: https://github.com/fw42/atmel/tree/master/stackmat

  I pulled alot of concepts from what he is doing and simplified it for arduino.
*/

//Libraries for display:
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include<SPI.h>


// The magical defines:
#define state_is_valid(s) (s=='I' || s=='A' || s==' ' || s=='S' || s=='L' || s=='R' || s=='C')
#define checksum(data) ( (data[1]-'0')+(data[2]-'0')+(data[3]-'0')+(data[4]-'0')+(data[5]-'0')+(data[6]-'0')+64 )
#define append(array, length, item) { for(uint8_t i=0; i<length-1; i++) { array[i] = array[i+1]; };  array[length-1] = item; }

#include <SoftwareSerial.h>

//Constants for display bits:
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

const int updateoffset = 10;
int updatecycle = 0;

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
char buf[9]={0,0,0,0,0,0,0,0,0};
byte inCount=0; // Counter for how many bytes we have received.

void setup() {
  Serial.begin(115200);     // Set up serial communication to computer at 115200 bps.
  ledMatSerial.begin(1200); // enable the ledmat serial at 1200bps
  Serial.println("Here we go again!");

    // Intialize the object
  myDisplay.begin();

  // Set the intensity (brightness) of the display (0-15)
  myDisplay.setIntensity(0);

  // Clear the display
  myDisplay.displayClear();

myDisplay.setTextAlignment(PA_LEFT);
myDisplay.print("Luke");
delay(500);
myDisplay.setTextAlignment(PA_CENTER);
myDisplay.print("Solves");
delay(500);
myDisplay.setTextAlignment(PA_RIGHT);
myDisplay.print("Cubes");
delay(500);

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
      Serial.println(buf);  // just print the entire buffer!  Final character
                            // is the checksum!
    }
  updatecycle++;
  if (updatecycle > updateoffset) {
    myDisplay.print(buf);
    updatecycle = 0;
    }
  
  }  
  
}
