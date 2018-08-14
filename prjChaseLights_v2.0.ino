#include <elapsedMillis.h>
#include <FAB_LED.h>
//#include <OneButton.h>
//#include <EEPROM.h>
#include "palette.h"

#define BUS B             // Bus the neopixels are on
#define PIN 3             // Pin the neopixels are on
#define LEDS 54           // 54 neopixels
#define SEQS 7            // 6 sequences
#define SEQDUR 10000      // 10 second sequence duration
#define PALS 5            // 7 palettes

ws2812b<BUS, PIN> strip;  // Initialize the neopixel object
grb pixels[LEDS] = {};    // Neopixel color array

elapsedMillis seqTmr;         // millis() counter that you can reset
uint8_t curSeq;               // initialize current sequence variable
uint8_t curDir;               // initialize durrent direction variable (only used on some sequences)

AlaPalette *newPal;         // Set up a palette pointer

void setup() {
  randomSeed(analogRead(A2)); // Provide a random seed from a floating analog pin
  strip.clear(2 * LEDS);      // Clear the neopixel data
  curSeq = random(SEQS);      // Select a random sequence
  //curSeq = 6;
  curDir = random(2);         // Select a random direction

  /*switch (random(PALS)) {     // Select a random palette
    case 0:
      newPal = &alaPalRainbow;
      break;

      case 1:
      newPal = &alaPalRainbowStripe;
      break;

    case 2:
      newPal = &alaPalRgb;
      break;

    case 3:
      newPal = &alaPalParty;
      break;

    case 4:
      newPal = &alaPalCool;
      break;

    case 5:
      newPal = &alaPalFire;
      break;

    case 6:
      newPal = &alaPalHeat;
      break;

    default:
      newPal = &alaPalRgb;
      break;
    }*/
    newPal = &alaPalRainbow;
}

void loop() {
  if (seqTmr > SEQDUR) {      // If we have been on the current sequence longer than the max duration
    //strip.clear(2 * LEDS);    // Clear the neopixel data (somtimes gets junk pixels)
    seqTmr = 0;               // Reset the sequence timer to 0
    curDir = random(2);       // Select a random direction
    
    uint8_t newSeq;           // Pick a random sequence until we get a unique one
    do {
      newSeq = random(SEQS);
    }while (curSeq == newSeq);
    curSeq = newSeq;

    switch (random(PALS)) {     // Select a random palette
      case 0:
        newPal = &alaPalRainbow;
        break;

        case 1:
        newPal = &alaPalRainbowStripe;
        break;

      /*case 2:
        newPal = &alaPalRgb;
        break;*/

      case 2:
        newPal = &alaPalParty;
        break;

      case 3:
        newPal = &alaPalCool;
        break;

      default:
        newPal = &alaPalFire;
        break;

      /*case 6:
        newPal = &alaPalHeat;
        break;*/

      /*default:
        newPal = &alaPalRgb;
        break;*/
    }
  }

  switch (curSeq) {
    case 0:
      sparkle(255, *newPal);
      break;

    case 1:
      comet(2000, *newPal, curDir);
      break;

    case 2:
      cometCol(2000, *newPal, curDir);
      break;

    case 3:
      larson(2000, *newPal);
      break;

    case 4:
      gradient(3333, *newPal, curDir);
      break;

    case 5:
      bars(3333, *newPal, curDir);
      break;

    default:
      plasma(2000, *newPal);
      break;

    /*default:
      curSeq = random(SEQS);*/
  }
  
  strip.sendPixels(LEDS, pixels);                 // Update the pixel display
}

/*void setPixel(uint8_t pix, char r, char g, char b) {
  pixels[pix].r = r;
  pixels[pix].g = g;
  pixels[pix].b = b;
}*/

/*void setStrip(char r, char g, char b) {
  for (uint8_t i = 0; i < LEDS; i++) {
    pixels[i].r = r;
    pixels[i].g = g;
    pixels[i].b = b;
  }
}*/

/*void setStrip(grb c) {
  for (uint8_t i = 0; i < LEDS; i++) {
    pixels[i].r = c.r;
    pixels[i].g = c.g;
    pixels[i].b = c.b;
  }
}*/

void sparkle(uint8_t fadeSpd, AlaPalette &pal) {
  //if (animTmr < 255 / spd) return;
    
  for (uint8_t x = 0; x < LEDS; x++) 
  {
    if (random(16384 / fadeSpd) == 0)                               // 1 in 500 chance to trigger a bright pixel
      pixels[x] = pal.colors[random(pal.numColors)];  // Set to a random color
    else
      pixels[x] = cScale(pixels[x], 88);          // Scale the pixel down to 88%
  }
}

void comet(uint16_t spd, AlaPalette &pal, uint8_t dir) {
  uint8_t l = LEDS / 3;
  float t = getStepFloat(spd, LEDS - 1);
  float tx = getStepFloat(spd, pal.numColors);
  grb c = pal.getPalColor(tx);

  for (uint8_t x = 0; x < LEDS / 2; x++) {
    uint8_t k = constrain((((x - t) / l + 1.2f)) * (((x - t)<0)? 1:0), 0, 1) * 100;
    if (dir) {
      pixels[x] = cScale(c, k);
      pixels[LEDS - 1 - x] = cScale(c, k);
    }else {
      pixels[x + LEDS / 2] = cScale(c, k);
      pixels[LEDS / 2 - 1 - x] = cScale(c, k);
    }
  }
  //setStrip(c);
}

void cometCol(uint16_t spd, AlaPalette &pal, uint8_t dir) {
  uint8_t l = LEDS / 3;
  float t = getStepFloat(spd, LEDS - 1);

  grb c;
  for (uint8_t x = 0; x < LEDS / 2; x++) {
    float tx = mapFloat(max(t - x, 0), 0, (float)LEDS / 1.7f, 0, pal.numColors - 1);
    c = pal.getPalColor(tx);
    uint8_t k = constrain((((x - t) / l + 1.2f)) * (((x - t)<0)? 1:0), 0, 1) * 100;
    if (dir) {
      pixels[x] = cScale(c, k);
      pixels[LEDS - 1 - x] = cScale(c, k);
    }else {
      pixels[x + LEDS / 2] = cScale(c, k);
      pixels[LEDS / 2 - 1 - x] = cScale(c, k);
    }
  }
}

void larson(uint16_t spd, AlaPalette &pal) {
  uint8_t l = LEDS / 4;
  float t = getStepFloat(spd, 2 * LEDS - 2);
  grb c = pal.getPalColor(getStepFloat(spd, pal.numColors));

  for(uint8_t x = 0; x < LEDS; x++) {
    uint8_t k = constrain((-abs(abs(t - LEDS + 1) - x) + 1), 0, 1) * 100;
    pixels[x] = cScale(c, k);
  }
}

void gradient(uint16_t spd, AlaPalette &pal, uint8_t dir) {
  float t = getStepFloat(spd, LEDS);
  for (uint8_t x = 0; x < LEDS; x++) {
    if (dir)
      pixels[LEDS - 1 - x] = pal.getPalColor((float)((x + t) * pal.numColors) / LEDS);
    else
      pixels[x] = pal.getPalColor((float)((x + t) * pal.numColors) / LEDS);
  }
}

void bars(uint16_t spd, AlaPalette &pal, uint8_t dir) {
  uint8_t t = getStep(spd, LEDS);
  for (uint8_t x = 0; x < LEDS; x++) {
    if (dir)
      pixels[LEDS - 1 - x] = pal.colors[(((t + x) * pal.numColors) / LEDS) % pal.numColors];
    else
      pixels[x] = pal.colors[(((t + x) * pal.numColors) / LEDS) % pal.numColors];
  }
}

void plasma(uint16_t spd, AlaPalette &pal) {
  float t = getStepFloat(spd, LEDS);

  for (uint8_t x = 0; x < LEDS; x++) {
    grb c1 = pal.getPalColor((float)((x + t) * pal.numColors) / LEDS);
    grb c2 = pal.getPalColor((float)((2 * x - t + LEDS) * pal.numColors) / LEDS);
    pixels[x] = cInterpolate(c1, c2, 50);
  }
}

float getStepFloat(uint16_t t, uint8_t v) {
  return (float)((seqTmr % t) * (float)v / t);
}

uint8_t getStep(uint16_t t, uint8_t v)
{
  return (seqTmr % t) * v / t;
}

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
