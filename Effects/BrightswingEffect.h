#ifndef BRIGHTSWINGEFFECT_H
#define BRIGHTSWINGEFFECT_H
#include "LedEffect.h"

class BrightswingEffect : public LedEffect {
public:
    BrightswingEffect(MotionState* _motionState, LedControl* _ledControl) : LedEffect(_motionState, _ledControl)
    {
    }

    bool apply(int param)
    {
        ledControl->brightness = map(motionState->relativeAngularVelocity, 0, 255, 0, ledControl->maxBrightness);
        return false;
    }
};
#endif