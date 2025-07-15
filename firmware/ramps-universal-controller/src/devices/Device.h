/**
 * @file Device.h
 * @brief Base class for all devices
 * 
 * Abstract base class providing common functionality for all device types
 */

#ifndef DEVICE_H
#define DEVICE_H

#include <Arduino.h>
#include "Config.h"

/**
 * @enum DeviceType
 * @brief Types of devices in the system
 */
enum class DeviceType {
    STEPPER_MOTOR,
    SERVO_MOTOR,
    MOSFET_OUTPUT,
    END_SWITCH,
    ANALOG_SENSOR,
    LCD_DISPLAY,    // Renamed from DISPLAY to avoid Arduino.h conflict
    ENCODER,
    UNKNOWN
};

/**
 * @enum DeviceState
 * @brief Common device states
 */
enum class DeviceState {
    IDLE,           // Device is idle/stopped
    ACTIVE,         // Device is active/moving
    ERROR,          // Device has error
    CALIBRATING,    // Device is calibrating
    DISABLED        // Device is disabled
};

/**
 * @class Device
 * @brief Abstract base class for all devices
 * 
 * Provides common properties and methods for all device types
 */
class Device {
protected:
    String name;                // User-defined device name
    DeviceType type;            // Device type
    DeviceState state;          // Current device state
    bool enabled;               // Is device enabled?
    unsigned long lastUpdate;   // Last update timestamp
    
public:
    /**
     * @brief Constructor
     * @param deviceName User-defined name
     * @param deviceType Type of device
     */
    Device(const String& deviceName, DeviceType deviceType);
    
    /**
     * @brief Virtual destructor
     */
    virtual ~Device() = default;
    
    /**
     * @brief Initialize the device
     * @return true if initialization successful
     */
    virtual bool init() = 0;
    
    /**
     * @brief Update device state (called in main loop)
     */
    virtual void update() = 0;
    
    /**
     * @brief Stop/disable the device
     */
    virtual void stop() = 0;
    
    /**
     * @brief Reset device to default state
     */
    virtual void reset();
    
    /**
     * @brief Get list of supported interfaces
     * @return Comma-separated list of interfaces
     */
    virtual String getInterfaces() const = 0;
    
    /**
     * @brief Get current status as string
     * @return Status string
     */
    virtual String getStatus() const;
    
    /**
     * @brief Get device name
     * @return Device name
     */
    const String& getName() const { return name; }
    
    /**
     * @brief Get device type
     * @return Device type enum
     */
    DeviceType getType() const { return type; }
    
    /**
     * @brief Get device type as string
     * @return Type string
     */
    String getTypeString() const;
    
    /**
     * @brief Get device state
     * @return Current state
     */
    DeviceState getState() const { return state; }
    
    /**
     * @brief Check if device is enabled
     * @return true if enabled
     */
    bool isEnabled() const { return enabled; }
    
    /**
     * @brief Enable the device
     */
    virtual void enable() { enabled = true; }
    
    /**
     * @brief Disable the device
     */
    virtual void disable() { 
        enabled = false; 
        stop(); 
    }
    
    /**
     * @brief Get time since last update
     * @return Milliseconds since last update
     */
    unsigned long getTimeSinceUpdate() const { 
        return millis() - lastUpdate; 
    }
    
protected:
    /**
     * @brief Update the last update timestamp
     */
    void updateTimestamp() { lastUpdate = millis(); }
    
    /**
     * @brief Set device state
     * @param newState New state
     */
    void setState(DeviceState newState) { state = newState; }
};

#endif // DEVICE_H