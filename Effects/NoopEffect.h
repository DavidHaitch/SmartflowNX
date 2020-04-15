#ifndef NOOPEFFECT_H
#define NOOPEFFECT_H
#include "LedEffect.h"

class NoopEffect : public LedEffect {
public:
    NoopEffect(MotionState* _motionState, LedControl* _ledControl) : LedEffect(_motionState, _ledControl)
    {
    }

    bool apply(int param)
    {
        return true;
    }
};
#endif