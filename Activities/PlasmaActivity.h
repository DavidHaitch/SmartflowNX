#ifndef PLASMAACTIVITY_H
#define PLASMAACTIVITY_H
#include "LedActivity.h"

#define qsuba(x, b)  ((x>b)?x-b:0)                          // Analog Unsigned subtraction macro. if result <0, then => 0
#define STEP_DELAY 0
class PlasmaActivity : public LedActivity {
public:
    PlasmaActivity(MotionState* _motionState, LedControl* _ledControl) : LedActivity(_motionState, _ledControl)
    {
    }

    bool enter(int param)
    {
        ledControl->minBrightness = 0;
        ledControl->directMode = false;
    }

    bool update(bool realMode)
    {
        if(millis() - lastStepTime >= STEP_DELAY)
        {
            lastStepTime = millis();
            int thisPhase = beatsin8(240,-64,64);                           // Setting phase change for a couple of waves.
            int thatPhase = beatsin8(70,-64,64);
            for (int k=0; k<NUM_LEDS; k++) {                              // For each of the LED's in the strand, set a brightness based on a wave as follows:

                int colorIndex = cubicwave8((k*23)+thisPhase)/2 + cos8((k*15)+thatPhase)/2;           // Create a wave and add a phase change and add another wave with its own phase change.. Hey, you can even change the frequencies if you wish.
                int thisBright = qsuba(colorIndex, beatsin8(7,0,96));                                 // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..
                CRGB color = ColorFromPalette(PartyColors_p, colorIndex, thisBright, LINEARBLEND);  // Let's now add the foreground colour.
                ledControl->leds[k] = color;
            }
        }

        return true;
    }

    bool exit(int param)
    {
    }    
private:
    long lastStepTime;
};
#endif