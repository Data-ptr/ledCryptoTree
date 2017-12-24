#include <Adafruit_NeoPixel.h>
#define verticalPIN 6
#define VerticalNUM 10
#define spiralPIN 7
#define spiralNUM 10
bool goal[] = {
  false, false, false
};
int ledPin = 9;
Adafruit_NeoPixel vertical = Adafruit_NeoPixel(VerticalNUM, verticalPIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel spiral = Adafruit_NeoPixel(spiralNUM, spiralPIN, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(9600);
  
  //Wait for Serial to become active
  while (!Serial);
  Serial.println("1-3 are state chamges 4-9 are light shows");
  vertical.begin();
  vertical.show(); // Initialize all pixels to 'off'
  spiral.begin();
  spiral.show(); // Initialize all pixels to 'off'
}
void loop() {
  
  if ((goal[0]) && (goal[1]) && (goal[2])) {
    simpleFadeIn(9, 3);
    simpleFadeOut(9, 3);
    RunningLights(0xff,0xff,0x00, 20);

  } else if (goal[0] && goal[1] && !goal[2]) {
    simpleFadeIn(9, 20);
      TwinkleRandom(20, 100, false);

    simpleFadeOut(9, 20);
      TwinkleRandom(20, 100, false);
  } else if (goal[0] && !goal[1] && !goal[2]) {
    simpleFadeIn(9, 50);
    for (int i = 0; i < 10; i++) {
      SnowSparkle(0x10, 0x10, 0x10, 20, random(10, 100));
    }
    simpleFadeOut(9, 50);
    for (int i = 0; i < 10; i++) {
      SnowSparkle(0x10, 0x10, 0x10, 20, random(10, 100));
    }
  } else {
    simpleFadeIn(9, 30);
    simpleFadeOut(9, 30);
  }
  if (Serial.available()) {
    int state = Serial.parseInt();
    if (state == 1) {
      goal[0] = true;
      goal[1] = false;
      goal[2] = false;
    }
    if (state == 2) {
      goal[0] = true;
      goal[1] = true;
      goal[2] = false;
    }
    if (state == 3) {
      goal[0] = true;
      goal[1] = true;
      goal[2] = true;
    }
    if (state == 4) {
      CylonBounce(0xff, 0xff, 0, 4, 10, 10);
    }
    if (state == 5) {
      CylonBounce(0xff, 0, 0, 4, 10, 50);
    }
    if (state == 6) {
      colorCount(0x00, 0xff, 0x00, 50, 50);
    }
    if (state == 7) {
      colorCount(0x00, 0xff, 0x00, 50, 100);
    }
    if (state == 8) {
      colorCount(0x00, 0xff, 0x00, 50, 30);
    }
    if (state == 9) {
      colorCount(0x00, 0xff, 0x00, 50, 10);
    }
  }
}

void simpleFadeIn(int pin, int speedVal) {
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
    analogWrite(ledPin, fadeValue);
    delay(speedVal);
  }

}
void simpleFadeOut(int pin, int speedVal) {
  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
    analogWrite(ledPin, fadeValue);
    delay(speedVal);
  }
}
void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for (uint16_t i = 0; i < VerticalNUM; i++) {
    setPixel(i, red, green, blue);
    showvertical();
    delay(SpeedDelay);
  }
}
void colorCount(byte red, byte green, byte blue, int SpeedDelay, int percent) {
  percent = (VerticalNUM * percent) / 100;
  for (uint16_t i = 0; i < spiralNUM; i++) {
    setPixelS(i, 0, 0, 0);
    showspiral();
    delay(SpeedDelay);
  }
  Serial.println(percent);
  for (uint16_t i = 0; i < percent; i++) {
    setPixelS(i, red, green, blue);
    showspiral();
    delay(SpeedDelay);
  }
}
void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position = 0;
  for (int i = 0; i < VerticalNUM * 2; i++)
  {
    Position++; // = 0; //Position + Rate;
    for (int i = 0; i < VerticalNUM; i++) {
      // sine wave, 3 offset waves make a rainbow!
      //float level = sin(i+Position) * 127 + 128;
      //setPixel(i,level,0,0);
      //float level = sin(i+Position) * 127 + 128;
      setPixel(i, ((sin(i + Position) * 127 + 128) / 255)*red,
               ((cos(i + Position) * 127 + 128) / 255)*green,
               ((sin(i + Position) * 127 + 128) / 255)*blue);
    }

    showvertical();
    delay(WaveDelay);
  }
}
void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
// Spiral pattern
  for (int i = 0; i < VerticalNUM - EyeSize - 2; i++) {
    setAllS(0, 0, 0);
    setPixelS(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixelS(i + j, red, green, blue);
    }
    setPixelS(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    showspiral();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
  for (int i = spiralNUM - EyeSize - 2; i > 0; i--) {
    setAllS(0, 0, 0);
    setPixelS(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixelS(i + j, red, green, blue);
    }
    setPixelS(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    showspiral();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);
}

// Vertical Patterns
void TwinkleRandom(int Count, int SpeedDelay, boolean OnlyOne) {
// Vertical Patern
  setAll(0, 0, 0);
  for (int i = 0; i < Count; i++) {
    setPixel(random(VerticalNUM), random(0, 255), random(0, 255), random(0, 255));
    showvertical();
    delay(SpeedDelay);
    if (OnlyOne) {
      setAll(0, 0, 0);
    }
  }
  delay(SpeedDelay);
}
void SnowSparkle(byte red, byte green, byte blue, int SparkleDelay, int SpeedDelay) {
// Vertical Pattern
  setAll(red, green, blue);

  int Pixel = random(VerticalNUM);
  setPixel(Pixel, 0xff, 0xff, 0xff);
  showvertical();
  delay(SparkleDelay);
  setPixel(Pixel, red, green, blue);
  showvertical();
  delay(SpeedDelay);
}
void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
 // Vertical Pattern
  int Pixel = random(VerticalNUM);
  setPixel(Pixel, red, green, blue);
  showvertical();
  delay(SpeedDelay);
  setPixel(Pixel, 0, 0, 0);
}

void simpleWave(float rate, int cycles, int wait) {
  // Vertical Patern
  float pos = 0.0;
  // cycle through x times
  for (int x = 0; x < (vertical.numPixels()*cycles); x++)
  {
    pos = pos + rate;
    for (int i = 0; i < vertical.numPixels(); i++) {
      float level = sin(i + pos) * 127 + 128;
      vertical.setPixelColor(i, (int)level, 255, 0);
    }
    vertical.show();
    delay(wait);
  }
}


// Helpers for Vertical
void setPixel(int Pixel, byte red, byte green, byte blue) {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  vertical.setPixelColor(Pixel, vertical.Color(red, green, blue));
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
#endif
}
void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < VerticalNUM; i++ ) {
    setPixel(i, red, green, blue);
  }
  showvertical();
}
void showvertical() {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  vertical.show();
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  FastLED.show();
#endif
}

// Helpers for Spiral
void setPixelS(int Pixel, byte red, byte green, byte blue) {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  spiral.setPixelColor(Pixel, spiral.Color(red, green, blue));
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
#endif
}
void setAllS(byte red, byte green, byte blue) {
  for (int i = 0; i < spiralNUM; i++ ) {
    setPixelS(i, red, green, blue);
  }
  showspiral();
}
void showspiral() {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  spiral.show();
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  FastLED.show();
#endif
}
