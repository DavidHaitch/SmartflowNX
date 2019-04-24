#ifndef LEDCONTROL_H
#define LEDCONTROL_H
//Defines that override FastLED defaults
#define SPI_DATA 3
#define SPI_CLOCK 4
#include <FastLED.h>

#define TRUE_LEDS 138
#define NUM_LEDS 34
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
    CRGB leds[TRUE_LEDS];
    CRGB trueLeds[TRUE_LEDS];
    void Clear();
    void Refresh();
    bool directMode;
private:
    long lastUpdate;
    const int fps = 100000 / 240;
};
#endif