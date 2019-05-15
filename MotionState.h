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
    int Update(MPU9250* imu); // returns milliseconds if processing falls behind filter rate.
private:
    float maxAngularAcceleration;
    float maxAngularVelocity;
    float maxJerk;
    float lastAccel;
    float lastAngularVelocity;
    long lastUpdateTime;
    
};
#endif