#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H
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

        if(smackCount > 4)
        {
            smackCount = 0;
        }

        for(int i = 1; i < smackCount + 1; i++)
        {
            ledControl->leds[i] = CRGB::White;
        }

        if(motionState->jerk > 1.75 && now - lastSmackTime > 250)
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