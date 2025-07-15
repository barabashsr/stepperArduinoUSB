/**
 * @file Actuator.cpp
 * @brief Implementation of Actuator base class
 */

#include "Actuator.h"

/**
 * @brief Constructor
 */
Actuator::Actuator(const String& deviceName, DeviceType deviceType) 
    : Device(deviceName, deviceType) {
    currentPosition = 0.0;
    targetPosition = 0.0;
    currentVelocity = 0.0;
    targetVelocity = 0.0;
    maxVelocity = DEFAULT_MAX_SPEED;
    acceleration = DEFAULT_ACCELERATION;
}

/**
 * @brief Get current status as string
 */
String Actuator::getStatus() const {
    String status = Device::getStatus();
    
    status += ", Pos: ";
    status += String(currentPosition, 2);
    status += "/";
    status += String(targetPosition, 2);
    
    status += ", Vel: ";
    status += String(currentVelocity, 2);
    status += "/";
    status += String(targetVelocity, 2);
    
    status += ", MaxVel: ";
    status += String(maxVelocity, 2);
    
    return status;
}