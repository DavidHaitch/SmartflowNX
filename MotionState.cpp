#include "MotionState.h"
MotionState::MotionState()
{
    isEnabled = true;
}

int MotionState::Update(MPU9250_DMP* imu)
{
    if(!isEnabled) return 0;
    
    int dT = millis() - lastUpdateTime;
    int lag = 0;
    if ( imu->dataReady() )
    {
        imu->update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
    }
    else
    {
        return 0;
    }

    float cgX = imu->calcGyro(imu->gx) * 0.0174533;
    float cgY = imu->calcGyro(imu->gy) * 0.0174533 * 0;
    float cgZ = imu->calcGyro(imu->gz) * 0.0174533;
    float caX = imu->calcAccel(imu->ax);
    float caY = imu->calcAccel(imu->ay);
    float caZ = imu->calcAccel(imu->az);

    float cmX = imu->calcMag(imu->mx);
    float cmY = imu->calcMag(imu->my);
    float cmZ = imu->calcMag(imu->mz);
    
    float deltat = orientation.deltatUpdate();
    // orientation.MadgwickUpdate(cgX, cgY, cgZ,
    //         caX, caY, caZ,
    //         cmX, cmY, cmZ,
    //         deltat);

    orientation.MadgwickUpdate(cgX, cgY, cgZ,
            caX, caY, caZ,
            deltat);

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
    // Serial.print("Yaw: ");
    // Serial.println(orientation.getYaw());
    // Serial.print("Pitch: ");
    // Serial.println(orientation.getPitch());
    // Serial.print("Roll: ");
    // Serial.println(orientation.getRoll());
    return lag;
}