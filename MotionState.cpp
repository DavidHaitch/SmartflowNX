#include "MotionState.h"
MotionState::MotionState()
{
    isEnabled = true;
}

int MotionState::Update(MPU9250* imu)
{
    if(!isEnabled) return 0;
    
    int dT = millis() - lastUpdateTime;
    int lag = 0;
    imu->readSensor();

    float cgX = imu->getGyroX_rads() / 8;
//    float cgX = imu->getGyroX_rads();
    float cgY = imu->getGyroY_rads();
    float cgZ = imu->getGyroZ_rads();
    
    float caX = imu->getAccelX_mss();
    float caY = imu->getAccelY_mss();
    float caZ = imu->getAccelZ_mss();

    float cmX = imu->getMagX_uT();
    float cmY = imu->getMagY_uT();
    float cmZ = imu->getMagZ_uT();
    
    float deltat = orientation.deltatUpdate();
    // orientation.MadgwickUpdate(cgX, cgY, cgZ,
    //         caX, caY, caZ,
    //         cmX, cmY, cmZ,
    //         deltat);

    orientation.MahonyUpdate(cgX, cgY, cgZ,
            caX, caY, caZ,
            deltat);

    float accel = abs(caX) + abs(caY) + abs(caZ);
    jerk = abs(accel - lastAccel);
    if(jerk > maxJerk)
    {
        maxJerk = jerk;
        jerkPercent = 100;
    }

    if(maxJerk < 1.5 * jerk)
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
    angularVelocity = abs(cgY) + abs(cgZ);
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

    //maxAngularVelocity *= 0.999;
  
    angularAcceleration = abs(angularVelocity - lastAngularVelocity);
    lastAngularVelocity = angularVelocity;
    if(angularAcceleration > maxAngularAcceleration)
    {
        maxAngularAcceleration = angularAcceleration;
        angularAccelerationPercent = 100;
    }

    if(maxAngularAcceleration < 1 * angularAcceleration)
    {
        //We don't have enough samples to really make sense.
        angularAccelerationPercent = 0;
    }
    else
    {
        angularAccelerationPercent = (angularAcceleration / maxAngularAcceleration) * 100;
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