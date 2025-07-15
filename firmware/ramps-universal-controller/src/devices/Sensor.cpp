/**
 * @file Sensor.cpp
 * @brief Implementation of Sensor base class
 */

#include "Sensor.h"

/**
 * @brief Constructor
 */
Sensor::Sensor(const String& deviceName, DeviceType deviceType) 
    : Device(deviceName, deviceType) {
    currentValue = 0.0;
    lastValue = 0.0;
    threshold = DEFAULT_THRESHOLD;
    hasNewData = false;
    lastReadTime = millis();
}

/**
 * @brief Get current status as string
 */
String Sensor::getStatus() const {
    String status = Device::getStatus();
    
    status += ", Value: ";
    status += String(currentValue, 2);
    
    if (hasNewData) {
        status += " (NEW)";
    }
    
    status += ", Threshold: ";
    status += String(threshold, 2);
    
    return status;
}