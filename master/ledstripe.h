#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS_ALL, LED_PIN, NEO_GRB + NEO_KHZ800);

int LED_STATS[NUM_PIXELS_ALL];
uint32_t LED_COLOR = 0xFF0000; // start with red
int LED_BLACK_PIXEL = 0;

bool ledAnimActive = false;
int ledAnimDelayMs = 50;
int ledDelayCurrent = 0;
int ledBrightness = BRIGHTNESS;
bool ledFadeOut = false;
