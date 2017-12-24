#include <Adafruit_NeoPixel.h>


#define NEOPIX_TYPE NEO_GRB + NEO_KHZ800
#define SERIAL_DATA_RATE 9600

#define VERTICAL_PIN 6
#define VERTICAL_NUM 10

#define SPIRAL_PIN 7
#define SPIRAL_NUM 10


typedef enum Goals {
  NONE   = 0,
  SMALL  = 1,
  MEDIUM = 2,
  BIG    = 4
} Goals;


Goals goal = NONE;

// Fairy lights
int ledPin = 9;

Adafruit_NeoPixel vertical = Adafruit_NeoPixel(VERTICAL_NUM, VERTICAL_PIN, NEOPIX_TYPE);
Adafruit_NeoPixel spiral   = Adafruit_NeoPixel(SPIRAL_NUM,   SPIRAL_PIN,   NEOPIX_TYPE);


void setup() {
  Serial.begin(SERIAL_DATA_RATE);
  
  // Wait for Serial to become active
  while (!Serial);

  Serial.println("1-3 are state chamges 4-9 are light shows");
 
  vertical.begin();
  vertical.show(); // Initialize all pixels to 'off'

  spiral.begin();
  spiral.show();   // Initialize all pixels to 'off'
}


void loop() {
  // Read the serial in and match a number passed in
  parseSerial();
  
  // Baised on the state of goal, do patterens
  parseGoals();
}


void parseGoals() {
  switch (goal) {
    case BIG:
      simpleFadeIn(9, 3);
      simpleFadeOut(9, 3);
      
      runningLights(0xff,0xff,0x00, 20);
    break;
    
    case MEDIUM:
      simpleFadeIn(9, 20);
      
      twinkleRandom(20, 100, false);
      
      simpleFadeOut(9, 20);
      twinkleRandom(20, 100, false);
    break;
    
    case SMALL:
      simpleFadeIn(9, 50);
      
      for (uint8_t i = 0; i < 10; i++) {
        snowSparkle(0x10, 0x10, 0x10, 20, random(10, 100));
      }
      
      simpleFadeOut(9, 50);
      
      for (uint8_t i = 0; i < 10; i++) {
        snowSparkle(0x10, 0x10, 0x10, 20, random(10, 100));
      }
    break;
    
    default:
      simpleFadeIn(9, 30);
      simpleFadeOut(9, 30);
    break;
  }
}


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
          cylonBounce(0xff, 0xff, 0, 4, 10, 10);
        break;

        case 5:
          cylonBounce(0xff, 0, 0, 4, 10, 50);
        break;

        case 6:
          colorCount(0x00, 0xff, 0x00, 50, 50);
        break;
      
        case 7:
          colorCount(0x00, 0xff, 0x00, 50, 100);
        break;

        case 8:
          colorCount(0x00, 0xff, 0x00, 50, 30);
        break;

        case 9:
          colorCount(0x00, 0xff, 0x00, 50, 10);
        break;
      };
    }
  }
}


// ##########
//
// Patterens
//
// ##########

void simpleFadeIn(int pin, int speedVal) {
  for (uint8_t fadeValue = 0; fadeValue <= 255; fadeValue += 5) {
    analogWrite(ledPin, fadeValue);

    delay(speedVal);
  } 
}


void simpleFadeOut(int pin, int speedVal) {
  for (uint8_t fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
    analogWrite(ledPin, fadeValue);

    delay(speedVal);
  }
}


void colorWipe(byte red, byte green, byte blue, int speedDelay) {
  for (uint16_t i = 0; i < VERTICAL_NUM; i++) {
    setPixel(i, red, green, blue);

    showVertical();

    delay(speedDelay);
  }
}


void colorCount(byte red, byte green, byte blue, int speedDelay, int percent) {
  //Modify percent
  percent = (VERTICAL_NUM * percent) / 100;

  for (uint16_t i = 0; i < SPIRAL_NUM; i++) {
    setPixelS(i, 0, 0, 0);

    showSpiral();

    delay(speedDelay);
  }

  Serial.println(percent);

  for (uint16_t i = 0; i < (uint16_t)percent; i++) {
    setPixelS(i, red, green, blue);

    showSpiral();

    delay(speedDelay);
  }
}


void runningLights(byte red, byte green, byte blue, int waveDelay) {
  int position = 0;

  for (int i = 0; i < VERTICAL_NUM * 2; i++, position++) {
    for (int i = 0; i < VERTICAL_NUM; i++) {
      setPixel(
        i,
        ((sin(i + position) * 127 + 128) / 255) * red,
        ((cos(i + position) * 127 + 128) / 255) * green,
        ((sin(i + position) * 127 + 128) / 255) * blue
      );
    }

    showVertical();

    delay(waveDelay);
  }
}


// ##########
//
// Spiral Patterens
//
// ##########

//TODO: It is labled as sprial, includes spiral, but also has vertical?
void cylonBounce(byte red, byte green, byte blue, int eyeSize, int speedDelay, int returnDelay) {
  //
  // Spiral pattern
  //

  uint16_t loopTest = VERTICAL_NUM - eyeSize - 2;

  for (uint16_t i = 0; i < loopTest; i++) {
    setAllS(0, 0, 0);

    setPixelS(i, red / 10, green / 10, blue / 10);
    
    // Inner-loop
    for (uint16_t j = 1; j <= (uint16_t)eyeSize; j++) {
      setPixelS(i + j, red, green, blue);
    }

    uint16_t n = i + eyeSize + 1;

    setPixelS(n, red / 10, green / 10, blue / 10);

    showSpiral();

    delay(speedDelay);
  }

  delay(returnDelay);

  uint16_t loopInit = SPIRAL_NUM - eyeSize - 2;

  for (uint16_t i = loopInit; i > 0; i--) {
    setAllS(0, 0, 0);

    setPixelS(i, red / 10, green / 10, blue / 10);

    // Inner-loop
    for (int j = 1; j <= eyeSize; j++) {
      setPixelS(i + j, red, green, blue);
    }

    uint16_t n = i + eyeSize + 1;
 
    setPixelS(n, red / 10, green / 10, blue / 10);

    showSpiral();

    delay(speedDelay);
  }

  delay(returnDelay);
}


// ##########
//
// Vertical Patterns
//
// ##########

void twinkleRandom(int count, int speedDelay, boolean onlyOne) {
  //
  // Vertical Pattern
  //
  
  setAll(0, 0, 0);

  for (int i = 0; i < count; i++) {
    setPixel(
      random(VERTICAL_NUM),
      random(0, 255),
      random(0, 255),
      random(0, 255)
    );

    showVertical();

    delay(speedDelay);

    if (onlyOne) {
      setAll(0, 0, 0);
    }
  }

  delay(speedDelay);
}


void snowSparkle(byte red, byte green, byte blue, int sparkleDelay, int speedDelay) {
  //
  // Vertical Pattern
  //

  int pixel = random(VERTICAL_NUM);

  // Set all pixles to passed in values
  setAll(red, green, blue);
  
  // Set random pixel to white
  setPixel(pixel, 0xff, 0xff, 0xff);

  showVertical();

  delay(sparkleDelay);


  // Reset random pixel to passed in values
  setPixel(pixel, red, green, blue);

  showVertical();

  delay(speedDelay);
}


void sparkle(byte red, byte green, byte blue, int speedDelay) {
  //
  // Vertical Pattern
  //

  int pixel = random(VERTICAL_NUM);

  // Set random pixel to passed in values
  setPixel(pixel, red, green, blue);

  showVertical();

  delay(speedDelay);

  // Set random pixel to black
  setPixel(pixel, 0, 0, 0);
}


//TODO:  Why not use VERTICAL_NUM in here?
void simpleWave(float rate, int cycles, int wait) {
  //
  // Vertical Patern
  //

  float pos = 0.0;

  // cycle through x times
  uint8_t loopTest = vertical.numPixels() * cycles;

  for (int x = 0; x < loopTest; x++) {
    pos = pos + rate;

    for (uint16_t i = 0; i < vertical.numPixels(); i++) {
      float level = sin(i + pos) * 127 + 128;

      vertical.setPixelColor(i, (int)level, 255, 0);
    }

    vertical.show();

    delay(wait);
  }
}


// ##########
//
// Helpers for Vertical
//
// ##########

void setPixel(uint16_t pixel, byte red, byte green, byte blue) {

#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  vertical.setPixelColor(pixel, vertical.Color(red, green, blue));
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[pixel].r = red;
  leds[pixel].g = green;
  leds[pixel].b = blue;
#endif

}


void setAll(byte red, byte green, byte blue) {
  for (uint16_t i = 0; i < VERTICAL_NUM; i++) {
    setPixel(i, red, green, blue);
  }

  showVertical();
}

void showVertical() {

#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  vertical.show();
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  FastLED.show();
#endif

}


// ##########
//
// Helpers for Spiral
//
// ##########

void setPixelS(uint16_t pixel, byte red, byte green, byte blue) {

#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  spiral.setPixelColor(pixel, spiral.Color(red, green, blue));
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[pixel].r = red;
  leds[pixel].g = green;
  leds[pixel].b = blue;
#endif

}


void setAllS(byte red, byte green, byte blue) {
  for (uint16_t i = 0; i < SPIRAL_NUM; i++ ) {
    setPixelS(i, red, green, blue);
  }

  showSpiral();
}


void showSpiral() {

#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  spiral.show();
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  FastLED.show();
#endif

}
