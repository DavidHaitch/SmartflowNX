#ifndef SIEZUREACTIVITY_H
#define SIEZUREACTIVITY_H
#include "LedActivity.h"

class SiezureActivity : public LedActivity {
public:
    SiezureActivity(MotionState* _motionState, LedControl* _ledControl) : LedActivity(_motionState, _ledControl)
    {
    }

    bool enter(int param)
    {
        ledControl->minBrightness = ledControl->maxBrightness / 2;
    }

    bool update(bool realMode)
    {
        if(realMode)
        {
            motionState->isEnabled = false;
        }

        if(count%2 == 0)
        {
            for (int i = 0; i < NUM_LEDS; i++)
            {
                ledControl->leds[i] = CRGB::Black;
            }
        }
        else
        {
            offset+=24;
            for (int i = 0; i < NUM_LEDS; i++)
            {
                ledControl->leds[i] = ColorFromPalette(RainbowColors_p, offset + (i*8), 255, LINEARBLEND);
            }

        }

        if(millis() - lastShift >= shiftDelay)
        {
            count++;
            lastShift = millis();
        }

        return true;
    }

    bool exit(int param)
    {
    }
private:
    int offset;
    int count;
    const int shiftDelay = 10;
    long lastShift;
};
#endif