#include <Adafruit_NeoPixel.h>
#define PIN            6
#define NUMPIXELS      25
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Input 1 pattern 1 on and pattern 2");
  pixels.begin(); 
}
void loop() {
  if (Serial.available()) {
    int state = Serial.parseInt();
    if (state == 1) {
      Serial.println("Pattern 1 started");
      colorWipe(pixels.Color(255, 0, 0), 50); 
      Serial.println("Pattern 1 complete");
    }
    if (state == 2) {
      Serial.println("pattern 2 started");
      simpleWave(0.3, 5, 10);
      colorWipe(pixels.Color(255, 255, 0), 50); 
      Serial.println("patterm 2 complete");
    }
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, c);
    pixels.show();
    delay(wait);
  }
}

void simpleWave(float rate, int cycles, int wait) {
  float pos = 0.0;
  // cycle through x times
  for (int x = 0; x < (pixels.numPixels()*cycles); x++)
  {
    pos = pos + rate;
    for (int i = 0; i < pixels.numPixels(); i++) {
      float level = sin(i + pos) * 127 + 128;
      pixels.setPixelColor(i, (int)level, 255, 0);
    }
    pixels.show();
    delay(wait);
  }
}
