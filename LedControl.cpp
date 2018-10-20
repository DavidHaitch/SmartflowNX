#include "LedControl.h"
const int refreshRate = 0;
LedControl::LedControl()
{
    brightness = 255;
    minBrightness = 0;
    maxBrightness = 255;
    FastLED.addLeds<LED_TYPE, BGR>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    Clear();
}

void LedControl::Refresh()
{
    uint8_t b = map(brightness, 0, 255, minBrightness, maxBrightness);
    
    FastLED.setBrightness(b);
    FastLED.show();
}

void LedControl::Clear()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Black;
    }

    FastLED.show();
}