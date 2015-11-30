#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 64

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 3
#define CLOCK_PIN 13

const uint8_t PROGMEM gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


// From http://ecg.utah.edu/img/items/Normal%2012_Lead%20ECG.jpg
byte beat[]  = {3,2,2,2,2,3,4,3,2,1,0,15,2,2,3,4,6,8,5,3,3,3};
byte beatLength = 22;
byte maxStrength = 15;

int bpm = 35;

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
  	  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

byte bufr[NUM_LEDS];

byte offset = 0;

void moveBuffer() {
  int i;

  byte c = (int) beat[offset] * 255 / beatLength;
  c = pgm_read_byte(&gamma[c]);

  bufr[NUM_LEDS - 1] = c;

  for (i=0;i<NUM_LEDS - 1;i++){
    bufr[i] = bufr[i+1];
  }
}

void blit() {
  int i;
  
  for (i=0;i<NUM_LEDS;i++){
    leds[i] = CRGB(bufr[i], 0, bufr[i] / 16);
  }

  FastLED.show();
}

#define movesPerBeat 4

void loop() { 

  offset = (offset + 1) % beatLength;

  int i;

  for (i=0;i<movesPerBeat;i++){
    delay((float) 1 / bpm * 60 * 1000 / beatLength / movesPerBeat);
    moveBuffer();
    blit();
  }
}
