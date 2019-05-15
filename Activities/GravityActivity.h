#ifndef GRAVITYACTIVITY_H
#define GRAVITYACTIVITY_H
#include "LedActivity.h"

class GravityActivity : public LedActivity {
public:
    GravityActivity(MotionState* _motionState, LedControl* _ledControl) : LedActivity(_motionState, _ledControl)
    {
    }

    bool enter(int param)
    {
        ledControl->minBrightness = 0;
        ledControl->directMode = true;
    }

    bool update(bool realMode)
    {
        if(millis() - lastCalc > wait)
        {
            lastCalc = millis();
            //float incline = (motionState->orientation.getPitch());
            //float accel = gravity * sin(incline * (3.14159/180));
            float accel = motionState->rawAxialAccel * -1;
            
            // cA = angVel^2 * radius 
            float centripetalAccel = (motionState->angularVelocity * motionState->angularVelocity) * 0.673;

            if(centripetalAccel > abs(accel))
            {
                accel = centripetalAccel;
                if(dot < (TRUE_LEDS / 2) * 50)
                {
                    accel *= -1;
                }
            }

            accel *= 1.5;

            speed += accel;
            dot += speed;
            if(dot < 0)
            {
                dot = 10;
                hue += abs(speed) / 32;
                speed = 0;
            }

            if(dot > (TRUE_LEDS / 2) * 100)
            {
                dot = ((TRUE_LEDS / 2) * 100) - 10;
                hue += abs(speed) / 32;
                speed = 0;
            }

            //if(abs(incline) < stickiness) speed = 0;
        }

        float antiDot = ((TRUE_LEDS / 2) * 100) - dot;
        for (int i = 0; i < TRUE_LEDS / 2; i++)
        {
            float dist = abs((i * 100) - dot);
            float antiDist = abs((i * 100) - antiDot);
            if(dist < 1024)
            {
                ledControl->leds[i] += CHSV(hue, 200, 64);
                ledControl->leds[i].fadeToBlackBy(dist / 4);
            }

            if(antiDist < 1024)
            {
                ledControl->leds[i] += CHSV(hue + 128, 200, 64);
                ledControl->leds[i].fadeToBlackBy(antiDist / 4);
            }
            
            if(dist > 1024 && antiDist > 1024)
            {
                ledControl->leds[i] = CRGB::Black;
            }
        }
         
        return true;
    }

    bool exit(int param)
    {
    }
private:
    int hue = 0;
    float dot = 0;
    float stickiness = 2.5;
    float gravity = 30.0;
    float speed = 0;
    int wait = 1000/60;
    long lastCalc = 0;
};
#endif