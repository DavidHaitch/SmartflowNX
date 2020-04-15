#ifndef MARCHINGEFFECT_H
#define MARCHINGEFFECT_H
#include "LedEffect.h"

class MarchingEffect : public LedEffect
{
  public:
    MarchingEffect(MotionState *_motionState, LedControl *_ledControl) : LedEffect(_motionState, _ledControl)
    {
    }

    bool apply(int param)
    {
        if (millis() - lastUpdate >= updateDelay)
        {
            lastUpdate = millis();
            place++;
        }

        for (int i = 0; i < NUM_LEDS; i++)
        {
            if ((i + place) % (NUM_LEDS / 8) < 8)
            {
                ledControl->leds[i] = CRGB::Black;
            }
        }

        return true;
    }

  private:
    int place;
    long lastUpdate;
    const int updateDelay = 0;
};
#endif