#ifndef LEDCONTROL_H
#define LEDCONTROL_H
//Defines that override FastLED defaults
#define SPI_DATA 7
#define SPI_CLOCK 8
#include <FastLED.h>

#define NUM_LEDS 144
#define COLOR_ORDER BGR
#define DATA_PIN    7
#define CLOCK_PIN   8
#define LED_TYPE    APA102
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
    bool directMode;
private:
    long lastUpdate;
    const int fps = 100000 / 240;
};
#endif