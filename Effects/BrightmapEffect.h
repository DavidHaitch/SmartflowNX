#ifndef BRIGHTMAPEFFECT_H
#define BRIGHTMAPEFFECT_H
#include "LedEffect.h"

class BrightmapEffect : public LedEffect {
public:
    BrightmapEffect(MotionState* _motionState, LedControl* _ledControl) : LedEffect(_motionState, _ledControl)
    {
    }

    bool apply(int param)
    {
        int fourth = NUM_LEDS / 4;  
        for (int i = 0; i <= fourth; i++)
        {
            float r = baseDistance + (stepDistance * (i + 1));
            int b = inoise8(abs(motionState->pointingX) * r, abs(motionState->pointingY) * r, abs(motionState->pointingZ) * r);
            if (b > 255) b = 255;
            int led = fourth - i;
            ledControl->leds[led].fadeToBlackBy(b);
            ledControl->leds[fourth + i].fadeToBlackBy(b);
            ledControl->leds[NUM_LEDS - led].fadeToBlackBy(b);
            ledControl->leds[NUM_LEDS - (fourth + i)].fadeToBlackBy(b);
        }

        return true;
    }
private:
    int baseDistance = 150; // governs how drastically color changes with movement
    int stepDistance = 45; //governs how different each pixel is from the one before it.
};
#endif