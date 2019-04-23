#include "MotionState.h"
MotionState::MotionState()
{
    isEnabled = true;
}

int MotionState::Update(Adafruit_LSM9DS1* imu)
{
    if(!isEnabled) return 0;
    
    long now = millis();
    int dT = now - lastUpdateTime;
    lastUpdateTime = now;
    imu->read();
    sensors_event_t a, m, g, temp;
    imu->getEvent(&a, &m, &g, &temp); 
    float radConv = 3.14159 / 180.0;
    float cgX = g.gyro.x * radConv;
    float cgY = g.gyro.y * radConv;
    float cgZ = g.gyro.z * radConv;

    float caX = a.acceleration.x;
    float caY = a.acceleration.y;
    float caZ = a.acceleration.z;

    float cmX = m.magnetic.x;
    float cmY = m.magnetic.y;
    float cmZ = m.magnetic.z;
    
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
    return dT;
}