/**
 * @file MosfetOutput.cpp
 * @brief Implementation of MosfetOutput class
 */

#include "MosfetOutput.h"

/**
 * @brief Constructor
 */
MosfetOutput::MosfetOutput(const String& name, int pin, bool pwm)
    : Actuator(name, DeviceType::MOSFET_OUTPUT) {
    outputPin = pin;
    supportsPWM = pwm;
    currentPWM = 0;
    targetPWM = 0;
    isOn = false;
    
    // Map PWM values to position/velocity for base class
    currentPosition = 0.0;  // 0.0-1.0 normalized
    targetPosition = 0.0;
    currentVelocity = 0.0;
    targetVelocity = 0.0;
    maxVelocity = 1.0;  // Full range in 1 second
}

/**
 * @brief Initialize the output
 */
bool MosfetOutput::init() {
    // Configure pin as output
    pinMode(outputPin, OUTPUT);
    
    // Start with output OFF
    writeOutput(0);
    
    // Initialize state
    state = DeviceState::IDLE;
    enabled = true;
    
    return true;
}

/**
 * @brief Update output state
 */
void MosfetOutput::update() {
    if (!enabled) return;
    
    // Handle velocity-based fading
    if (targetVelocity != 0.0) {
        // Calculate new position based on velocity
        float deltaTime = (millis() - lastUpdate) / 1000.0;  // Convert to seconds
        float newPosition = currentPosition + (targetVelocity * deltaTime);
        
        // Constrain to valid range
        newPosition = constrainValue(newPosition, 0.0, 1.0);
        
        // Check if we've reached the limit
        if ((targetVelocity > 0 && newPosition >= 1.0) || 
            (targetVelocity < 0 && newPosition <= 0.0)) {
            targetVelocity = 0.0;
            currentVelocity = 0.0;
        }
        
        // Update position
        currentPosition = newPosition;
        currentPWM = normalizeToPWM(currentPosition);
        writeOutput(currentPWM);
    } else if (currentPWM != targetPWM) {
        // Direct transition to target
        currentPWM = targetPWM;
        currentPosition = PWMToNormalized(currentPWM);
        writeOutput(currentPWM);
    }
    
    // Update state
    if (currentPWM > 0) {
        isOn = true;
        state = (currentPWM != targetPWM || targetVelocity != 0) ? 
                DeviceState::ACTIVE : DeviceState::IDLE;
    } else {
        isOn = false;
        state = DeviceState::IDLE;
    }
    
    updateTimestamp();
}

/**
 * @brief Stop/turn off the output
 */
void MosfetOutput::stop() {
    targetPWM = 0;
    targetPosition = 0.0;
    targetVelocity = 0.0;
    currentVelocity = 0.0;
    update();  // Apply immediately
}

/**
 * @brief Reset to default state
 */
void MosfetOutput::reset() {
    turnOff();
    Actuator::reset();
}

/**
 * @brief Set output level (0.0-1.0)
 */
bool MosfetOutput::setPosition(float position) {
    if (!enabled) return false;
    
    // Constrain to valid range
    position = constrainValue(position, 0.0, 1.0);
    
    targetPosition = position;
    targetPWM = normalizeToPWM(position);
    targetVelocity = 0.0;  // Cancel any velocity-based movement
    
    return true;
}

/**
 * @brief Set fade velocity
 */
bool MosfetOutput::setVelocity(float velocity) {
    if (!enabled) return false;
    
    // Velocity represents change in position per second
    targetVelocity = constrainValue(velocity, -maxVelocity, maxVelocity);
    currentVelocity = targetVelocity;
    
    // Set target position based on velocity direction
    if (velocity > 0) {
        targetPosition = 1.0;
        targetPWM = 255;
    } else if (velocity < 0) {
        targetPosition = 0.0;
        targetPWM = 0;
    } else {
        // Zero velocity - stop at current position
        targetPosition = currentPosition;
        targetPWM = currentPWM;
    }
    
    return true;
}

/**
 * @brief Turn output ON
 */
bool MosfetOutput::turnOn() {
    return setPWM(255);
}

/**
 * @brief Turn output OFF
 */
bool MosfetOutput::turnOff() {
    return setPWM(0);
}

/**
 * @brief Set PWM duty cycle
 */
bool MosfetOutput::setPWM(uint8_t duty) {
    if (!enabled) return false;
    
    targetPWM = duty;
    targetPosition = PWMToNormalized(duty);
    targetVelocity = 0.0;  // Cancel velocity mode
    
    return true;
}

/**
 * @brief Get current status
 */
String MosfetOutput::getStatus() const {
    String status = Device::getStatus();
    
    status += ", Output: ";
    if (isOn) {
        status += "ON (";
        status += String((currentPWM * 100) / 255);
        status += "%)";
    } else {
        status += "OFF";
    }
    
    if (supportsPWM) {
        status += ", PWM: ";
        status += String(currentPWM);
        status += "/255";
    }
    
    return status;
}

/**
 * @brief Write output value
 */
void MosfetOutput::writeOutput(uint8_t value) {
    if (supportsPWM) {
        analogWrite(outputPin, value);
    } else {
        // Digital only - treat as ON/OFF
        digitalWrite(outputPin, value > 127 ? HIGH : LOW);
        currentPWM = value > 127 ? 255 : 0;
    }
}