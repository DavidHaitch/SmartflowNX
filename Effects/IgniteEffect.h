#ifndef IGNITEEFFECT_H
#define IGNITEEFFECT_H
#include "LedEffect.h"

class IgniteEffect : public LedEffect {
public:
    IgniteEffect(MotionState* _motionState, LedControl* _ledControl) : LedEffect(_motionState, _ledControl)
    {
    }

    bool apply(int param)
    {
        if (ignitedLeds <= NUM_LEDS / 2)
        {
            if(millis() - lastChangeTime >= 10)
            {
                ignitedLeds++;
                lastChangeTime = millis();
            }

            for (int i = 0; i < NUM_LEDS; i++)
            {
                if (i < ignitedLeds || NUM_LEDS - i < ignitedLeds)
                {
                    continue;
                }
                else
                {
                    ledControl->leds[i] = CRGB::Black;
                }
            }

            return true;
        }       
        else
        {
            return false;
        }
    }
private:
    long lastChangeTime;
    uint8_t ignitedLeds;
};
#endif