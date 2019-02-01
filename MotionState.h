#ifndef MOTIONSTATE_H
#define MOTIONSTATE_H
#include "SensorFusion.h"
//#include <MahonyAHRS.h>
#include "MPU9250.h"

class MotionState
{
public:
    MotionState();
    bool isEnabled;
    SF orientation;
    int angularVelocityPercent;
    float angularVelocity;
    int jerkPercent;
    float jerk;
    float pointingX;
    float pointingY;
    float pointingZ;
    int Update(MPU9250* imu); // returns milliseconds if processing falls behind filter rate.
private:
    float maxAngularVelocity;
    float maxJerk;
    float lastAccel;
    long lastUpdateTime;
};
#endif