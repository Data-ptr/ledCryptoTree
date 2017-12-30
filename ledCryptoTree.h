/** @file */

//####################
// Simple Definitions
//####################

#define COLOR_VAL_MIN 0
#define COLOR_VAL_MAX 255 //** Fits in uint8_t
#define SERIAL_DATA_RATE 9600
#define NEOPIX_TYPE NEO_GRB + NEO_KHZ800
#define SERIAL_INTRO_TEXT "1-3 are state chamges 4-9 are light shows"

//** The "Maypole" (Addressable)
#define VERTICAL_PIN 6
#define VERTICAL_NUM 10

//** The XYZ11 Neopixel "Christmas style" lights (Addressable)
#define SPIRAL_PIN 7
#define SPIRAL_NUM 10

//** Fairy lights (Controlled by FizzPop)
#define FAIRY_PIN 9


//######################
// Commonly used colors
//######################

#define LW_BLACK (LwColor){0x00, 0x00, 0x00}
#define LW_RED   (LwColor){0xff, 0x00, 0x00}
#define LW_GREEN (LwColor){0x00, 0xff, 0x00}
#define LW_BLUE  (LwColor){0x00, 0x00, 0xff}
#define LW_WHITE (LwColor){0xff, 0xff, 0xff}


//########
// Macros
//########

// See: https://krazydad.com/tutorials/makecolors.php
#define COLOR_SIN_FULL(f, i, c) (((sin(f + i) * 127 + 128) / COLOR_VAL_MAX) * c)
// #                                   ^   ^    ^     ^      ^    Color Value ^
// #                                   |   |    |     |      = Maximum Color Value
// #                                   |   |    |     = Center point (255/2)
// #                                   |   |    = Amplitude ~(255/2)
// #                                   |   = Interval
// #                                   = Frequency

#define RGB_SINE(f, i) (sin(f + i) * 127 + 128)
#define COLOR_SIN(p, c) ((RGB_SINE(p, 0) / COLOR_VAL_MAX) * c)

#define DIV_CONV(a, b) ((byte)(a / b))
#define COLOR_SIN_CONV(p, c) ((byte)COLOR_SIN(p, c))
#define COLOR_VAL_RAND ((byte)random(COLOR_VAL_MIN, COLOR_VAL_MAX))


//######################
// Type (re)definitions
//######################

typedef uint16_t Pixel;

typedef Adafruit_NeoPixel NeoPix;


//#############
// Enumerables
//#############

/**
  * For keeping track of the goal state.
  *
  */

typedef enum Goals {
  NONE   = 0,
  SMALL  = 1,
  MEDIUM = 2,
  BIG    = 4
} Goals;


//############
// Structures
//############

typedef struct LwColor {
  byte red;   /**< The red byte   */
  byte green; /**< The green byte */
  byte blue;  /**< The blue byte  */
} LwColor;


//#####################
// "Gloabal" Variables
//#####################

Goals goal = NONE;

NeoPix vertical = Adafruit_NeoPixel(VERTICAL_NUM, VERTICAL_PIN, NEOPIX_TYPE);
NeoPix spiral   = Adafruit_NeoPixel(SPIRAL_NUM,   SPIRAL_PIN,   NEOPIX_TYPE);


//##################
// Function Headers
//##################

void setupSerial();
void setupNeopix();

void parseSerial();
void parseGoals();

void simpleFadeIn(int, int);
void simpleFadeOut(int, int);

void colorCount(LwColor, int, int);
void cylonBounce(LwColor, int, int, int);
void colorWipe(LwColor, int);
void runningLights(LwColor, int);
void twinkleRandom(int, int, bool);
void snowSparkle(LwColor, int, int);
void sparkle(LwColor, int);
void simpleWave(float, int, int);

void setPixel(NeoPix, Pixel, LwColor);
void setPixelNil(NeoPix, Pixel);
void setAll(NeoPix, LwColor);
void setAllNil(NeoPix);
void show(NeoPix);
