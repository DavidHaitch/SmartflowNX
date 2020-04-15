#ifndef LEDCONTROL_H
#define LEDCONTROL_H
#include "HardwareType.h"

//Defines that override FastLED defaults
#ifdef BATON
#define SPI_DATA 7
#define SPI_CLOCK 8
#endif

#ifdef STAFF
#define SPI_DATA 3
#define SPI_CLOCK 4
#endif
#include <FastLED.h>

#ifdef BATON
#define TRUE_LEDS 144
#define NUM_LEDS 35
#define COLOR_ORDER BGR
#define DATA_PIN    7
#define CLOCK_PIN   8
#define LED_TYPE    APA102
#endif

#ifdef STAFF
#define TRUE_LEDS 138
#define NUM_LEDS 34
#define COLOR_ORDER BGR
#define DATA_PIN    3
#define CLOCK_PIN   4
#define LED_TYPE    SK9822
#endif

enum LedAddressingMode {Direct, Mirror, Centered};

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
    LedAddressingMode addressingMode;
    //bool directMode;
private:
    long lastUpdate;
    const int fps = 100000 / 240;
};
#endif