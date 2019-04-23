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
        ledControl->minBrightness = ledControl->maxBrightness / 16;
        ledControl->directMode = false;
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
            offset+=32;
            for (int i = 0; i < NUM_LEDS; i++)
            {
                ledControl->leds[i] = CHSV(offset, 255, 255);
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
    const int shiftDelay = 20;
    long lastShift;
};
#endif