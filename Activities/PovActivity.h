#ifndef POVACTIVITY_H
#define POVACTIVITY_H
#include "LedActivity.h"
#include "../PovData/FzbnData.h"

#define FPS 24000

class PovActivity : public LedActivity {
public:
    PovActivity(MotionState* _motionState, LedControl* _ledControl) : LedActivity(_motionState, _ledControl)
    {
        frameDelay = 1000000 / FPS;
        hueShiftDelay = 50;
    }

    bool enter(int param)
    {
        ledControl->minBrightness = ledControl->maxBrightness / 2;
        ledControl->addressingMode = Centered;
    }

    bool update(bool realMode)
    {
        if(realMode)
        {
            motionState->isEnabled = false;
        }

        if(micros() - lastFrameChange > frameDelay)
        {
            lastFrameChange = micros();
            frame++;
        }

        for (int i=0; i<NUM_LEDS; i++)
        {         
            int pixelIndex = i % (NUM_LEDS);
            int index = frame*(NUM_LEDS)*3 + pixelIndex*3;
            CRGB color = CRGB(data[index], data[index+1],data[index+2]);
            ledControl->leds[i] = color;
        }

        if (frame >= FRAMES)
        {
          frame = 0;
        } 

        return true;
    }

    bool exit(int param)
    {
    }
private:
    uint8_t hueShift;
    long lastHueShift;
    int hueShiftDelay;
    long lastFrameChange;
    uint8_t frame;
    uint8_t frameDelay;
};
#endif