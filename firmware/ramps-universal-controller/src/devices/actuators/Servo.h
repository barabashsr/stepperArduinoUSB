/**
 * @file Servo.h
 * @brief Servo motor control class
 * 
 * Controls standard servo motors with software-based speed control
 * Supports position control with simulated velocity
 */

#ifndef SERVO_H
#define SERVO_H

#include "../Actuator.h"
#include <Servo.h>

/**
 * @class ServoMotor
 * @brief Controls a standard servo motor
 * 
 * Provides position control with software-based speed limiting
 * Velocity control is simulated through time-based interpolation
 */
class ServoMotor : public Actuator {
private:
    Servo servo;                // Arduino Servo instance
    int servoPin;               // Servo control pin
    float minAngle;             // Minimum angle (degrees)
    float maxAngle;             // Maximum angle (degrees)
    float currentAngleDeg;      // Current angle in degrees
    float targetAngleDeg;       // Target angle in degrees
    float angleSpeed;           // Speed in degrees/sec
    unsigned long lastUpdateMs; // Last position update time
    bool attached;              // Is servo attached?
    
    // For speed-based movement
    float startAngle;           // Starting angle for interpolation
    unsigned long moveStartTime; // Movement start time
    float moveDuration;         // Expected movement duration
    bool speedMode;             // Using speed-based movement
    
public:
    /**
     * @brief Constructor
     * @param name Device name
     * @param pin Servo control pin
     * @param minAng Minimum angle (default 0)
     * @param maxAng Maximum angle (default 180)
     */
    ServoMotor(const String& name, int pin, float minAng = 0.0, float maxAng = 180.0);
    
    /**
     * @brief Destructor
     */
    ~ServoMotor();
    
    /**
     * @brief Initialize the servo
     * @return true if successful
     */
    bool init() override;
    
    /**
     * @brief Update servo position (for speed-based movement)
     */
    void update() override;
    
    /**
     * @brief Stop the servo (stays at current position)
     */
    void stop() override;
    
    /**
     * @brief Reset servo to default position
     */
    void reset() override;
    
    /**
     * @brief Set target position
     * @param position Target position in radians
     * @return true if accepted
     */
    bool setPosition(float position) override;
    
    /**
     * @brief Set target velocity (simulated)
     * @param velocity Target velocity in rad/sec
     * @return true if accepted
     */
    bool setVelocity(float velocity) override;
    
    /**
     * @brief Get current position
     * @return Position in radians
     */
    float getPosition() const override;
    
    /**
     * @brief Get current velocity
     * @return Velocity in rad/sec (calculated)
     */
    float getVelocity() const override;
    
    /**
     * @brief Set servo angle limits
     * @param minAng Minimum angle in degrees
     * @param maxAng Maximum angle in degrees
     */
    void setAngleLimits(float minAng, float maxAng);
    
    /**
     * @brief Get minimum angle
     * @return Minimum angle in degrees
     */
    float getMinAngle() const { return minAngle; }
    
    /**
     * @brief Get maximum angle
     * @return Maximum angle in degrees
     */
    float getMaxAngle() const { return maxAngle; }
    
    /**
     * @brief Enable the servo
     */
    void enable() override;
    
    /**
     * @brief Disable the servo (detach)
     */
    void disable() override;
    
    /**
     * @brief Get list of supported interfaces
     * @return Comma-separated list
     */
    String getInterfaces() const override {
        return "position,velocity,stop,reset,enable,disable";
    }
    
private:
    /**
     * @brief Convert radians to degrees
     * @param rad Angle in radians
     * @return Angle in degrees
     */
    float radToDeg(float rad) const {
        return rad * 180.0 / PI;
    }
    
    /**
     * @brief Convert degrees to radians
     * @param deg Angle in degrees
     * @return Angle in radians
     */
    float degToRad(float deg) const {
        return deg * PI / 180.0;
    }
    
    /**
     * @brief Write angle to servo
     * @param angleDeg Angle in degrees
     */
    void writeAngle(float angleDeg);
    
    /**
     * @brief Calculate interpolated position
     * @return Current interpolated angle in degrees
     */
    float calculateInterpolatedPosition();
};

#endif // SERVO_H