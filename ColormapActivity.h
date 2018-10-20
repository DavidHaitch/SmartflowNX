#ifndef COLORMAPACTIVITY_H
#define COLORMAPACTIVITY_H
#include "LedActivity.h"

class ColormapActivity : public LedActivity {
public:
    ColormapActivity(MotionState* _motionState, LedControl* _ledControl) : LedActivity(_motionState, _ledControl)
    {
        palette = RainbowColors_p;
    }

    CRGBPalette16 palette;
    bool enter(int param)
    {
        ledControl->minBrightness = 0;
    }

    bool update(int param)
    {
        if(millis() - lastShiftDecay > 20)
        {
            //shift += param / 5;
            if(shift > 0) shift--;
        }

        shift = map(motionState->orientation.getYawRadians() * 100,-314, 314, 0, 255 );

        int fourth = NUM_LEDS / 4;  
        for (int i = 0; i <= fourth; i++)
        {
            float r = baseDistance + (stepDistance * (i + 1));
            int c = inoise8(motionState->pointingX * r, motionState->pointingY * r, motionState->pointingZ * r);
            CRGB color = ColorFromPalette( palette, c + shift, 255, LINEARBLEND);
            //color = qsub8(color, 16);
            //color = qadd8(color, scale8(color, 39));
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
    long lastShiftDecay = 0;
    uint8_t shift = 0;
    int baseDistance = 300; // governs how drastically color changes with movement
    int stepDistance = 20; //governs how different each pixel is from the one before it.
};
#endif