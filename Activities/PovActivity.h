#ifndef POVACTIVITY_H
#define POVACTIVITY_H
#include "LedActivity.h"
#include "../PovData/DualHeartsData.h"

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
    }

    bool update(int param)
    {
        if(micros() - lastFrameChange > frameDelay)
        {
            lastFrameChange = micros();
            frame++;
        }

        for (int i=0; i<NUM_LEDS / 2; i++)
        {         
            int pixelIndex = i % (NUM_LEDS/2);
            int index = frame*(NUM_LEDS/2)*3 + pixelIndex*3;
            CRGB color = CRGB(data[index], data[index+1],data[index+2]);
            ledControl->leds[i] = color;
            ledControl->leds[NUM_LEDS - i] = color;
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