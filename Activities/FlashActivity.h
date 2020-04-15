#ifndef FLASHACTIVITY_H
#define FLASHACTIVITY_H
#include "LedActivity.h"

class FlashActivity : public LedActivity
{
public:
    FlashActivity(MotionState *_motionState, LedControl *_ledControl) : LedActivity(_motionState, _ledControl)
    {
    }

    bool enter(int param)
    {
        ledControl->minBrightness = 0;
        ledControl->addressingMode = Direct;
    }

    bool update(bool realMode)
    {
        long now = millis();
        if(now - lastFlash > flashInterval)
        {
            flashing = true;
            lastFlash = now;
        }

        if(flashing == true && now - lastFlash > flashDuration)
        {
            flashing = false;
        } 

        CRGB color = CRGB::Black;
        if(flashing) color = CRGB::White;
        for(int i = 0; i < TRUE_LEDS; i++)
        {
            ledControl->leds[i] = color;
        }

        return true;
    }

    bool exit(int param)
    {
        return true;
    }

private:
    bool flashing = false;
    long lastFlash = 0;
    int flashDuration = 100;
    int flashInterval = 2000;
};
#endif