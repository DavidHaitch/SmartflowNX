#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h> 
#include "MotionState.h"
#include "LedControl.h"
#include "Activities.h"
#include "Effects.h"

Adafruit_LSM9DS1 imu = Adafruit_LSM9DS1();

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
        &colormap,
        &firemap,
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

float getBatteryVolts()
{
    float measuredvbat = analogRead(A0);
    measuredvbat *= 2;    // we divided by 2, so multiply back
    measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
    measuredvbat /= 1024; // convert to voltage
    return measuredvbat;
}

void showBatteryVoltage()
{
    float vbat = getBatteryVolts();
    int mapped = map(vbat * 10.0, 33, 42, 0, NUM_LEDS/2);
    ledControl.Clear();
    CRGB c = CRGB::Green;
    if(vbat >= 3.6 && vbat <= 3.8)
    {
        c = CRGB::Blue;
    }

    if(vbat < 3.6)
    {
        c = CRGB::Red;
    }

    for(int i = 0; i < mapped; i++)
    {
        ledControl.leds[i] = c;
    }

    ledControl.Refresh();
    delay(1000);
}

void setup()
{
    Serial.begin(115200);

    while(!imu.begin())
    {
        Serial.println("Err");
    }

    imu.setupAccel(imu.LSM9DS1_ACCELRANGE_2G);

    imu.setupMag(imu.LSM9DS1_MAGGAIN_4GAUSS);
    imu.setupGyro(imu.LSM9DS1_GYROSCALE_500DPS);

    ledControl.maxBrightness = brightnesses[0];
    
    showBatteryVoltage();

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
    int motionLag = motionState.Update(&imu);
    long renderStart = millis();
    base->update(configured);

    if(!configured && millis() > 2000)
    {

        int c = config.configure(&motionState, &ledControl);

        if (c == 1)
        {
            //ledControl.maxBrightness = brightnesses[config.options[0] % BRIGHTNESS_SETTINGS];
            FastLED.setMaxPowerInVoltsAndMilliamps(3.7, powerLevels[config.options[0]%BRIGHTNESS_SETTINGS]);
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

    int renderLag = millis() - renderStart;

    int pushStart = millis();
    ledControl.Refresh();
    int pushLag = millis() - pushStart;

    Serial.print(motionLag);
    Serial.print("\t");
    Serial.print(renderLag);
    Serial.print("\t");
    Serial.print(pushLag);
    Serial.print("\t");
    Serial.println(millis() - start);
}