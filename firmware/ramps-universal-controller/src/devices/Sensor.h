/**
 * @file Sensor.h
 * @brief Base class for all sensor devices
 * 
 * Abstract base class for devices that sense the environment
 * (switches, analog sensors, etc.)
 */

#ifndef SENSOR_H
#define SENSOR_H

#include "Device.h"

/**
 * @class Sensor
 * @brief Abstract base class for sensor devices
 * 
 * Provides common interfaces for reading sensor values
 */
class Sensor : public Device {
protected:
    float currentValue;         // Current sensor reading
    float lastValue;            // Previous sensor reading
    float threshold;            // Threshold for change detection
    bool hasNewData;            // Flag for new data available
    unsigned long lastReadTime; // Last sensor read timestamp
    
public:
    /**
     * @brief Constructor
     * @param deviceName User-defined name
     * @param deviceType Type of sensor
     */
    Sensor(const String& deviceName, DeviceType deviceType);
    
    /**
     * @brief Read current sensor value
     * @return Current value
     */
    virtual float readValue() = 0;
    
    /**
     * @brief Get last read value without new reading
     * @return Last value
     */
    float getValue() const { return currentValue; }
    
    /**
     * @brief Get previous value
     * @return Previous value
     */
    float getLastValue() const { return lastValue; }
    
    /**
     * @brief Check if new data is available
     * @return true if new data since last read
     */
    bool hasNewDataAvailable() const { return hasNewData; }
    
    /**
     * @brief Clear new data flag
     */
    void clearNewDataFlag() { hasNewData = false; }
    
    /**
     * @brief Set threshold for change detection
     * @param thresh Threshold value
     */
    void setThreshold(float thresh) { threshold = abs(thresh); }
    
    /**
     * @brief Get threshold
     * @return Current threshold
     */
    float getThreshold() const { return threshold; }
    
    /**
     * @brief Check if value changed significantly
     * @return true if change exceeds threshold
     */
    bool hasSignificantChange() const {
        return abs(currentValue - lastValue) > threshold;
    }
    
    /**
     * @brief Get time since last reading
     * @return Milliseconds since last read
     */
    unsigned long getTimeSinceLastRead() const {
        return millis() - lastReadTime;
    }
    
    /**
     * @brief Stop sensor (base implementation)
     */
    void stop() override {
        state = DeviceState::IDLE;
    }
    
    /**
     * @brief Get list of supported interfaces
     * @return Comma-separated list of interfaces
     */
    String getInterfaces() const override {
        return "read,value,status";
    }
    
    /**
     * @brief Get current status as string
     * @return Status string with sensor value
     */
    String getStatus() const override;
    
protected:
    /**
     * @brief Update sensor reading
     * @param newValue New sensor value
     */
    void updateValue(float newValue) {
        lastValue = currentValue;
        currentValue = newValue;
        lastReadTime = millis();
        hasNewData = true;
        updateTimestamp();
    }
};

#endif // SENSOR_H