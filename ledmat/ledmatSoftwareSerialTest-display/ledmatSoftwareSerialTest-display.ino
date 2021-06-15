#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 5

#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

int digits[8]; // Create an array to store the digits of a "packet"
String(displaytext); //Create a string to store the display text

// Hardware SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

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
  P.begin(); // Initialize the display
  Serial.begin(115200); // Set up serial communication to computer at 115200 bps.
  ledMatSerial.begin(1200); // enable the ledmat serial at 1200bps
  P.print("LEDmat");
}

void loop() {
while( ledMatSerial.available() > 0) { // Is data available from ledmat?
   char inbyte=ledMatSerial.read(); //save it in inbyte
    if (isAlpha(inbyte)) { //If the byte is an Alphabetic character, we can infer it's a data string from the timer, and thus the next 8 bytes are the data string
      Serial.println("Found a numeric String: ");
      displaytext = "";
      memset(digits, 0, sizeof(digits));
      /*
      for (int lc = 0; lc > 8; lc++) { //Loop through to read next 8 bytes
        char digitread = ledMatSerial.read(); //incrementally add the incoming digit to a string
        displaytext+= digitread;
      }*/
      digits[0]=ledMatSerial.read();
      digits[1]=ledMatSerial.read();
      digits[2]=ledMatSerial.read();
      digits[3]=ledMatSerial.read();
      digits[4]=ledMatSerial.read();
      digits[5]=ledMatSerial.read();
      digits[6]=ledMatSerial.read();
      digits[7]=ledMatSerial.read();
      digits[8]=ledMatSerial.read();
            Serial.print(digits[0]);
            Serial.print(digits[1]);
            Serial.print(".");
            Serial.print(digits[2]);
            Serial.print(digits[3]);
            Serial.print(digits[4]);
            Serial.print(digits[5]);
            Serial.print(digits[6]);
            Serial.print(digits[7]);
            Serial.print(digits[8]);
            
            P.print(digits[0]);
    }
else {
  Serial.println(inbyte);
}
}
}
