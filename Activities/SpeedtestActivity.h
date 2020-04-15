#ifndef SPEEDTESTACTIVITY_H
#define SPEEDTESTACTIVITY_H
#include "LedActivity.h"

#define SWEEP_DELAY 5000
class SpeedTestActivity : public LedActivity {
public:
    SpeedTestActivity(MotionState* _motionState, LedControl* _ledControl) : LedActivity(_motionState, _ledControl)
    {
    }

    bool enter(int param)
    {
        ledControl->minBrightness = 0;
        ledControl->addressingMode = Centered;
    }

    bool update(bool realMode)
    {
        step++;
        step %= NUM_LEDS/2;
        for (int i = 0; i < NUM_LEDS/2; i++)
        {
             if(i == step)  ledControl->leds[i] = CRGB::Red;
            else  ledControl->leds[i] = CRGB::Black;
        }
         
        return true;
    }

    bool exit(int param)
    {
    }
private:
    int step;
};
#endif