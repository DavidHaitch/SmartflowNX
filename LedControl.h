#ifndef LEDCONTROL_H
#define LEDCONTROL_H
#include <FastLED.h>

#define NUM_LEDS    138
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
    long lastRefresh;
};
#endif