#include "MotionState.h"
MotionState::MotionState()
{
    isEnabled = true;
    orientation.begin(60);
}

int MotionState::Update(MPU9250_DMP* imu)
{
    if(!isEnabled) return 0;
    
    int dT = millis() - lastUpdateTime;
    int lag = 0;
    if(dT < 16)
    {
        return 0;
    }

    lastUpdateTime = millis();
    lag = dT - 8;
    //Serial.println(lag);
    if ( imu->dataReady() )
    {
      imu->update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
    }

    float cgX = imu->calcGyro(imu->gx);
    float cgY = imu->calcGyro(imu->gy);
    float cgZ = imu->calcGyro(imu->gz);

    float caX = imu->calcAccel(imu->ax);
    float caY = imu->calcAccel(imu->ay);
    float caZ = imu->calcAccel(imu->az);

    float cmX = imu->calcMag(imu->mx);
    float cmY = imu->calcMag(imu->my);
    float cmZ = imu->calcMag(imu->mz);
    
    // orientation.updateIMU(cgX, cgY, cgZ,
    //                        caX, caY, caZ);
    orientation.update(cgX, cgY, cgZ,
            caX, caY, caZ,
            cmX, cmY, cmZ);

    float accel = abs(caX) + abs(caY) + abs(caZ);
    jerk = abs(accel - lastAccel);
    if(jerk > maxJerk)
    {
        maxJerk = jerk;
        jerkPercent = 100;
    }

    if(maxJerk < 1 * jerk)
    {
        //We don't have enough samples to really make sense.
        jerkPercent = 0;
    }
    else
    {
        jerkPercent = (jerk / maxJerk) * 100;
    }   

    lastAccel = accel;

    // Y axis is not factored into angular velocity, because that represents the roll axis.
    // For applications where the SmartFlow board does not point along the axis of the prop, this must be changed.
    angularVelocity = abs(cgX) + abs(cgZ);
    if(angularVelocity > maxAngularVelocity)
    {
        maxAngularVelocity = angularVelocity;
        angularVelocityPercent = 100;
    } 

    if(maxAngularVelocity < 1 * angularVelocity)
    {
        //We don't have enough samples to really make sense.
        angularVelocityPercent = 0;
    }
    else
    {
        angularVelocityPercent = (angularVelocity / maxAngularVelocity) * 100;
    }

    float yawRad = orientation.getYawRadians();
    float pitchRad = orientation.getRollRadians();

    pointingX = cos(yawRad) * cos(pitchRad);
    pointingY = sin(yawRad) * cos(pitchRad);
    pointingZ = sin(pitchRad);
    return lag;
}