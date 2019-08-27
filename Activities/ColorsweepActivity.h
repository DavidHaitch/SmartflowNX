#ifndef COLORSWEEPACTIVITY_H
#define COLORSWEEPACTIVITY_H
#include "LedActivity.h"

#define SWEEP_DELAY 200
class ColorsweepActivity : public LedActivity {
public:
    ColorsweepActivity(MotionState* _motionState, LedControl* _ledControl, CRGBPalette16 _palette) : LedActivity(_motionState, _ledControl)
    {
        palette = _palette;
    }

    bool enter(int param)
    {
        ledControl->minBrightness = 0;
        ledControl->directMode = false;
    }

    bool update(bool realMode)
    {
        if(realMode)
        {
            //motionState->isEnabled = false;
        }

        if(micros() - lastShiftTime >= SWEEP_DELAY)
        {
            lastShiftTime = micros();
            coord += 1;
            offset++;
        }

        for (int i = 0; i < NUM_LEDS; i++)
        {
            float r = baseDistance + (stepDistance * (i + 1));
            int color = inoise8(coord, r);
            // color = qsub8(color, 16);
            // color = qadd8(color, scale8(color, 39));
            ledControl->leds[i] = ColorFromPalette( palette, color + offset, 255, LINEARBLEND);
        }
         
        return true;
    }

    bool exit(int param)
    {
    }
private:
    long coord;
    int offset;
    long lastShiftTime;
    int baseDistance = 200; // governs how drastically color changes with movement
    int stepDistance = 500; //governs how different each pixel is from the one before it.
    CRGBPalette16 palette;
};
#endif