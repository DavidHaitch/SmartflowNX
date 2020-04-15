#ifndef FIRESWINGEFFECT_H
#define FIRESWINGEFFECT_H
#include "LedEffect.h"

class FireswingEffect : public LedEffect {
public:
    FireswingEffect(MotionState* _motionState, LedControl* _ledControl) : LedEffect(_motionState, _ledControl)
    {
    }

    bool apply(int param)
    {
        int target = 40;
        int range = abs(255 - target) > abs(0 - target) ? abs(255 - target) : abs(0 - target);
        int difference = abs(motionState->relativeAngularVelocity - target);
        
        ledControl->brightness = map(range - difference, 0, range, ledControl->minBrightness, ledControl->maxBrightness);
        //ledControl->brightness = 32;
        return false;
    }
};
#endif