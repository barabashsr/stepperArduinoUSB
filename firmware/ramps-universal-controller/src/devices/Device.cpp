/**
 * @file Device.cpp
 * @brief Implementation of Device base class
 */

#include "Device.h"

/**
 * @brief Constructor
 */
Device::Device(const String& deviceName, DeviceType deviceType) {
    name = deviceName;
    type = deviceType;
    state = DeviceState::IDLE;
    enabled = false;
    lastUpdate = millis();
}

/**
 * @brief Reset device to default state
 */
void Device::reset() {
    state = DeviceState::IDLE;
    enabled = false;
    lastUpdate = millis();
}

/**
 * @brief Get current status as string
 */
String Device::getStatus() const {
    String status = "Name: " + name;
    status += ", Type: " + getTypeString();
    status += ", State: ";
    
    switch (state) {
        case DeviceState::IDLE:
            status += "IDLE";
            break;
        case DeviceState::ACTIVE:
            status += "ACTIVE";
            break;
        case DeviceState::ERROR:
            status += "ERROR";
            break;
        case DeviceState::CALIBRATING:
            status += "CALIBRATING";
            break;
        case DeviceState::DISABLED:
            status += "DISABLED";
            break;
    }
    
    status += ", Enabled: ";
    status += enabled ? "YES" : "NO";
    
    return status;
}

/**
 * @brief Get device type as string
 */
String Device::getTypeString() const {
    switch (type) {
        case DeviceType::STEPPER_MOTOR:
            return "StepperMotor";
        case DeviceType::SERVO_MOTOR:
            return "ServoMotor";
        case DeviceType::MOSFET_OUTPUT:
            return "MosfetOutput";
        case DeviceType::END_SWITCH:
            return "EndSwitch";
        case DeviceType::ANALOG_SENSOR:
            return "AnalogSensor";
        case DeviceType::LCD_DISPLAY:
            return "Display";
        case DeviceType::ENCODER:
            return "Encoder";
        default:
            return "Unknown";
    }
}