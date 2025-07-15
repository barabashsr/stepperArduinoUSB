/**
 * @file StepperMotor.cpp
 * @brief Implementation of StepperMotor class
 */

#include "StepperMotor.h"
#include "PinDefinitions.h"
#include "Config.h"

/**
 * @brief Constructor
 */
StepperMotor::StepperMotor(const String& name, int step, int dir, int enable, float stepsRev) 
    : Actuator(name, DeviceType::STEPPER_MOTOR) {
    stepPin = step;
    dirPin = dir;
    enablePin = enable;
    stepsPerRev = stepsRev;
    stepsPerUnit = stepsRev / (2.0 * PI);  // Default: steps per radian
    velocityMode = false;
    invertDirection = false;
    
    // Create AccelStepper instance
    stepper = new AccelStepper(AccelStepper::DRIVER, stepPin, dirPin);
}

/**
 * @brief Destructor
 */
StepperMotor::~StepperMotor() {
    if (stepper) {
        delete stepper;
    }
}

/**
 * @brief Initialize the stepper motor
 */
bool StepperMotor::init() {
    if (!stepper) return false;
    
    // Configure enable pin
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin, STEPPER_ENABLE_OFF);  // Start disabled
    
    // Configure stepper parameters
    stepper->setMaxSpeed(speedUnitsToSteps(maxVelocity));
    stepper->setAcceleration(speedUnitsToSteps(acceleration));
    stepper->setPinsInverted(invertDirection, false, true);  // dir, step, enable
    stepper->setCurrentPosition(0);
    
    // Initialize state
    currentPosition = 0.0;
    targetPosition = 0.0;
    currentVelocity = 0.0;
    targetVelocity = 0.0;
    velocityMode = false;
    enabled = false;  // Start disabled
    state = DeviceState::DISABLED;
    
    return true;
}

/**
 * @brief Update motor state
 */
void StepperMotor::update() {
    if (!stepper || !enabled) return;
    
    // Handle velocity mode
    if (velocityMode) {
        // In velocity mode, continuously adjust target to maintain speed
        if (abs(stepper->speed()) < abs(speedUnitsToSteps(targetVelocity)) * 0.95) {
            // Still accelerating or need to adjust
            long target = (targetVelocity > 0) ? LONG_MAX_STEPS : LONG_MIN_STEPS;
            stepper->moveTo(target);
        }
        
        // Check if we need to stop
        if (targetVelocity == 0.0 && stepper->speed() == 0.0) {
            velocityMode = false;
            state = DeviceState::IDLE;
        } else {
            state = DeviceState::ACTIVE;
        }
    } else {
        // Position mode
        if (stepper->distanceToGo() != 0) {
            state = DeviceState::ACTIVE;
        } else {
            state = DeviceState::IDLE;
        }
    }
    
    // Run the stepper
    bool moving = stepper->run();
    
    // Update current values
    currentPosition = stepsToUnits(stepper->currentPosition());
    currentVelocity = stepsToSpeedUnits(stepper->speed());
    
    updateTimestamp();
}

/**
 * @brief Stop the motor
 */
void StepperMotor::stop() {
    if (!stepper) return;
    
    velocityMode = false;
    targetVelocity = 0.0;
    stepper->stop();  // Decelerate to stop
    state = DeviceState::IDLE;
}

/**
 * @brief Reset motor to default state
 */
void StepperMotor::reset() {
    stop();
    if (stepper) {
        stepper->setCurrentPosition(0);
    }
    currentPosition = 0.0;
    targetPosition = 0.0;
    currentVelocity = 0.0;
    targetVelocity = 0.0;
    velocityMode = false;
    Actuator::reset();
}

/**
 * @brief Set target position
 */
bool StepperMotor::setPosition(float position) {
    if (!stepper) {
        if (DEBUG_ENABLED) {
            Serial.println("ERROR: StepperMotor::setPosition() - Stepper object is null");
        }
        return false;
    }
    
    if (!enabled) {
        if (DEBUG_ENABLED) {
            Serial.print("ERROR: StepperMotor::setPosition() - Motor '");
            Serial.print(name);
            Serial.println("' is not enabled. Use 'enable' command first.");
        }
        return false;
    }
    
    velocityMode = false;
    targetPosition = position;
    long targetSteps = unitsToSteps(position);
    stepper->moveTo(targetSteps);
    
    return true;
}

/**
 * @brief Set target velocity
 */
bool StepperMotor::setVelocity(float velocity) {
    if (!stepper) {
        if (DEBUG_ENABLED) {
            Serial.println("ERROR: StepperMotor::setVelocity() - Stepper object is null");
        }
        return false;
    }
    
    if (!enabled) {
        if (DEBUG_ENABLED) {
            Serial.print("ERROR: StepperMotor::setVelocity() - Motor '");
            Serial.print(name);
            Serial.println("' is not enabled. Use 'enable' command first.");
        }
        return false;
    }
    
    // Constrain velocity
    velocity = constrainValue(velocity, -maxVelocity, maxVelocity);
    targetVelocity = velocity;
    
    if (velocity == 0.0) {
        // Stop command
        velocityMode = false;
        stepper->stop();
    } else {
        // Enter velocity mode
        velocityMode = true;
        stepper->setMaxSpeed(speedUnitsToSteps(abs(velocity)));
        
        // Set target far in the direction of motion
        long target = (velocity > 0) ? LONG_MAX_STEPS : LONG_MIN_STEPS;
        stepper->moveTo(target);
    }
    
    return true;
}

/**
 * @brief Get current position
 */
float StepperMotor::getPosition() const {
    if (!stepper) return 0.0;
    return stepsToUnits(stepper->currentPosition());
}

/**
 * @brief Get current velocity
 */
float StepperMotor::getVelocity() const {
    if (!stepper) return 0.0;
    return stepsToSpeedUnits(stepper->speed());
}

/**
 * @brief Set maximum speed
 */
void StepperMotor::setMaxVelocity(float velocity) {
    Actuator::setMaxVelocity(velocity);
    if (stepper) {
        stepper->setMaxSpeed(speedUnitsToSteps(maxVelocity));
    }
}

/**
 * @brief Set acceleration
 */
void StepperMotor::setAcceleration(float accel) {
    Actuator::setAcceleration(accel);
    if (stepper) {
        stepper->setAcceleration(speedUnitsToSteps(acceleration));
    }
}

/**
 * @brief Check if motor is at target
 */
bool StepperMotor::isAtTarget() const {
    if (!stepper) return true;
    
    if (velocityMode) {
        // In velocity mode, we're "at target" if we're at the target speed
        return abs(currentVelocity - targetVelocity) < 0.1;
    } else {
        // In position mode, check if we're at the target position
        return stepper->distanceToGo() == 0;
    }
}

/**
 * @brief Enable the motor driver
 */
void StepperMotor::enable() {
    digitalWrite(enablePin, STEPPER_ENABLE_ON);
    enabled = true;
    state = DeviceState::IDLE;
}

/**
 * @brief Disable the motor driver
 */
void StepperMotor::disable() {
    stop();
    digitalWrite(enablePin, STEPPER_ENABLE_OFF);
    enabled = false;
    state = DeviceState::DISABLED;
}

/**
 * @brief Get current step position
 */
long StepperMotor::getCurrentSteps() const {
    if (!stepper) return 0;
    return stepper->currentPosition();
}

/**
 * @brief Set current position as zero
 */
void StepperMotor::setZeroPosition() {
    if (stepper) {
        stepper->setCurrentPosition(0);
        currentPosition = 0.0;
    }
}

/**
 * @brief Emergency stop (immediate)
 */
void StepperMotor::emergencyStop() {
    if (stepper) {
        velocityMode = false;
        targetVelocity = 0.0;
        stepper->setSpeed(0);
        stepper->stop();
        stepper->runToPosition();  // Forces immediate stop
    }
    state = DeviceState::IDLE;
}

/**
 * @brief Convert units to steps
 */
long StepperMotor::unitsToSteps(float units) const {
    return (long)(units * stepsPerUnit);
}

/**
 * @brief Convert steps to units
 */
float StepperMotor::stepsToUnits(long steps) const {
    return (float)steps / stepsPerUnit;
}

/**
 * @brief Convert speed units to steps/sec
 */
float StepperMotor::speedUnitsToSteps(float speed) const {
    return speed * stepsPerUnit;
}

/**
 * @brief Convert steps/sec to speed units
 */
float StepperMotor::stepsToSpeedUnits(float stepsPerSec) const {
    return stepsPerSec / stepsPerUnit;
}