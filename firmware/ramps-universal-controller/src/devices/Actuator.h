/**
 * @file Actuator.h
 * @brief Base class for all actuator devices
 * 
 * Abstract base class for devices that act on the environment
 * (motors, outputs, etc.)
 */

#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "Device.h"

/**
 * @class Actuator
 * @brief Abstract base class for actuator devices
 * 
 * Provides common interfaces for position and velocity control
 */
class Actuator : public Device {
protected:
    float currentPosition;      // Current position (steps, degrees, etc.)
    float targetPosition;       // Target position
    float currentVelocity;      // Current velocity (steps/sec, rad/sec, etc.)
    float targetVelocity;       // Target velocity
    float maxVelocity;          // Maximum allowed velocity
    float acceleration;         // Acceleration rate
    
public:
    /**
     * @brief Constructor
     * @param deviceName User-defined name
     * @param deviceType Type of actuator
     */
    Actuator(const String& deviceName, DeviceType deviceType);
    
    /**
     * @brief Set target position
     * @param position Target position in device units
     * @return true if command accepted
     */
    virtual bool setPosition(float position) = 0;
    
    /**
     * @brief Set target velocity
     * @param velocity Target velocity in device units/sec
     * @return true if command accepted
     */
    virtual bool setVelocity(float velocity) = 0;
    
    /**
     * @brief Get current position
     * @return Current position in device units
     */
    virtual float getPosition() const { return currentPosition; }
    
    /**
     * @brief Get current velocity
     * @return Current velocity in device units/sec
     */
    virtual float getVelocity() const { return currentVelocity; }
    
    /**
     * @brief Get target position
     * @return Target position
     */
    float getTargetPosition() const { return targetPosition; }
    
    /**
     * @brief Get target velocity
     * @return Target velocity
     */
    float getTargetVelocity() const { return targetVelocity; }
    
    /**
     * @brief Set maximum velocity
     * @param velocity Maximum velocity
     */
    virtual void setMaxVelocity(float velocity) { 
        maxVelocity = abs(velocity); 
    }
    
    /**
     * @brief Get maximum velocity
     * @return Maximum velocity
     */
    float getMaxVelocity() const { return maxVelocity; }
    
    /**
     * @brief Set acceleration
     * @param accel Acceleration rate
     */
    virtual void setAcceleration(float accel) { 
        acceleration = abs(accel); 
    }
    
    /**
     * @brief Get acceleration
     * @return Acceleration rate
     */
    float getAcceleration() const { return acceleration; }
    
    /**
     * @brief Check if actuator is at target position
     * @return true if at target
     */
    virtual bool isAtTarget() const {
        return abs(currentPosition - targetPosition) < 0.1;
    }
    
    /**
     * @brief Check if actuator is moving
     * @return true if moving
     */
    virtual bool isMoving() const {
        return state == DeviceState::ACTIVE;
    }
    
    /**
     * @brief Get list of supported interfaces
     * @return Comma-separated list of interfaces
     */
    String getInterfaces() const override {
        return "position,velocity,stop,reset";
    }
    
    /**
     * @brief Get current status as string
     * @return Status string with position/velocity info
     */
    String getStatus() const override;
    
protected:
    /**
     * @brief Constrain value to valid range
     * @param value Value to constrain
     * @param minVal Minimum value
     * @param maxVal Maximum value
     * @return Constrained value
     */
    float constrainValue(float value, float minVal, float maxVal) {
        if (value < minVal) return minVal;
        if (value > maxVal) return maxVal;
        return value;
    }
};

#endif // ACTUATOR_H