#include <Wire.h>
#include <SparkFunMPU9250-DMP.h>
#include "MotionState.h"
#include "LedControl.h"
#include "Activities.h"
#include "Effects.h"

MPU9250_DMP imu;
MotionState motionState;
LedControl ledControl;

IgniteEffect ignite(&motionState, &ledControl);
BrightswingEffect brightswing(&motionState, &ledControl);
BrightmapEffect brightmap(&motionState, &ledControl);
SparkleEffect sparkle(&motionState, &ledControl);
NoopEffect noop(&motionState, &ledControl);

DEFINE_GRADIENT_PALETTE( pfoenix_p ) {
  0, 0, 0, 0,
200, 96, 0, 255,
255, 0, 0, 0 };

ColormapActivity colormap(&motionState, &ledControl, 600, 28);
ColormapActivity colormap_frantic(&motionState, &ledControl, 6000, 28);
ColorswingActivity colorswing(&motionState, &ledControl);
FiremapActivity firemap(&motionState, &ledControl);
ColorsweepActivity colorsweep(&motionState, &ledControl, RainbowColors_p);
ColorsweepActivity pfoenix(&motionState, &ledControl, pfoenix_p);

PovActivity pov(&motionState, &ledControl);
SiezureActivity zap(&motionState, &ledControl);
PlasmaActivity plasma(&motionState, &ledControl);

#define NUM_BASE_ACTIVITIES 10
LedActivity* baseActivities[NUM_BASE_ACTIVITIES] =
    {
        &pfoenix, 
        &colormap,
        &colormap_frantic,
        &colorswing,
        &firemap,
        &colorsweep,
        &pov,
        &zap,
        &plasma
    };

LedEffect* effects[NUM_BASE_ACTIVITIES] = 
    { 
        &noop,
        &brightswing,
        &brightswing,
        &brightmap,
        &brightswing,
        &sparkle,
        &brightswing,
        &brightswing,
        &brightswing
    };
#define BRIGHTNESS_SETTINGS 3
int brightnesses[BRIGHTNESS_SETTINGS] = { 64, 32, 255 };
int powerLevels[BRIGHTNESS_SETTINGS] = { 150, 300, 1000 };

LedActivity* base;
LedEffect* effect;
ConfigManager config;

long lastDebugPrint = 0;
bool effectEnable = false;
bool isIgniting = true;
void setup()
{
    if (imu.begin() != INV_SUCCESS)
    {
        while (1)
        {
            delay(5000);
        }
    }

    imu.setGyroFSR(1000);
    imu.setAccelFSR(4); 
    //imu.setLPF(98); 
    imu.setSampleRate(1000);
    imu.setCompassSampleRate(100);
    imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
    ledControl.maxBrightness = 255;
    FastLED.setMaxPowerInVoltsAndMilliamps(3.7, powerLevels[0]);
    base = baseActivities[0];
    effect = effects[0];
}

LedActivity* transitionActivity(LedActivity* from, LedActivity* to)
{
    from->exit(0);
    to->enter(0);
    return to;
}

bool configured = false;
void loop()
{
    if(millis() < 250) return; // Allow sensors to settle before advancing.
    long start = millis();
    motionState.Update(&imu);

    base->update(0);

    if(!configured)
    {
        int c = config.configure(&motionState, &ledControl);

        if(c == 1)
        {
            //ledControl.maxBrightness = brightnesses[config.options[0]%BRIGHTNESS_SETTINGS];
            FastLED.setMaxPowerInVoltsAndMilliamps(3.7, powerLevels[config.options[0]%BRIGHTNESS_SETTINGS]);
            base = transitionActivity(base, base); //Reinit base activity with new settings.
        }

        if(c == 2)
        {
            uint8_t mode = config.options[1]%NUM_BASE_ACTIVITIES;
            base = transitionActivity(base, baseActivities[mode]);
            effect = effects[mode];
        }

        if(c >= 3)
        {          
            effectEnable = true;
            configured = true;
        }
    }

    if(effectEnable && configured)
    {
        effect->apply(0);
    }

    if(isIgniting)
    {
        isIgniting = ignite.apply(0);
    }

    ledControl.Refresh();
}