#ifndef COLORMAPACTIVITY_H
#define COLORMAPACTIVITY_H
#include "LedActivity.h"

class ColormapActivity : public LedActivity {
public:
    ColormapActivity(MotionState* _motionState, LedControl* _ledControl, CRGBPalette16* _palette, int _baseDistance, int _stepDistance) : LedActivity(_motionState, _ledControl)
    {
        palette = _palette;
        baseDistance = _baseDistance;
        stepDistance = _stepDistance;
    }

    bool enter(int param)
    {
        ledControl->minBrightness = 4;
        ledControl->addressingMode = Centered;
    }

    bool update(bool realMode)
    {
        if(millis() - lastShiftDecay > 200)
        {
            lastShiftDecay = millis();
            if(shift > 0) shift--;
        }
        
        int angVel = (motionState->angularVelocity * (180 / 3.14159));
        shift = motionState->orientation.getPitch();

        for (int i = 0; i < NUM_LEDS; i++)
        {
            float r = baseDistance + (stepDistance * i);
            int c = inoise8(motionState->pointingX * r, motionState->pointingY * r, motionState->pointingZ * r);
            //int c2 = inoise8(secondSampleOffset + motionState->pointingX * r,secondSampleOffset + motionState->pointingY * r, secondSampleOffset + motionState->pointingZ * r);
            CRGB color = ColorFromPalette(*palette, c + shift + (i*2), 255, LINEARBLEND);
            ledControl->leds[i] = color;
        }
                
        return true;
    }

    bool exit(int param)
    {
    }

private:
    long lastShiftDecay = 0;
    uint8_t shift = 0;
    int baseDistance = 1; // governs how drastically color changes with movement
    int stepDistance = 1; //governs how different each pixel is from the one before it.
    CRGBPalette16* palette;
    CRGBPalette16 warmPalette;
    CRGBPalette16 coolPalette;
    int secondSampleOffset = 10000;
};
#endif