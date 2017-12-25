//
// Simple Definitions
//

#define COLOR_VAL_MIN 0
#define COLOR_VAL_MAX 255 // Fits in uint8_t
#define SERIAL_DATA_RATE 9600
#define NEOPIX_TYPE NEO_GRB + NEO_KHZ800

#define LW_BLACK (LwColor){0x00, 0x00, 0x00}
#define LW_RED   (LwColor){0xff, 0x00, 0x00}
#define LW_GREEN (LwColor){0x00, 0xff, 0x00}
#define LW_BLUE  (LwColor){0x00, 0x00, 0xff}
#define LW_WHITE (LwColor){0xff, 0xff, 0xff}

// The "Maypole" (Addressable)
#define VERTICAL_PIN 6
#define VERTICAL_NUM 10

// The XYZ11 "Christmas style" lights (Addressable)
#define SPIRAL_PIN 7
#define SPIRAL_NUM 10

// Fairy lights (Controlled by FizzPop)
#define FAIRY_PIN 9


//
// Macros
//

// See: https://krazydad.com/tutorials/makecolors.php
#define COLOR_SIN_FULL(f, i, c) (((sin(f + i) * 127 + 128) / COLOR_VAL_MAX) * c)
// #                                  ^   ^    ^     ^      ^                ^ Color Value
// #                                  |   |    |     |      = Maximum Color Value
// #                                  |   |    |     = Center point (255/2)
// #                                  |   |    = Amplitude ~(255/2)
// #                                  |   = Interval
// #                                  = Frequency

#define RGB_SINE(f, i) (sin(f + i) * 127 + 128)
#define COLOR_SIN(p, c) ((RGB_SINE(p, 0) / COLOR_VAL_MAX) * c)

#define DIV_CONV(a, b) ((byte)(a / b))
#define COLOR_SIN_CONV(p, c) ((byte)COLOR_SIN(p, c))
#define COLOR_VAL_RAND ((byte)random(COLOR_VAL_MIN, COLOR_VAL_MAX))


// Typedefs
typedef uint16_t Pixel;

typedef Adafruit_NeoPixel NeoPix;

typedef enum Goals {
  NONE   = 0,
  SMALL  = 1,
  MEDIUM = 2,
  BIG    = 4
} Goals;


typedef struct LwColor {
  byte red;
  byte green;
  byte blue;
} LwColor;

// "Gloabals"
Goals goal = NONE;

NeoPix vertical = Adafruit_NeoPixel(VERTICAL_NUM, VERTICAL_PIN, NEOPIX_TYPE);
NeoPix spiral   = Adafruit_NeoPixel(SPIRAL_NUM,   SPIRAL_PIN,   NEOPIX_TYPE);


// ##########
//
// Function Headers
//
// ##########

void setupSerial();
void setupNeopix();
void parseGoals();
void parseSerial();
void simpleFadeIn(int pin, int speedVal);
void simpleFadeOut(int pin, int speedVal);
void colorCount(LwColor color, int speedDelay, int percent);
void cylonBounce(LwColor color, int eyeSize, int speedDelay, int returnDelay);
void colorWipe(LwColor color, int speedDelay);
void runningLights(LwColor color, int waveDelay);
void twinkleRandom(int count, int speedDelay, boolean onlyOne);
void snowSparkle(LwColor color, int sparkleDelay, int speedDelay);
void sparkle(LwColor color, int speedDelay);
void simpleWave(float rate, int cycles, int wait);
void setPixel(NeoPix neopix, Pixel pixel, struct LwColor color);
void setPixelNil(NeoPix neopix, Pixel pixel);
void setAll(NeoPix neopix, LwColor color);
void setAllNil(NeoPix neopix);
void show(NeoPix neopix);


