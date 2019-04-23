#ifndef MOTIONSTATE_H
#define MOTIONSTATE_H
#include "SensorFusion.h"
//#include <MahonyAHRS.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h> 

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
    int Update(Adafruit_LSM9DS1* imu); // returns milliseconds if processing falls behind filter rate.
private:
    float maxAngularAcceleration;
    float maxAngularVelocity;
    float maxJerk;
    float lastAccel;
    float lastAngularVelocity;
    long lastUpdateTime;
};
#endif