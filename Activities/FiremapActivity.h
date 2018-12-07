#ifndef FIREMAPACTIVITY_H
#define FIREMAPACTIVITY_H
#include "LedActivity.h"

class FiremapActivity : public LedActivity {
public:
    FiremapActivity(MotionState* _motionState, LedControl* _ledControl) : LedActivity(_motionState, _ledControl)
    {
        palette = CRGBPalette16(CRGB::Black, CRGB::Red, CRGB::Black, CRGB::OrangeRed, CRGB::Black, CRGB::Orange, CRGB::Black, CRGB::Yellow,
                                CRGB::Black, CRGB::Orange, CRGB::Black, CRGB::DarkOrange, CRGB::Black, CRGB::OrangeRed, CRGB::Black, CRGB::DarkRed);
        // palette = CRGBPalette16(CRGB::DarkRed, CRGB::Red, CRGB::DarkRed, CRGB::OrangeRed, CRGB::DarkOrange, CRGB::Orange, CRGB::Yellow, CRGB::Yellow,
        //                         CRGB::Yellow, CRGB::Yellow, CRGB::Orange, CRGB::DarkOrange, CRGB::OrangeRed, CRGB::DarkRed, CRGB::Red, CRGB::DarkRed);
    }

    CRGBPalette16 palette;

    bool enter(int param)
    {
        ledControl->minBrightness = 0;
    }

    bool update(int param)
    {
        if(millis() - lastShiftTime > 100)
        {
            lastShiftTime = millis();
            shift += 1;
        }

        int fourth = NUM_LEDS / 4;  
        for (int i = 0; i <= fourth; i++)
        {
            float r = baseDistance + (stepDistance * (i + 1));
            int c = inoise8(abs(motionState->pointingX) * r, abs(motionState->pointingY) * r, abs(motionState->pointingZ) * r);            
            CRGB color = ColorFromPalette( palette, c + shift, 255, LINEARBLEND);
            int led = fourth - i;
            ledControl->leds[led] = color;
            ledControl->leds[fourth + i] = color;
            ledControl->leds[NUM_LEDS - led] = color;
            ledControl->leds[NUM_LEDS - (fourth + i)] = color;
        }
                
        return true;
    }

    bool exit(int param)
    {
    }
private:
    long lastShiftTime = 0;
    uint8_t shift = 0;
    int baseDistance = 300; // governs how drastically color changes with movement
    int stepDistance = 60; //governs how different each pixel is from the one before it.
};
#endif