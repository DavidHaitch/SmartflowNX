#ifndef SPARKLEEFFECT_H
#define SPARKLEEFFECT_H
#include "LedEffect.h"

class SparkleEffect : public LedEffect {
public:
    SparkleEffect(MotionState* _motionState, LedControl* _ledControl) : LedEffect(_motionState, _ledControl)
    {
    }

    bool apply(int param)
    {
        if(millis() - lastSparkle > sparkleTime)
        {
            lastSparkle = millis();
            for(int i = 0; i < NUM_LEDS / 2; i++)
            {
                int sparklechance = motionState->relativeAngularVelocity;
                if (sparklechance > 192) sparklechance = 192;
                if(rand()%255 < sparklechance)
                {
                    ledControl->leds[i] = CRGB::White;
                    ledControl->leds[NUM_LEDS - i] = CRGB::White;
                }
            }
        }

        return false;
    }

private:
    int sparkleTime = 30;
    long lastSparkle = 0;
};
#endif