#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H
#include "MotionState.h"
class ConfigManager
{
public:
    uint8_t options[3]; //Three dimensions of configuration.
    ConfigManager()
    {
        smackCount = 0;
        lastSmackTime = 0;
    } 

    int configure(MotionState* motionState, LedControl* ledControl)
    {
        long now = millis();

        if(smackCount > 3)
        {
            smackCount = 0;
        }

        for(int i = 1; i < smackCount + 1; i++)
        {
            ledControl->leds[5 + i] = CRGB::White;
        }

        if(motionState->jerk > 2.75 && now - lastSmackTime > 250)
        {
            lastSmackTime = now;
            smackCount++;
        }

        if(smackCount > 0 && now - lastSmackTime > 2000)
        {
            options[smackCount - 1]++;
            int ret = smackCount;
            smackCount = 0;
            return ret;
        }
        
        return 0;
    }
private:
    int lastSmackTime;
    int smackCount;
};
#endif