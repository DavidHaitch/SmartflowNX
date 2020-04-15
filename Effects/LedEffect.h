#ifndef LEDEFFECT_H
#define LEDEFFECT_H
#include "../MotionState.h"
#include "../LedControl.h"

class LedEffect {
public:
    LedEffect(MotionState* _motionState, LedControl* _ledControl)
    {
        motionState = _motionState;
        ledControl = _ledControl;
    }
    
    virtual bool apply(int param) = 0;
    ~LedEffect()
    {

    }
protected:
    MotionState* motionState;
    LedControl* ledControl;
};
#endif