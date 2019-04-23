#include "LedControl.h"
const int refreshRate = 0;
LedControl::LedControl()
{
    brightness = 255;
    minBrightness = 0;
    maxBrightness = 255;
    directMode = false;
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    Clear();
}

void LedControl::Refresh()
{
    long now = micros();
    if(now - lastUpdate < fps)
    {
        return;
    }

    lastUpdate = micros();
    uint8_t b = map(brightness, 0, 255, minBrightness, maxBrightness);
    FastLED.setBrightness(b);

    for(int i = 0; i < NUM_LEDS / 2; i++)
    {
        leds[i] = leds[i];
        leds[(NUM_LEDS/2) + i] = leds[(NUM_LEDS/2) - i - 1];
    }
    

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