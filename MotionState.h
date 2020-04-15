#ifndef MOTIONSTATE_H
#define MOTIONSTATE_H
#include "HardwareType.h"
#include "SensorFusion.h"

#ifdef BATON
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h> 
#endif
#ifdef STAFF
#include "MPU9250.h"
#endif

class MotionState
{
public:
    MotionState();
    bool isEnabled;
    SF orientation;
    int relativeAngularVelocity;
    float angularVelocity;
    float angularAcceleration;
    int relativeAngularAcceleration;
    int relativeJerk;
    float jerk;
    float pointingX;
    float pointingY;
    float pointingZ;
    float rawAxialAccel;
    #ifdef BATON
    int Update(Adafruit_LSM9DS1* imu); // returns milliseconds if processing falls behind filter rate.
    #endif
    #ifdef STAFF
    int Update(MPU9250* imu); // returns milliseconds if processing falls behind filter rate.
    #endif
private:
    float maxAngularAcceleration;
    float maxAngularVelocity;
    float maxJerk;
    float lastAccel;
    float lastAngularVelocity;
    long lastUpdateTime;
};
#endif