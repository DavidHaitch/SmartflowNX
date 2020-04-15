#ifndef GRAVITYEFFECT_H
#define GRAVITYEFFECT_H
#include "LedEffect.h"

class GravityEffect : public LedEffect {
public:
    GravityEffect(MotionState* _motionState, LedControl* _ledControl) : LedEffect(_motionState, _ledControl)
    {
    }

    bool apply(int param)
    {
        if(millis() - lastCalc > wait)
        {
            lastCalc = millis();
            float incline = -1.0 * (motionState->orientation.getPitch());
            float accel = gravity * sin(incline * (3.1415 / 180));
            speed += accel;
            dot += speed;
            if(dot < 0)
            {
                dot = 0;
                speed = 0;
            }
            if(dot > NUM_LEDS * 100)
            {
                dot = NUM_LEDS * 100;
                speed = 0;
            }
        }

        for (int i = 0; i < NUM_LEDS; i++)
        {
            float dist = abs((i * 100) - dot);
            if(dist > 512) ledControl->leds[i] = CRGB::Black;
            else ledControl->leds[i].fadeToBlackBy(dist / 2);
        }

        return true;
    }

private:
    float dot = 0;
    float gravity = 25.0;
    float speed = 0;
    int wait = 1000/60;
    long lastCalc = 0;
};
#endif