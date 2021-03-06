#include "MotionState.h"
MotionState::MotionState()
{
    isEnabled = true;
}

#ifdef BATON
int MotionState::Update(Adafruit_LSM9DS1 *imu)
#endif
#ifdef STAFF
    int MotionState::Update(MPU9250 *imu)
#endif
{
    if (!isEnabled)
        return 0;

    long now = millis();
    int dT = now - lastUpdateTime;
    lastUpdateTime = now;

#ifdef BATON
    imu->read();
    sensors_event_t a, m, g, temp;
    imu->getEvent(&a, &m, &g, &temp);
    float radConv = 3.14159 / 180.0;
    float cgX = g.gyro.x;
    float cgY = g.gyro.y;
    float cgZ = g.gyro.z;

    // Y axis is not factored into angular velocity, because that represents the roll axis.
    // For applications where the SmartFlow board does not point along the axis of the prop, this must be changed.
    angularVelocity = abs(cgY) + abs(cgZ);
    float centripetalAccel = (angularVelocity * angularVelocity) * 0.33;

    float caX = a.acceleration.x;
    float caY = a.acceleration.y;
    float caZ = a.acceleration.z;

    float cmX = m.magnetic.x;
    float cmY = m.magnetic.y;
    float cmZ = m.magnetic.z;
#endif

#ifdef STAFF
    imu->readSensor();
    float cgX = imu->getGyroX_rads();
    float cgY = imu->getGyroY_rads();
    float cgZ = imu->getGyroZ_rads();

    float scaled_cgX = cgX * (1 - (relativeAngularVelocity / 255));
    float scaled_cgY = cgY * (1 - (relativeAngularVelocity / 255));
    float scaled_cgZ = cgZ * (1 - (relativeAngularVelocity / 255));

    // Y axis is not factored into angular velocity, because that represents the roll axis.
    // For applications where the SmartFlow board does not point along the axis of the prop, this must be changed.
    angularVelocity = abs(cgY) + abs(cgZ);
    float centripetalAccel = (angularVelocity * angularVelocity) * 0.66;

    float caX = imu->getAccelX_mss();
    float caY = imu->getAccelY_mss();
    float caZ = imu->getAccelZ_mss();

    float cmX = imu->getMagX_uT();
    float cmY = imu->getMagY_uT();
    float cmZ = imu->getMagZ_uT();
#endif

    rawAxialAccel = caX;
    
    float scaled_caX = caX * (1 - (255 / relativeAngularVelocity));
    float scaled_caY = caY * (1 - (255 / relativeAngularVelocity));
    float scaled_caZ = caZ * (1 - (255 / relativeAngularVelocity));

    float deltat = orientation.deltatUpdate();
    // orientation.MadgwickUpdate(cgX, cgY, cgZ,
    //         caX, caY, caZ,
    //         cmX, cmY, cmZ,
    //         deltat);

    orientation.MahonyUpdate(cgX, cgY, cgZ,
                             scaled_caX, scaled_caY, scaled_caZ,
                             deltat);

    float accel = abs(caX) + abs(caY) + abs(caZ);
    jerk = abs(accel - lastAccel);
    if (jerk > maxJerk)
    {
        maxJerk = jerk;
        relativeJerk = 255;
    }

    if (maxJerk < 1.0 * jerk)
    {
        //We don't have enough samples to really make sense.
        relativeJerk = 0;
    }
    else
    {
        relativeJerk = (jerk / maxJerk) * 255;
    }

    lastAccel = accel;

    if (angularVelocity > maxAngularVelocity)
    {
        maxAngularVelocity = angularVelocity;
        relativeAngularVelocity = 255;
    }
    else
    {
        relativeAngularVelocity = (angularVelocity / maxAngularVelocity) * 255.0;
    }

    maxAngularVelocity *= 0.99;

    angularAcceleration = abs(angularVelocity - lastAngularVelocity);
    lastAngularVelocity = angularVelocity;
    if (angularAcceleration > maxAngularAcceleration)
    {
        maxAngularAcceleration = angularAcceleration;
        relativeAngularAcceleration = 255;
    }

    if (maxAngularAcceleration < 1 * angularAcceleration)
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
    return dT;
}