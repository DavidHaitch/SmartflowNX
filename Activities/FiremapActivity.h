#ifndef FIREMAPACTIVITY_H
#define FIREMAPACTIVITY_H
#include "LedActivity.h"

class FiremapActivity : public LedActivity {
public:
    FiremapActivity(MotionState* _motionState, LedControl* _ledControl) : LedActivity(_motionState, _ledControl)
    {
        palette = CRGBPalette16(CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
                                CRGB::Black, CRGB::OrangeRed, CRGB::Orange, CRGB::OrangeRed, CRGB::Orange, CRGB::OrangeRed, CRGB::Black, CRGB::Black);
        // palette = CRGBPalette16(CRGB::DarkRed, CRGB::Red, CRGB::DarkRed, CRGB::OrangeRed, CRGB::DarkOrange, CRGB::Orange, CRGB::Yellow, CRGB::Yellow,
        //                         CRGB::Yellow, CRGB::Yellow, CRGB::Orange, CRGB::DarkOrange, CRGB::OrangeRed, CRGB::DarkRed, CRGB::Red, CRGB::DarkRed);
    }

    CRGBPalette16 palette;

    bool enter(int param)
    {
        ledControl->minBrightness = 4;
        ledControl->directMode = false;
    }

    bool update(bool realMode)
    {
        if(millis() - lastShiftTime > 10)
        {
            lastShiftTime = millis();
            //shift += 1;
            coord += map(motionState->angularVelocityPercent, 0, 100, 1, 30);
        }

        for (int i = 0; i < NUM_LEDS; i++)
        {
            float r = baseDistance + (stepDistance * (i + 1));
            //int c = inoise8(abs(motionState->pointingX) * r, abs(motionState->pointingY) * r, abs(motionState->pointingZ) * r);            
            int c = inoise8(coord, r);            
            CRGB color = ColorFromPalette( palette, c + shift, 255, LINEARBLEND);
            ledControl->leds[i] = color;
        }
                
        return true;
    }

    bool exit(int param)
    {
    }
private:
    long lastShiftTime = 0;
    uint8_t shift = 0;
    int coord;
    int baseDistance = 300; // governs how drastically color changes with movement
    int stepDistance = 120; //governs how different each pixel is from the one before it.
};
#endif