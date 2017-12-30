/** @file */

//##########
// Includes
//##########

#include <Adafruit_NeoPixel.h>
#include "ledCryptoTree.h"


//#######################
// mController Functions
//#######################

/**
  *  mController setup function (initilizer)
  *
  */

void setup() {
  setupSerial();

  setupNeopix();
}


/**
  *  mController loop function (control loop)
  *
  */

void loop() {
  // Read the serial in and match a number passed in
  parseSerial();

  // Baised on the state of goal, do patterens
  parseGoals();
}


//#################
// Setup Functions
//#################

/**
  * Setup for serial connection
  *
  */

void setupSerial() {
  Serial.begin(SERIAL_DATA_RATE);

  // Wait for Serial to become active
  while(!Serial);

  Serial.println(SERIAL_INTRO_TEXT);
}


/**
  * Setup for Neopixel objects
  *
  */

void setupNeopix() {
  vertical.begin();
  vertical.show(); // Initialize all pixels to 'off'

  spiral.begin();
  spiral.show();   // Initialize all pixels to 'off'
}


//#################
// Parse Functions
//#################

/**
  * Parse the current goal in to patterns
  *
  * @see enum Goals
  *
  */

void parseGoals() {
  switch(goal) {
    case BIG:
    { // Scope it cause I can (But really initilizers)
      simpleFadeIn(FAIRY_PIN, 3);
      simpleFadeOut(FAIRY_PIN, 3);

      runningLights((LwColor){0xff, 0xff, 0x00}, 20);
    }
    break;

    case MEDIUM:
      simpleFadeIn(FAIRY_PIN, 20);
      twinkleRandom(20, 100, false);

      simpleFadeOut(FAIRY_PIN, 20);
      twinkleRandom(20, 100, false);
    break;

    case SMALL:
    {
      simpleFadeIn(FAIRY_PIN, 50);

      // Making local variable as it will be in a loop (Yeah, it is probably optimized that way)
      LwColor c = {0x10, 0x10, 0x10};

      for(uint8_t i = 0; i < 20; i++) {
        snowSparkle(c, 20, random(10, 100));

        if(10 == i)
          simpleFadeOut(FAIRY_PIN, 50);
      }
    }
    break;

    default:
      simpleFadeIn(FAIRY_PIN, 30);
      simpleFadeOut(FAIRY_PIN, 30);
    break;
  }
}


/**
  * Parse the incoming serial data to set Goal progression
  * and/or patterns
  *
  * @see Goal
  *
  */

void parseSerial() {
  if (Serial.available()) {
    int state = Serial.parseInt();

    // If nothing, or anything but a number is passed
    // after a time out, or return is pressed 0 is returned.
    // Skip processing an input that will not trigger an actual
    // result.
    if (state) {
      switch (state) {
        case 1:
          goal = SMALL;
        break;

        case 2:
          goal = MEDIUM;
        break;

        case 3:
          goal = BIG;
        break;

        case 4:
          cylonBounce((LwColor){0xff, 0xff, 0x00}, 4, 10, 10);
        break;

        case 5:
          cylonBounce(LW_RED, 4, 10, 50);
        break;

        case 6:
          colorCount(LW_GREEN, 50, 50);
        break;

        case 7:
          colorCount(LW_GREEN, 50, 100);
        break;

        case 8:
          colorCount(LW_GREEN, 50, 30);
        break;

        case 9:
          colorCount(LW_GREEN, 50, 10);
        break;
      };
    }
  }
}


//#################
// Simple Patterns
// (Fairy Lights)
//#################

/**
  * Simple fade-in pattern for non-addressable lights.
  *
  * A simple pattern
  * Controls a single analog pin.
  *
  * @param pin      The pin to control
  * @param speedVal The delay used to control the speed
  *                 (inverse of speed, lower number is faster)
  */

void simpleFadeIn(int pin, int speedVal) {
  for (uint8_t fadeValue = COLOR_VAL_MIN; fadeValue <= COLOR_VAL_MAX; fadeValue += 5) {
    analogWrite(pin, fadeValue);

    delay(speedVal);
  }
}


/**
  * Simple fade-out pattern for non-addressable lights.
  *
  * A simple pattern
  * Controls a single analog pin.
  *
  * @param pin      The pin to control
  * @param speedVal The delay used to control the speed
  *                 (inverse of speed, lower number is faster)
  */


void simpleFadeOut(int pin, int speedVal) {
  for (uint8_t fadeValue = COLOR_VAL_MAX; fadeValue >= COLOR_VAL_MIN; fadeValue -= 5) {
    analogWrite(pin, fadeValue);

    delay(speedVal);
  }
}


//#################
// Spiral Patterns
//#################

/**
  * Counts the color...? Pattern.
  *
  * A spiral pattern
  * TODO: Get Lindy's interpretation of this pattern.
  *
  * @param color      The color to use (start with?) in LwColor type
  * @param speedDelay The delay used to control the speed
  *                   (inverse of speed, lower number is faster)
  * @param percent    Percent of...? to...?
  *
  */

void colorCount(LwColor color, int speedDelay, int percent) {
  // Modify percent
  percent = (VERTICAL_NUM * percent) / 100; //TODO: Make a macro for dis
  //TODO: Also, I think this is supposed to be SPIRAL_NUM?

  for(Pixel i = 0; i < SPIRAL_NUM; i++) {
    setPixelNil(spiral, i);

    show(spiral);

    delay(speedDelay);
  }

  Serial.println(percent);

  for (Pixel i = 0; i < (Pixel)percent; i++) {
    setPixel(spiral, i, color);

    show(spiral);

    delay(speedDelay);
  }
}


/**
  * Pattern that bounds back and forth like a clasic cylon's eyes.
  *
  * A spiral pattern
  * TODO: Get Lindy's full interpretation of this pattern.
  *
  * @param color       The color to use (start with?) in LwColor type
  * @param eyeSize     The size of the... Eyes?
  * @param speedDelay  The delay used to control the speed
  *                    (inverse of speed, lower number is faster)
  * @param returnDelay Rebounding delay?
  *
  */


//TODO: It is labled as sprial, includes spiral, but also has vertical?
void cylonBounce(LwColor color, int eyeSize, int speedDelay, int returnDelay) {
  //
  // Spiral pattern
  //

  LwColor colorTenthed = (LwColor){
    DIV_CONV(color.red, 10),
    DIV_CONV(color.green, 10),
    DIV_CONV(color.blue, 10)
  };

  Pixel loopTest = VERTICAL_NUM - eyeSize - 2;

  for(Pixel i = 0; i < loopTest; i++) {
    setAllNil(spiral);

    setPixel(
      spiral,
      i,
      colorTenthed
    );

    // Inner-loop
    for(Pixel j = 1; j <= (Pixel)eyeSize; j++) {
      setPixel(
        spiral,
        i + j,
        color
      );
    }

    Pixel n = i + eyeSize + 1;

    setPixel(
      spiral,
      n,
      colorTenthed
    );

    show(spiral);

    delay(speedDelay);
  }

  delay(returnDelay);

  Pixel loopInit = SPIRAL_NUM - eyeSize - 2;

  for(Pixel i = loopInit; i > 0; i--) {
    setAllNil(spiral);

    setPixel(
      spiral,
      i,
      colorTenthed
    );

    // Inner-loop
    for (Pixel j = 1; j <= (Pixel)eyeSize; j++) {
      setPixel(
        spiral,
        i + j,
        color
      );
    }

    Pixel n = i + eyeSize + 1;

    setPixel(
      spiral,
      n,
      colorTenthed
    );

    show(spiral);

    delay(speedDelay);
  }

  delay(returnDelay);
}


//###################
// Vertical Patterns
//###################

/**
  * Pattern that wipes with color.
  *
  * A vertical Pattern
  * TODO: Get Lindy's interpretation of this pattern.
  *
  * @param color      The color to use (start with?) in LwColor type
  * @param speedDelay The delay used to control the speed
  *                   (inverse of speed, lower number is faster)
  *
  */

void colorWipe(LwColor color, int speedDelay) {
  for(Pixel i = 0; i < VERTICAL_NUM; i++) {
    setPixel(
      vertical,
      i,
      color
    );

    show(vertical);

    delay(speedDelay);
  }
}


/**
  * Pattern that runs down the string with color.
  *
  * A vertical Pattern
  * TODO: Get Lindy's interpretation of this pattern.
  *
  * @param color     The color to use (start with?) in LwColor type
  * @param waveDelay The delay used to control the speed
  *                  (inverse of speed, lower number is faster)
  *
  */

void runningLights(LwColor color, int waveDelay) {
  for(Pixel i = 0; i < VERTICAL_NUM * 2; i++) {
    for(Pixel j = 0; j < VERTICAL_NUM; j++) {
      int p = j + i;

      setPixel(
        vertical,
        j,
        (LwColor) {
          COLOR_SIN_CONV(p, color.red),
          COLOR_SIN_CONV(p, color.green),
          COLOR_SIN_CONV(p, color.blue)
        }
      );
    }

    show(vertical);

    delay(waveDelay);
  }
}


/**
  * Pattern that randomly "twinkles" lights in the string.
  *
  * A vertical pattern
  *
  * @TODO: Get Lindy's interpretation of this pattern.
  *
  * @param count      The number of lights to twinkle in the string
  *                   (Sets up density)
  * @param speedDelay The delay used to control the speed
  *                   (inverse of speed, lower number is faster)
  * @param onlyOne    Only twinkles one light at a time?
  */

void twinkleRandom(int count, int speedDelay, boolean onlyOne) {
  setAllNil(vertical);

  for(int i = 0; i < count; i++) {
    setPixel(
      vertical,
      (Pixel)random(VERTICAL_NUM),
      (LwColor) {
        COLOR_VAL_RAND,
        COLOR_VAL_RAND,
        COLOR_VAL_RAND
      }
    );

    show(vertical);

    delay(speedDelay);

    if(onlyOne) {
      setAllNil(vertical);
    }

    //TODO: Delay? Show?
  }

  delay(speedDelay);
}


/**
  * Pattern that sparkles like snow!
  *
  * A vertical pattern
  *
  * @TODO: Get Lindy's interpretation of this pattern.
  *
  * @param color        The color to use (start with?) in LwColor type
  * @param sparkleDelay The delay used to control the duration of a "sparkle"?
  * @param speedDelay   The delay used to control the speed
  *                     (inverse of speed, lower number is faster)
  *
  */

void snowSparkle(LwColor color, int sparkleDelay, int speedDelay) {
  Pixel pixel = (Pixel)random(VERTICAL_NUM);

  // Set all pixles to passed in values
  setAll(vertical, color);

  // Set random pixel to white
  setPixel(vertical, pixel, LW_WHITE);

  show(vertical);

  delay(sparkleDelay);


  // Reset random pixel to passed in values
  setPixel(vertical, pixel, color);

  show(vertical);

  delay(speedDelay);
}


/**
  * Pattern that sparkles.
  *
  * A vertical pattern
  *
  * @TODO: Jane, you left off here 'cause brain tired.
  * @TODO: Get Lindy's interpretation of this pattern.
  *
  * @param color        The color to use (start with?) in LwColor type
  * @param sparkleDelay The delay used to control the duration of a "sparkle"?
  * @param speedDelay   The delay used to control the speed
  *                     (inverse of speed, lower number is faster)
  *
  */

void sparkle(LwColor color, int speedDelay) {
  Pixel pixel = (Pixel)random(VERTICAL_NUM);

  // Set random pixel to passed in values
  setPixel(vertical, pixel, color);

  show(vertical);

  delay(speedDelay);

  // Set random pixel to black
  setPixelNil(vertical, pixel);
}


void simpleWave(float rate, int cycles, int wait) {
  //
  // Vertical Pattern
  //

  // cycle through i times
  Pixel loopTest = VERTICAL_NUM * cycles;

  float pos = 0.0;

  for(Pixel i = 0; i < loopTest; i++, pos += rate) {
    for(Pixel j = 0; j < VERTICAL_NUM; i++) {
      float level = RGB_SINE(j, pos);

      setPixel(
        vertical,
        j,
        (LwColor){(byte)level, 255, 0}
      );
    }

    show(vertical);

    delay(wait);
  }
}


//################################
// Helpers for Adafruit_NeoPixels
//################################


void setPixel(NeoPix neopix, Pixel pixel, LwColor color) {

#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  neopix.setPixelColor(pixel, neopix.Color(color.red, color.green, color.blue));
#else
  // FastLED
  leds[pixel].r = color.red; //TODO: No idea.
  leds[pixel].g = color.green;
  leds[pixel].b = color.blue;
#endif

}


void setPixelNil(NeoPix neopix, Pixel pixel) {
  setPixel(neopix, pixel, LW_BLACK);
};


void setAll(NeoPix neopix, LwColor color) {
  for (Pixel i = 0; i < neopix.numPixels(); i++) {
    setPixel(neopix, i, color);
  }

  show(neopix);
}


void setAllNil(NeoPix neopix) {
  setAll(neopix, LW_BLACK);
}


void show(NeoPix neopix) {

#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  neopix.show();
#else
  // FastLED
  FastLED.show(); //TODO: Is this actually right?
#endif

}
