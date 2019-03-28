#ifndef COLORMAPACTIVITY_H
#define COLORMAPACTIVITY_H
#include "LedActivity.h"

class ColormapActivity : public LedActivity {
public:
    ColormapActivity(MotionState* _motionState, LedControl* _ledControl, CRGBPalette16 _palette, int _baseDistance, int _stepDistance) : LedActivity(_motionState, _ledControl)
    {
        palette = _palette;
        baseDistance = _baseDistance;
        stepDistance = _stepDistance;
    }

    bool enter(int param)
    {
        ledControl->minBrightness = 4;
    }

    bool update(bool realMode)
    {
        if(millis() - lastShiftDecay > 200)
        {
            lastShiftDecay = millis();
            if(shift > 0) shift--;
        }
        
        shift = motionState->angularVelocityPercent;

        for (int i = 0; i <= NUM_LEDS; i++)
        {
            float r = baseDistance + (stepDistance * (i + 1));
            int c = inoise8(abs(motionState->pointingX) * r, abs(motionState->pointingY) * r, abs(motionState->pointingZ) * r);
            CRGB color = ColorFromPalette( palette, c + shift, 255, LINEARBLEND);
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
    int baseDistance = 600; // governs how drastically color changes with movement
    int stepDistance = 30; //governs how different each pixel is from the one before it.
    CRGBPalette16 palette;
};
#endif