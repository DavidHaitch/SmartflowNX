#ifndef LEDCONTROL_H
#define LEDCONTROL_H
#define SPI_DATA 3
#define SPI_CLOCK 4
#include <FastLED.h>

//Defines that override FastLED defaults
#define SPI_DATA 3
#define SPI_CLOCK 4

#define TRUE_LEDS 138
#define NUM_LEDS 32
#define COLOR_ORDER BGR
#define DATA_PIN    3
#define CLOCK_PIN   4
#define LED_TYPE    SK9822
class LedControl
{
public:
    LedControl();
    uint8_t brightness;
    uint8_t maxBrightness;
    uint8_t minBrightness;
    CRGB leds[NUM_LEDS];
    void Clear();
    void Refresh();
private:
    CRGB trueLeds[TRUE_LEDS];
};
#endif