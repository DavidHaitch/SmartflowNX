#ifndef PLASMAACTIVITY_H
#define PLASMAACTIVITY_H
#include "LedActivity.h"

// Derived from https://github.com/johncarl81/neopixelplasma/blob/master/neopixelplasma.ino

class PlasmaActivity : public LedActivity
{
public:
    PlasmaActivity(MotionState *_motionState, LedControl *_ledControl) : LedActivity(_motionState, _ledControl)
    {
    }

    bool enter(int param)
    {
        ledControl->minBrightness = 0;
        ledControl->addressingMode = Centered;
    }

    bool update(bool realMode)
    {
        Point p1 = {(sin(phase * 1.000) + 1.0) * scale, (sin(phase * 1.310) + 1.0) * scale, (sin(phase * 1.290) + 1.0) * scale};
        Point p2 = {(sin(phase * 1.770) + 1.0) * scale, (sin(phase * 2.865) + 1.0) * scale, (sin(phase * 2.597) + 1.0) * scale};
        Point p3 = {(sin(phase * 0.250) + 1.0) * scale, (sin(phase * 0.750) + 1.0) * scale, (sin(phase * 1.866) + 1.0) * scale};
        for (int i = 0; i < NUM_LEDS; i++)
        {
            float r = baseDistance + (stepDistance * (float)i);
            Point p = {motionState->pointingX * r, motionState->pointingY * r, motionState->pointingZ * r};
            //Point p = {r,r,r};

            // Calculate the distance between this LED, and p1.
            Point dist1 = {p.x - p1.x, p.y - p1.y, p.z - p1.z}; // The vector from p1 to this LED.
            float distance1 = sqrt(dist1.x * dist1.x + dist1.y * dist1.y + dist1.z * dist1.z);

            // Calculate the distance between this LED, and p2.
            Point dist2 = {p.x - p2.x, p.y - p2.y, p.z - p2.z}; // The vector from p2 to this LED.
            float distance2 = sqrt(dist2.x * dist2.x + dist2.y * dist2.y + dist2.z * dist2.z);

            // Calculate the distance between this LED, and p3.
            Point dist3 = {p.x - p3.x, p.y - p3.y, p.z - p3.z}; // The vector from p3 to this LED.
            float distance3 = sqrt(dist3.x * dist3.x + dist3.y * dist3.y + dist3.z * dist3.z);

            // Warp the distance with a sin() function. As the distance value increases, the LEDs will get light,dark,light,dark,etc...
            // You can use a cos() for slightly different shading, or experiment with other functions. Go crazy!
            float color_1 = distance1; // range: 0.0...1.0
            float color_2 = distance2;
            float color_3 = distance3;
            float color_4 = (sin(distance1 * distance2 * colorStretch)) + 2.0 * 0.5;

            // Square the color_f value to weight it towards 0. The image will be darker and have higher contrast.
            color_1 *= color_1 * color_4;
            color_2 *= color_2 * color_4;
            color_3 *= color_3 * color_4;
            color_4 *= color_4;

            ledControl->leds[i] = CRGB(color_1, color_2, color_3);
            //ledControl->leds[i].maximizeBrightness();
        }

        return true;
    }

    bool exit(int param)
    {
        return true;
    }

private:
    struct Point
    {
        float x;
        float y;
        float z;
    };
    float phase = 10.0;
    float scale = 1.5;
    float baseDistance = 5;
    float stepDistance = 0.3;
    const float colorStretch = 0.05; // Higher numbers will produce tighter color bands. I like 0.11 .
};
#endif