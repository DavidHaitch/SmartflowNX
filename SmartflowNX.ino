#include "MPU9250.h"
MPU9250 imu(Wire, 0x68);

#include "MotionState.h"
#include "LedControl.h"
#include "Activities.h"
#include "Effects.h"

MotionState motionState;
LedControl ledControl;

IgniteEffect ignite(&motionState, &ledControl);
BrightswingEffect brightswing(&motionState, &ledControl);
FireswingEffect fireswing(&motionState, &ledControl);
BrightmapEffect brightmap(&motionState, &ledControl);
SparkleEffect sparkle(&motionState, &ledControl);
NoopEffect noop(&motionState, &ledControl);
MarchingEffect marching(&motionState, &ledControl);

DEFINE_GRADIENT_PALETTE(pfoenix_p){
    0, 0, 0, 0,
    200, 96, 0, 255,
    255, 0, 0, 0};

ColormapActivity colormap(&motionState, &ledControl, RainbowColors_p, 200, 32);
ColormapActivity colormap_frantic(&motionState, &ledControl, RainbowColors_p, 6000, 28);
ColorswingActivity colorswing(&motionState, &ledControl);
FiremapActivity firemap(&motionState, &ledControl);
GravityActivity gravity(&motionState, &ledControl);
ColorsweepActivity colorsweep(&motionState, &ledControl, RainbowColors_p);
ColorsweepActivity pfoenix(&motionState, &ledControl, pfoenix_p);

PovActivity pov(&motionState, &ledControl);
SiezureActivity zap(&motionState, &ledControl);
PlasmaActivity plasma(&motionState, &ledControl);

#define NUM_BASE_ACTIVITIES 7
LedActivity *baseActivities[NUM_BASE_ACTIVITIES] =
    {
        &firemap,
        &colormap,
        &gravity,
        &colorsweep,
        &zap,
        &plasma,
        &colorswing};

LedEffect *effects[NUM_BASE_ACTIVITIES] =
    {
        &noop,
        &brightmap,
        &noop,
        &marching,
        &noop,
        &brightswing,
        &brightmap};
#define BRIGHTNESS_SETTINGS 3
int brightnesses[BRIGHTNESS_SETTINGS] = {32, 64, 192};
int powerLevels[BRIGHTNESS_SETTINGS] = {150, 300, 1000};

LedActivity *base;
LedEffect *effect;
ConfigManager config;

long lastDebugPrint = 0;
bool effectEnable = false;
bool isIgniting = true;
void setup()
{
    Serial.begin(9600);
    for (int i = 0; i < TRUE_LEDS; i++)
    {
        ledControl.leds[i] = CRGB::Red;
    }

    ledControl.Refresh();
    imu.begin();
    imu.setAccelRange(MPU9250::ACCEL_RANGE_4G);
    imu.setGyroRange(MPU9250::GYRO_RANGE_2000DPS);
    ledControl.maxBrightness = brightnesses[0];

    //FastLED.setMaxPowerInVoltsAndMilliamps(3.7, powerLevels[0]);
    base = baseActivities[0];
    base->enter(0);
    effect = effects[0];
}

LedActivity *transitionActivity(LedActivity *from, LedActivity *to)
{
    from->exit(0);
    to->enter(0);
    return to;
}

bool configured = false;
void loop()
{
    long start = millis();

    motionState.Update(&imu);
    base->update(configured);

    if (!configured && millis() > 5000)
    {

        int c = config.configure(&motionState, &ledControl);

        if (c == 1)
        {
            ledControl.maxBrightness = brightnesses[config.options[0] % BRIGHTNESS_SETTINGS];
            //FastLED.setMaxPowerInVoltsAndMilliamps(3.7, powerLevels[config.options[0]%BRIGHTNESS_SETTINGS]);
            base = transitionActivity(base, base); //Reinit base activity with new settings.
        }

        if (c == 2)
        {
            uint8_t mode = config.options[1] % NUM_BASE_ACTIVITIES;
            ledControl.Clear();
            base = transitionActivity(base, baseActivities[mode]);
            effect = effects[mode];
        }

        if (c >= 3)
        {
            effectEnable = true;
            configured = true;
        }
    }

    if (effectEnable && configured)
    {
        effect->apply(0);
    }

    if (isIgniting)
    {
        isIgniting = ignite.apply(0);
    }

    ledControl.Refresh();
}