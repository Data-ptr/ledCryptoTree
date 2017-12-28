#include <Adafruit_NeoPixel.h>
#define verticalPIN 6
#define VerticalNUM 700
#define spiralPIN 7
#define spiralNUM 10
bool goal[] = {
  false, false, false
};
bool randomMode = false;
int ledPin = 2;
Adafruit_NeoPixel vertical = Adafruit_NeoPixel(VerticalNUM, verticalPIN, NEO_GRB + NEO_KHZ400);
Adafruit_NeoPixel spiral = Adafruit_NeoPixel(spiralNUM, spiralPIN, NEO_GRB + NEO_KHZ400);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("1-3 are state chamges 4-9 are light shows");
  Serial.println("1 snowflake mode");
  Serial.println("2 twinkle mode");
  Serial.println("3 super donate mode");
  Serial.println("4 slow color climb");
  Serial.println("5 longer color wave");
  Serial.println("6 multicolor supertinkle");
  Serial.println("7 strobe");
  Serial.println("8 color strobe");
  Serial.println("9 RAINBOW");
  Serial.println("11 toggle random mode (may take time to switch)");
  vertical.begin();
  vertical.show(); // Initialize all pixels to 'off'

}
void loop() {
  if (randomMode) {
    colorWipe(random(0, 255), random(0, 200), random(0, 200), 0);
    colorWipe(random(0, 200), random(0, 200), random(0, 200), 0);
    colorWipe(random(0, 200), random(0, 200), random(0, 200), 0);
    for (int i = 10; i < 5; i++) {
      RunningLights(random(0, i * 255), random(0, i * 255), random(0, i * 255), 20);
      colorInsta(random(0, i * 100), random(0, i * 100), 0x00, 0);
      delay(100);
      colorInsta(0xff, random(0, i * 100), random(0, i * 100), 0);
      delay(100);
      colorInsta(0xff, random(0, i * 100), 0x00, 0);
      delay(100);
    }
    for (int i = 0; i < random(0, 10); i++) {
      theaterChase(random(0, 255), random(0, 255), random(0, 255), 50);
    }
    for (int i = 0; i < random(0, 1000); i++) {
      colorInsta(0x50, 0x50, 0x50, 0);
      delay(20);
      colorInsta(0x00, 0x00, 0x00, 0);
      delay(20);
    }
    for (int i = 0; i < random(0, 10); i++) {
      rainbowCycle(5);
    }
  } else {

    if ((goal[0]) && (goal[1]) && (goal[2])) {
      RunningLights(0xff, 0xff, 0x00, 1);
    } else if (goal[0] && goal[1] && !goal[2]) {
      TwinkleRandom(200, 1, false);
    } else if (goal[0] && !goal[1] && !goal[2]) {
      //for (int i = 0; i < 10; i++) {
      SnowSparkle(0x10, 0x10, 0x10, 200, random(10, 100));
      //}

    } else {

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
        colorWipe(random(0, 255), random(0, 200), random(0, 200), 0);
        colorWipe(random(0, 200), random(0, 200), random(0, 200), 0);
        colorWipe(random(0, 200), random(0, 200), random(0, 200), 0);
      }
      if (state == 5) {
        for (int i = 10; i < 5; i++) {
          RunningLights(random(0, i * 255), random(0, i * 255), random(0, i * 255), 20);
          colorInsta(random(0, i * 100), random(0, i * 100), 0x00, 0);
          delay(100);
          colorInsta(0xff, random(0, i * 100), random(0, i * 100), 0);
          delay(100);
          colorInsta(0xff, random(0, i * 100), 0x00, 0);
          delay(100);
        }
      }
      if (state == 6) {
        for (int i = 0; i < random(0, 10); i++) {
          theaterChase(random(0, 255), random(0, 255), random(0, 255), 50);
        }
      }
      if (state == 7) {
        for (int i = 0; i < random(0, 1000); i++) {
          colorInsta(0x50, 0x50, 0x50, 0);
          delay(20);
          colorInsta(0x00, 0x00, 0x00, 0);
          delay(20);
        }
      }
      if (state == 8) {
        for (int i = 0; i < random(0, 100); i++) {
          colorInsta(random(0, 100), random(0, 100), 0x00, 0);
          delay(100);
          colorInsta(random(0, 100), 0x00, random(0, 100), 0);
          delay(random(0, 1000));
          colorInsta(random(0, 100), 0xff, random(0, 100), 0);
          delay(random(0, 1000));
          colorInsta(random(0, 100), 0x30, 0x50, 0);
        }
      }
      if (state == 9) {
        for (int i = 0; i < random(0, 10); i++) {
          rainbowCycle(5);
        }
      }
      if (state == 11) {
        if (randomMode) {
          randomMode = false;
          Serial.println("random was on, now its off");
        } if (!randomMode) {
          randomMode = true;
          Serial.println("random was off, now its on");
        }
      }
    }
  }
}
void theaterChase(byte red, byte green, byte blue, int SpeedDelay) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < 700; i = i + 3) {
        setPixel(i + q, red, green, blue);  //turn every third pixel on
      }
      showvertical();

      delay(SpeedDelay);

      for (int i = 0; i < 700; i = i + 3) {
        setPixel(i + q, 0, 0, 0);    //turn every third pixel off
      }
    }
  }
}
void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for (j = 0; j < 256; j += 10) { // 5 cycles of all colors on wheel
    for (i = 0; i < 700; i++) {
      c = Wheel(((i * 256 / 700) + j) & 255);
      setPixel(i, *c, *(c + 1), *(c + 2));
    }
    showvertical();
    delay(SpeedDelay);
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }

  return c;
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
  for (uint16_t i = 0; i < 700; i++) {
    setPixel(i, red, green, blue);
    showvertical();
    //delay(SpeedDelay);
  }
}
void colorInsta(byte red, byte green, byte blue, int SpeedDelay) {
  for (uint16_t i = 0; i < 700; i++) {
    setPixel(i, red, green, blue);

    //delay(SpeedDelay);
  }
  showvertical();
}
void colorCount(byte red, byte green, byte blue, int SpeedDelay, int percent) {
  percent = (VerticalNUM * percent) / 100;
  for (uint16_t i = 0; i < spiralNUM; i++) {
    setPixel(i, 0, 0, 0);
    showvertical();
    delay(SpeedDelay);
  }
  Serial.println(percent);
  for (uint16_t i = 0; i < percent; i++) {
    setPixel(i, red, green, blue);
    showvertical();
    delay(SpeedDelay);
  }
}
void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position = 0;
  for (int i = 0; i < 700; i += 20)
  {
    Position += 10; // = 0; //Position + Rate;
    for (int i = 0; i < 700; i++) {
      setPixel(i, ((sin(i + Position) * 127 + 128) / 255)*red,
               ((cos(i + Position) * 127 + 128) / 255)*green,
               ((sin(i + Position) * 127 + 128) / 255)*blue);
    }
    showvertical();
  }
}
void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for (int i = 0; i < 700 - EyeSize - 2; i++) {
    setAll(0, 0, 0);
    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    showvertical();
    // delay(SpeedDelay);
  }
  //delay(ReturnDelay);
  for (int i = spiralNUM - EyeSize - 2; i > 0; i--) {
    setAll(0, 0, 0);
    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    showvertical();
    //delay(SpeedDelay);
  }

  //delay(ReturnDelay);
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
  //delay(SparkleDelay);
  setPixel(Pixel, red, green, blue);
  showvertical();
  // delay(SpeedDelay);
}
void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  // Vertical Pattern
  int Pixel = random(VerticalNUM);
  setPixel(Pixel, red, green, blue);
  showvertical();
  delay(SpeedDelay);
  setPixel(Pixel, 0, 0, 0);
}
//simpleWave(10, 1, 10);
void simpleWave(float rate, int cycles, int wait, int r, int g, int b) {
  // Vertical Patern
  float pos = 0.0;
  // cycle through x times
  for (int x = 0; x < (vertical.numPixels()*cycles); x += 20)
  {
    pos = pos + rate;
    for (int i = 0; i < vertical.numPixels(); i++) {
      float level = sin(i + pos) * 127 + 128;
      vertical.setPixelColor(i, (int)level, g, b);
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

