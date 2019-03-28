#include "LedControl.h"
const int refreshRate = 0;
LedControl::LedControl()
{
    brightness = 255;
    minBrightness = 0;
    maxBrightness = 255;
    FastLED.addLeds<LED_TYPE, BGR>(trueLeds, TRUE_LEDS).setCorrection(TypicalLEDStrip);
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
    int midpointOne = (TRUE_LEDS / 4) - 1;
    int midpointTwo = (TRUE_LEDS / 2) + midpointOne + 2;
    uint8_t b = map(brightness, 0, 255, minBrightness, maxBrightness);

    trueLeds[midpointOne] = CRGB::Black;
    trueLeds[midpointTwo] = CRGB::Black;

    //Segment One
    for(int i = 0; i < NUM_LEDS; i++)
    {
        trueLeds[midpointOne - i - 1] = leds[i];
    }

    //Segment Two
    for(int i = 0; i < NUM_LEDS; i++)
    {
        trueLeds[midpointOne + i + 1] = leds[i];
    }

    //Segment Three
    for(int i = 0; i < NUM_LEDS; i++)
    {
        trueLeds[midpointTwo - i - 1] = leds[i];
    }

    //Segment Four
    for(int i = 0; i < NUM_LEDS; i++)
    {
        trueLeds[midpointTwo + i + 1] = leds[i];
    }

    FastLED.setBrightness(b);
    FastLED.show();
}

void LedControl::Clear()
{
    for (int i = 0; i < TRUE_LEDS; i++)
    {
        trueLeds[i] = CRGB::Black;
    }

    FastLED.show();
}