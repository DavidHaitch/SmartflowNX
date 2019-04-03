#ifndef MOTIONSTATE_H
#define MOTIONSTATE_H
#include "SensorFusion.h"
//#include <MahonyAHRS.h>
#include "MPU9250.h"

#define SAMPLES 128

class MotionState
{
public:
    MotionState();
    bool isEnabled;
    SF orientation;
    int angularVelocityPercent;
    float angularVelocity;
    float angularAcceleration;
    int angularAccelerationPercent;
    int jerkPercent;
    float jerk;
    float pointingX;
    float pointingY;
    float pointingZ;
    int Update(MPU9250* imu); // returns milliseconds if processing falls behind filter rate.
private:
    float maxAngularAcceleration;
    float maxAngularVelocity;
    float angularVelocityHistory[SAMPLES];
    int sampleIdx;
    float maxJerk;
    float lastAccel;
    float lastAngularVelocity;
    long lastUpdateTime;
    
};
#endif