#include <Wire.h>
#include "Adafruit_MPR121.h"


#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap1 = Adafruit_MPR121();
Adafruit_MPR121 cap2 = Adafruit_MPR121();
Adafruit_MPR121 cap3 = Adafruit_MPR121();

void setup() {
  Serial.begin(115200);

  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  
  if (!cap1.begin(0x5A)) {
    //Serial.println("MPR121 not found on 0x5A, check wiring?");
    while (1);
  }
  if (!cap2.begin(0x5B)) {
    //Serial.println("MPR121 not found on 0x5B, check wiring?");
    while (1);
  }
  if (!cap3.begin(0x5C)) {
    //Serial.println("MPR121 not found on 0x5B, check wiring?");
    while (1);
  }
}

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

int velocity = 100;
int noteON = 144;
int noteOFF = 128;

void MIDImessage(int command, int MIDInote, int MIDIvelocity) {
  Serial.write(command);//send note on or note off command 
  Serial.write(MIDInote);//send pitch data
  Serial.write(MIDIvelocity);//send velocity data
}

void loop() {
  currtouched = cap1.touched();
  
  for (int i=0; i<12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      MIDImessage(noteON, i, velocity);
      delay(10);
    }
  }

  // reset our state
  lasttouched = currtouched;

  // Get the currently touched pads
  currtouched = cap2.touched();

  for (int i=0; i<12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      MIDImessage(noteON, i+12, velocity);
      delay(10);
    }
  }
  // reset our state
  lasttouched = currtouched;

  currtouched = cap3.touched();

  for (int i=0; i<12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      MIDImessage(noteON, i+24, velocity);
      delay(10);
    }
  }
  // reset our state
  lasttouched = currtouched;

  delay(10);
}
