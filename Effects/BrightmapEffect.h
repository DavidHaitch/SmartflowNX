#ifndef BRIGHTMAPEFFECT_H
#define BRIGHTMAPEFFECT_H
#include "LedEffect.h"

class BrightmapEffect : public LedEffect
{
  public:
    BrightmapEffect(MotionState *_motionState, LedControl *_ledControl) : LedEffect(_motionState, _ledControl)
    {
    }

    bool apply(int param)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            float r = baseDistance + (stepDistance * (i + 1));
            int b = inoise8(abs(motionState->pointingX) * r, abs(motionState->pointingY) * r, abs(motionState->pointingZ) * r);
            if (b > 255)
                b = 255;
            ledControl->leds[i].fadeLightBy(cubicwave8(b));
        }

        return true;
    }

  private:
    int baseDistance = 100; // governs how drastically color changes with movement
    int stepDistance = 70; //governs how different each pixel is from the one before it.
};
#endif