#ifndef MOTIONSTATE_H
#define MOTIONSTATE_H
//#include <MadgwickAHRS.h>
#include <MahonyAHRS.h>
#include <SparkFunMPU9250-DMP.h>

class MotionState
{
public:
    MotionState();
    bool isEnabled;
    //Madgwick orientation;
    Mahony orientation;
    int angularVelocityPercent;
    float angularVelocity;
    int jerkPercent;
    float jerk;
    float pointingX;
    float pointingY;
    float pointingZ;
    int Update(MPU9250_DMP* imu); // returns milliseconds if processing falls behind filter rate.
private:
    float maxAngularVelocity;
    float maxJerk;
    float lastAccel;
    long lastUpdateTime;
};
#endif