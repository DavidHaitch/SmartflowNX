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

    // float cgX = imu->getGyroX_rads() / 8;
    float cgX = imu->getGyroX_rads();
    float cgY = imu->getGyroY_rads();
    float cgZ = imu->getGyroZ_rads();

    float caX = imu->getAccelX_mss();
    float caY = imu->getAccelY_mss();
    float caZ = imu->getAccelZ_mss();

    float cmX = imu->getMagX_uT();
    float cmY = imu->getMagY_uT();
    float cmZ = imu->getMagZ_uT();
    
    float deltat = orientation.deltatUpdate();
    orientation.MadgwickUpdate(cgX, cgY, cgZ,
            caX, caY, caZ,
            cmX, cmY, cmZ,
            deltat);

    // orientation.MahonyUpdate(cgX, cgY, cgZ,
    //         caX, caY, caZ,
    //         deltat);

    float accel = abs(caX) + abs(caY) + abs(caZ);
    jerk = abs(accel - lastAccel);
    if(jerk > maxJerk)
    {
        maxJerk = jerk;
        relativeJerk = 255;
    }

    if(maxJerk < 1.0 * jerk)
    {
        //We don't have enough samples to really make sense.
        relativeJerk = 0;
    }
    else
    {
        relativeJerk = (jerk / maxJerk) * 255;
    }   

    lastAccel = accel;

    // Y axis is not factored into angular velocity, because that represents the roll axis.
    // For applications where the SmartFlow board does not point along the axis of the prop, this must be changed.
    angularVelocity = abs(cgY) + abs(cgZ);
    if(angularVelocity > maxAngularVelocity)
    {
        maxAngularVelocity = angularVelocity;
        relativeAngularVelocity = 255;
    } 

    if(maxAngularVelocity < 1 * angularVelocity)
    {
        //We don't have enough samples to really make sense.
        relativeAngularVelocity = 0;
    }
    else
    {
        relativeAngularVelocity = (angularVelocity / maxAngularVelocity) * 255;
    }

    //maxAngularVelocity *= 0.999;
  
    angularAcceleration = abs(angularVelocity - lastAngularVelocity);
    lastAngularVelocity = angularVelocity;
    if(angularAcceleration > maxAngularAcceleration)
    {
        maxAngularAcceleration = angularAcceleration;
        relativeAngularAcceleration = 255;
    }

    if(maxAngularAcceleration < 1 * angularAcceleration)
    {
        //We don't have enough samples to really make sense.
        relativeAngularAcceleration = 0;
    }
    else
    {
        relativeAngularAcceleration = (angularAcceleration / maxAngularAcceleration) * 255;
    }

    float yawRad = orientation.getPitchRadians();
    float pitchRad = orientation.getYawRadians();

    pointingX = cos(yawRad) * cos(pitchRad);
    pointingY = sin(yawRad) * cos(pitchRad);
    pointingZ = sin(pitchRad);
    // Serial.print("Orientation: ");
    // Serial.print(pointingX);
    // Serial.print(" ");
    // Serial.print(pointingY);
    // Serial.print(" ");
    // Serial.println(pointingZ);
    return lag;
}