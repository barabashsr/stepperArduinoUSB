/**
 * @file StepperMotor.h
 * @brief Stepper motor control class
 * 
 * Controls stepper motors using AccelStepper library
 * Supports position and velocity control with acceleration
 */

#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

#include "../Actuator.h"
#include <AccelStepper.h>

/**
 * @class StepperMotor
 * @brief Controls a stepper motor via RAMPS stepper driver
 * 
 * Provides position and velocity control with smooth acceleration
 */
class StepperMotor : public Actuator {
private:
    AccelStepper* stepper;      // AccelStepper instance
    int stepPin;                // Step pin number
    int dirPin;                 // Direction pin number
    int enablePin;              // Enable pin number
    float stepsPerRev;          // Steps per revolution
    float stepsPerUnit;         // Steps per unit (for conversions)
    bool velocityMode;          // true = velocity mode, false = position mode
    bool invertDirection;       // Invert motor direction
    
    // For continuous rotation
    static constexpr long LONG_MAX_STEPS = 2147483647L;
    static constexpr long LONG_MIN_STEPS = -2147483648L;
    
public:
    /**
     * @brief Constructor
     * @param name Device name
     * @param step Step pin
     * @param dir Direction pin
     * @param enable Enable pin
     * @param stepsRev Steps per revolution
     */
    StepperMotor(const String& name, int step, int dir, int enable, float stepsRev = 200.0);
    
    /**
     * @brief Destructor
     */
    ~StepperMotor();
    
    /**
     * @brief Initialize the stepper motor
     * @return true if successful
     */
    bool init() override;
    
    /**
     * @brief Update motor state (call frequently)
     */
    void update() override;
    
    /**
     * @brief Stop the motor
     */
    void stop() override;
    
    /**
     * @brief Reset motor to default state
     */
    void reset() override;
    
    /**
     * @brief Set target position
     * @param position Target position in radians or meters
     * @return true if accepted
     */
    bool setPosition(float position) override;
    
    /**
     * @brief Set target velocity
     * @param velocity Target velocity in rad/sec or m/sec
     * @return true if accepted
     */
    bool setVelocity(float velocity) override;
    
    /**
     * @brief Get current position
     * @return Position in radians or meters
     */
    float getPosition() const override;
    
    /**
     * @brief Get current velocity
     * @return Velocity in rad/sec or m/sec
     */
    float getVelocity() const override;
    
    /**
     * @brief Set maximum speed
     * @param velocity Maximum speed in rad/sec or m/sec
     */
    void setMaxVelocity(float velocity) override;
    
    /**
     * @brief Set acceleration
     * @param accel Acceleration in rad/sec² or m/sec²
     */
    void setAcceleration(float accel) override;
    
    /**
     * @brief Set steps per unit conversion
     * @param steps Steps per radian or meter
     */
    void setStepsPerUnit(float steps) { stepsPerUnit = steps; }
    
    /**
     * @brief Get steps per unit
     * @return Steps per unit
     */
    float getStepsPerUnit() const { return stepsPerUnit; }
    
    /**
     * @brief Invert motor direction
     * @param invert true to invert
     */
    void setInvertDirection(bool invert) { invertDirection = invert; }
    
    /**
     * @brief Check if motor is at target
     * @return true if at target position
     */
    bool isAtTarget() const override;
    
    /**
     * @brief Enable the motor driver
     */
    void enable() override;
    
    /**
     * @brief Disable the motor driver
     */
    void disable() override;
    
    /**
     * @brief Get current step position
     * @return Current position in steps
     */
    long getCurrentSteps() const;
    
    /**
     * @brief Set current position as zero
     */
    void setZeroPosition();
    
    /**
     * @brief Emergency stop (immediate)
     */
    void emergencyStop();
    
private:
    /**
     * @brief Convert units to steps
     * @param units Position/velocity in user units
     * @return Steps
     */
    long unitsToSteps(float units) const;
    
    /**
     * @brief Convert steps to units
     * @param steps Steps
     * @return Position/velocity in user units
     */
    float stepsToUnits(long steps) const;
    
    /**
     * @brief Convert speed units to steps/sec
     * @param speed Speed in user units/sec
     * @return Steps per second
     */
    float speedUnitsToSteps(float speed) const;
    
    /**
     * @brief Convert steps/sec to speed units
     * @param stepsPerSec Steps per second
     * @return Speed in user units/sec
     */
    float stepsToSpeedUnits(float stepsPerSec) const;
};

#endif // STEPPER_MOTOR_H