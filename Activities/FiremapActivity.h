#ifndef FIREMAPACTIVITY_H
#define FIREMAPACTIVITY_H
#include "LedActivity.h"

class FiremapActivity : public LedActivity {
public:
    FiremapActivity(MotionState* _motionState, LedControl* _ledControl) : LedActivity(_motionState, _ledControl)
    {
        palette = CRGBPalette16(
            0x000000, 0x330000, 0x661100, 0x992200,
            0xCC3300, 0xFF4400, 0xFF5500, 0xFF6600,
            0xFF9900, 0xFFCC00, 0xFFFF00, 0xFFFF33,
            0xFFFF66, 0xFFFF99, 0xFFFFCC, 0xFFFFFF
        );
    }

    bool enter(int param)
    {
        ledControl->minBrightness = 4;
        ledControl->directMode = true;
    }

    bool update(bool realMode)
    {
        long now = millis();
        if(now - lastFireTick > 10)
        {
            lastFireTick = now;
            for(int i = 0; i < NUM_LEDS/2; i++)
            {
                int coolingFactor = map(motionState->relativeAngularVelocity, 0, 255, 4, 8);
                if(heat[i] >= coolingFactor) heat[i]-= coolingFactor;
                
                int temperature = map(motionState->relativeAngularVelocity, 0, 255, 64, 172);
                int heatChance = map(motionState->relativeAngularVelocity, 0, 255, 12, 1);
                if(rand()%255 < heatChance)
                {
                    heat[i] = temperature;
                }
            }
        }

        if(now - lastFireRise > 100)
        {
            lastFireRise = now;
            if(motionState->orientation.getPitch() < 0)
            {
                for(int i = NUM_LEDS/2; i > 0; i--)
                {
                    heat[i] = heat[i - 1];
                }
            }
            else
            {
                for(int i = 0; i < (NUM_LEDS/2) - 1; i++)
                {
                   if(rand()%16 < 15) heat[i] = heat[i + 1];
                }
            }
        }

        for (int i = 0; i < NUM_LEDS/2; i++)
        {
            ledControl->leds[i] = ColorFromPalette( palette, heat[i], 255, LINEARBLEND);;
        }

        blur1d(ledControl->leds, NUM_LEDS/2, 32);
        blur1d(ledControl->leds, NUM_LEDS/2, 32);
        return true;
    }

    bool exit(int param)
    {
    }
private:
    int heat[NUM_LEDS];
    long lastFireTick = 0;
    long lastFireRise = 0;
    CRGBPalette16 palette;
};
#endif