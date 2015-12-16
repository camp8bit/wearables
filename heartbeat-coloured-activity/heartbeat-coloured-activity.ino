#include "FastLED.h"

/*
 * Heartbeat simulator
 * 
 * Shows a different hue and brightness based on accelerometer activity,
 * with the idea that low movement means colder and less bright colours,
 * with more movement turning it into warmer and brighter colours.
 * 
 * TODO Integrate pluse meter
 * 
 * Credit to Ben Nolan for the heart beat LED logic.
 * And for lending me some Neopixels to play around with!
 */
 
#define BAUD_RATE 9600
#define NUM_LEDS 30
#define DATA_PIN 6
#define ACCELX_PIN 0
#define ACCELY_PIN 1
#define ACCELZ_PIN 2

// config
int bpm = 35; // From http://ecg.utah.edu/img/items/Normal%2012_Lead%20ECG.jpg
byte beat[]  = {3,2,2,2,2,3,4,3,2,1,0,15,2,2,3,4,6,8,5,3,3,3};
byte beatLength = 22;
int movesPerBeat = 8;
int minMagnitude = 80;
int maxMagnitude = 150;
int minBrightness = 10; // from 0-255
int maxBrightness = 100; // from 0-255
int minHue = 160; // hue value under low motion
int maxHue = 255; // hue value under high motion
int gainRatePerBeat = 200; // value change towards new target value (per beat)
int decayRatePerBeat = 1; // percent change towards new target per beat
int sampleSize = 8; // smooth out accelerometer readings

// state
byte bufr[NUM_LEDS];
byte offset = 0;
int targetMagnitude = minMagnitude;
int adjustedMagnitude = minMagnitude;

CRGB leds[NUM_LEDS];

void setup() { 
      FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
      Serial.begin(BAUD_RATE);
}

void moveBuffer() {
  int i;
  byte c = (int) beat[offset] * 255 / beatLength;

  // Move current beat intensity backwards through buffer
  bufr[NUM_LEDS - 1] = c;
  for (i=0;i<NUM_LEDS - 1;i++){
    bufr[i] = bufr[i+1];
  }
}

void draw() {
  int i;

  // Go from a cool color on low to a warm color on high activity
  int hue = map(
    adjustedMagnitude,
    minMagnitude,
    maxMagnitude,
    minHue,
    maxHue
  );

  // Lower magnitude means more brightness reduction
  int brightnessFactor = map(
    adjustedMagnitude,
    minMagnitude,
    maxMagnitude,
    3, 
    1
  );
  
  for (i=0;i<NUM_LEDS;i++){
      leds[i] = CHSV(hue, bufr[i], bufr[i]/brightnessFactor);
  }
  
  FastLED.show();
}

int calcAdjustedMagnitude() {
  // Get new target (smoothed out over a couple of readings)
  targetMagnitude = max(
    constrain(getMagnitude(), minMagnitude, maxMagnitude), 
    targetMagnitude
  );
  
  // Slowly work towards new target
  if(adjustedMagnitude <= targetMagnitude) {
    adjustedMagnitude = constrain(
      (targetMagnitude*100 + gainRatePerBeat) / 100, 
      minMagnitude, 
      maxMagnitude
    );
  } else {
    adjustedMagnitude = constrain(
      (targetMagnitude*100 - gainRatePerBeat) / 100, 
      minMagnitude, 
      maxMagnitude
    );
  }

  // Slowly decay max target
  targetMagnitude = constrain(
    (targetMagnitude*100 - decayRatePerBeat) / 100, 
    minMagnitude, 
    maxMagnitude
  );
}

// Get a magnitude across all vectors.
// Smooth out result through a rolling average
int getMagnitude() {
  float avgMag = 0;
  for(int x = 0 ; x < sampleSize ; x++) {
      int aX = analogRead(ACCELX_PIN);
      int aY = analogRead(ACCELY_PIN);
      int aZ = analogRead(ACCELZ_PIN);
  
      float magnitude = sqrt((aX * aX) + (aY * aY) + (aZ * aZ));
      avgMag += magnitude;
  }
  avgMag /= sampleSize;
  
  return avgMag;
}

void loop() { 
  offset = (offset + 1) % beatLength;

  int i;
  for (i=0;i<movesPerBeat;i++){
    delay((float) 1 / bpm * 60 * 1000 / beatLength / movesPerBeat);
    moveBuffer();
    draw();
  }

  calcAdjustedMagnitude();
}