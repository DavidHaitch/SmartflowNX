#ifndef FIREMAPACTIVITY_H
#define FIREMAPACTIVITY_H
#include "LedActivity.h"

class FiremapActivity : public LedActivity
{
  public:
    FiremapActivity(MotionState *_motionState, LedControl *_ledControl) : LedActivity(_motionState, _ledControl)
    {
        palette = CRGBPalette16(
            0x000000, 0x330000, 0x661100, 0x991100,
            0xCC2200, 0xFF3300, 0xFF4400, 0xFF6600,
            0xFF9900, 0xFFCC00, 0xFFFF00, 0xFFFF33,
            0xFFFF33, 0x6633FF, 0xFFFFCC, 0xFFFFFF);
    }

    bool enter(int param)
    {
        ledControl->minBrightness = 4;
        ledControl->directMode = true;
    }

    bool update(bool realMode)
    {
        long now = millis();
        if (now - lastFireTick > 10)
        {
            lastFireTick = now;
            int angVel = (motionState->angularVelocity * (180 / 3.14159));
            int spinout = 810;
            for (int i = 0; i < TRUE_LEDS / 2; i++)
            {
                int coolingFactor = map(angVel, 0, spinout, 1, 4);
                int temperature = map(angVel, 0, spinout, 96, 192);
                int heatChance = map(angVel, 0, spinout, 2, 8);
                if (angVel > spinout)
                {
                    temperature /= 2;
                    heatChance /= 8;
                    //coolingFactor *= 2;
                }

                if (heat[i] > coolingFactor)
                {
                    heat[i] -= coolingFactor;
                }
                else if (angVel > spinout)
                {
                    heat[i] = (-1 * rand() % 1024) - 1024;
                }
                else if (heat[i] > 0)
                {
                    heat[i] = 0;
                }

                if (rand() % 255 < heatChance && heat[i] < temperature)
                {
                    heat[i] += temperature;
                }
            }

            for (int i = 0; i < (TRUE_LEDS / 2); i++)
            {
                int removed = abs(heat[i]) / 16;
                int originalHeat = heat[i];

                if (i - 1 >= 0 && heat[i - 1] < originalHeat)
                {
                    heat[i] -= removed / 2;
                    heat[i - 1] += removed / 2;
                }
                if (i + 1 <= (TRUE_LEDS / 2) && heat[i + 1] < originalHeat)
                {
                    heat[i] -= removed / 2;
                    heat[i + 1] += removed / 2;
                }
            }
        }

        if (now - lastFireRise > 40)
        {
            lastFireRise = now;

            if (motionState->rawAxialAccel > 0)
            {
                for (int i = TRUE_LEDS / 2; i > 0; i--)
                {
                    if (heat[i] >= 0 && heat[i - 1] >= 0)
                    {
                        heat[i] += heat[i - 1] / 2;
                        heat[i - 1] -= heat[i - 1] / 2;
                    }
                }
            }
            else
            {
                for (int i = 0; i < (TRUE_LEDS / 2) - 1; i++)
                {
                    if (heat[i] >= 0 && heat[i + 1] >= 0)
                    {
                        heat[i] += heat[i + 1] / 2;
                        heat[i + 1] -= heat[i + 1] / 2;
                    }
                }

                heat[0] = 0;
            }
        }

        for (int i = 0; i < TRUE_LEDS / 2; i++)
        {
            if (heat[i] >= 0)
            {
                ledControl->leds[i] = ColorFromPalette(palette, heat[i], 255, LINEARBLEND);
            }
            else
            {
                int b = (heat[i] * -1) / 64;
                ledControl->leds[i] = CRGB(b/2,b/2,b);
            }
        }

        blur1d(ledControl->leds, TRUE_LEDS/2, 64);
        blur1d(ledControl->leds, TRUE_LEDS, 92);
        return true;
    }

    bool exit(int param)
    {
    }

  private:
    int heat[TRUE_LEDS];
    long lastFireTick = 0;
    long lastFireRise = 0;
    CRGBPalette16 palette;
};
#endif