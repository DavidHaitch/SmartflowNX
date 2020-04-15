#include "LedControl.h"
const int refreshRate = 0;
LedControl::LedControl()
{
    brightness = 255;
    minBrightness = 0;
    maxBrightness = 255;
    addressingMode = Mirror;
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(trueLeds, TRUE_LEDS).setCorrection(TypicalLEDStrip);
    Clear();
}

void LedControl::Refresh()
{
    long now = micros();
    if (now - lastUpdate < fps)
    {
        return;
    }

    lastUpdate = micros();
#ifdef STAFF
    int midpointOffset = 0;
#endif
#ifdef BATON
    int midpointOffset = 1;
#endif
    int midpointOne = (TRUE_LEDS / 4) - midpointOffset;
    int midpointTwo = (TRUE_LEDS / 2) + midpointOne + midpointOffset;
    uint8_t b = map(brightness, 0, 255, minBrightness, maxBrightness);
    FastLED.setBrightness(b);

    if (addressingMode == Centered)
    {
        //Segment One
        for (int i = 0; i < NUM_LEDS; i++)
        {
            trueLeds[midpointOne - i - 1] = leds[i];
        }

        //Segment Two
        for (int i = 0; i < NUM_LEDS; i++)
        {
            trueLeds[midpointOne + i + 1] = leds[i];
        }

        //Segment Three
        for (int i = 0; i < NUM_LEDS; i++)
        {
            trueLeds[midpointTwo - i - 1] = leds[i];
        }

        //Segment Four
        for (int i = 0; i < NUM_LEDS; i++)
        {
            trueLeds[midpointTwo + i + 1] = leds[i];
        }
    }
    else if (addressingMode == Mirror)
    {
        for (int i = 0; i < TRUE_LEDS / 2; i++)
        {
            trueLeds[i] = leds[i];
            trueLeds[(TRUE_LEDS / 2) + i] = leds[(TRUE_LEDS / 2) - i - 1];
        }
    }
    else if (addressingMode == Direct)
    {
        for (int i = 0; i < TRUE_LEDS; i++)
        {
            trueLeds[i] = leds[i];
        }
    }

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