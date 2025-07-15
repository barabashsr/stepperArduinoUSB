/**
 * @file Servo.cpp
 * @brief Implementation of ServoMotor class
 */

#include "Servo.h"

/**
 * @brief Constructor
 */
ServoMotor::ServoMotor(const String& name, int pin, float minAng, float maxAng)
    : Actuator(name, DeviceType::SERVO_MOTOR) {
    servoPin = pin;
    minAngle = minAng;
    maxAngle = maxAng;
    currentAngleDeg = (minAngle + maxAngle) / 2.0;  // Start at center
    targetAngleDeg = currentAngleDeg;
    angleSpeed = radToDeg(SERVO_DEFAULT_SPEED);  // Convert rad/sec to deg/sec
    lastUpdateMs = millis();
    attached = false;
    
    startAngle = currentAngleDeg;
    moveStartTime = millis();
    moveDuration = 0;
    speedMode = false;
    
    // Convert position to radians for base class
    currentPosition = degToRad(currentAngleDeg);
    targetPosition = currentPosition;
    maxVelocity = SERVO_DEFAULT_SPEED;  // rad/sec
}

/**
 * @brief Destructor
 */
ServoMotor::~ServoMotor() {
    if (attached) {
        servo.detach();
    }
}

/**
 * @brief Initialize the servo
 */
bool ServoMotor::init() {
    // Attach servo to pin
    servo.attach(servoPin);
    attached = true;
    
    // Set to initial position
    writeAngle(currentAngleDeg);
    
    // Initialize state
    state = DeviceState::IDLE;
    enabled = true;
    
    return true;
}

/**
 * @brief Update servo position
 */
void ServoMotor::update() {
    if (!enabled || !attached) return;
    
    unsigned long currentTime = millis();
    float deltaTime = (currentTime - lastUpdateMs) / 1000.0;  // Convert to seconds
    
    if (speedMode && moveDuration > 0) {
        // Speed-based movement using interpolation
        float newAngle = calculateInterpolatedPosition();
        
        if (newAngle != currentAngleDeg) {
            currentAngleDeg = newAngle;
            writeAngle(currentAngleDeg);
            currentPosition = degToRad(currentAngleDeg);
            
            // Calculate actual velocity
            float elapsed = (currentTime - moveStartTime) / 1000.0;
            if (elapsed > 0) {
                float angleChange = currentAngleDeg - startAngle;
                currentVelocity = degToRad(angleChange) / elapsed;
            }
            
            state = DeviceState::ACTIVE;
        } else {
            // Movement complete
            speedMode = false;
            currentVelocity = 0.0;
            state = DeviceState::IDLE;
        }
    } else if (deltaTime > SERVO_UPDATE_INTERVAL / 1000.0) {
        // Standard movement - check if we need to move
        if (abs(currentAngleDeg - targetAngleDeg) > 0.5) {
            // Move towards target at maximum speed
            float maxStep = angleSpeed * deltaTime;
            float diff = targetAngleDeg - currentAngleDeg;
            
            if (abs(diff) <= maxStep) {
                // Reached target
                currentAngleDeg = targetAngleDeg;
                currentVelocity = 0.0;
                state = DeviceState::IDLE;
            } else {
                // Move one step
                currentAngleDeg += (diff > 0) ? maxStep : -maxStep;
                currentVelocity = (diff > 0) ? maxVelocity : -maxVelocity;
                state = DeviceState::ACTIVE;
            }
            
            writeAngle(currentAngleDeg);
            currentPosition = degToRad(currentAngleDeg);
        } else {
            currentVelocity = 0.0;
            state = DeviceState::IDLE;
        }
        
        lastUpdateMs = currentTime;
    }
    
    updateTimestamp();
}

/**
 * @brief Stop the servo
 */
void ServoMotor::stop() {
    speedMode = false;
    targetAngleDeg = currentAngleDeg;
    targetPosition = currentPosition;
    currentVelocity = 0.0;
    targetVelocity = 0.0;
    state = DeviceState::IDLE;
}

/**
 * @brief Reset servo to default position
 */
void ServoMotor::reset() {
    float centerAngle = (minAngle + maxAngle) / 2.0;
    targetAngleDeg = centerAngle;
    targetPosition = degToRad(centerAngle);
    speedMode = false;
    angleSpeed = radToDeg(SERVO_DEFAULT_SPEED);
    Actuator::reset();
}

/**
 * @brief Set target position
 */
bool ServoMotor::setPosition(float position) {
    if (!enabled || !attached) return false;
    
    // Convert radians to degrees
    float angleDeg = radToDeg(position);
    
    // Constrain to limits
    angleDeg = constrainValue(angleDeg, minAngle, maxAngle);
    
    // Set up movement
    targetAngleDeg = angleDeg;
    targetPosition = degToRad(angleDeg);
    
    // Calculate movement time based on current speed setting
    float angleChange = abs(targetAngleDeg - currentAngleDeg);
    if (angleChange > 0 && angleSpeed > 0) {
        speedMode = true;
        startAngle = currentAngleDeg;
        moveStartTime = millis();
        moveDuration = (angleChange / angleSpeed) * 1000.0;  // Convert to ms
    } else {
        speedMode = false;
    }
    
    return true;
}

/**
 * @brief Set target velocity (simulated)
 */
bool ServoMotor::setVelocity(float velocity) {
    if (!enabled || !attached) return false;
    
    // For servos, velocity sets the movement speed
    float speedDegSec = abs(radToDeg(velocity));
    
    if (speedDegSec > 0) {
        angleSpeed = speedDegSec;
        targetVelocity = velocity;
        
        // If velocity is non-zero, move to limit in that direction
        if (velocity > 0) {
            setPosition(degToRad(maxAngle));
        } else {
            setPosition(degToRad(minAngle));
        }
    } else {
        // Zero velocity means stop
        stop();
    }
    
    return true;
}

/**
 * @brief Get current position
 */
float ServoMotor::getPosition() const {
    return degToRad(currentAngleDeg);
}

/**
 * @brief Get current velocity
 */
float ServoMotor::getVelocity() const {
    return currentVelocity;
}

/**
 * @brief Set servo angle limits
 */
void ServoMotor::setAngleLimits(float minAng, float maxAng) {
    minAngle = constrainValue(minAng, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
    maxAngle = constrainValue(maxAng, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
    
    // Ensure min < max
    if (minAngle > maxAngle) {
        float temp = minAngle;
        minAngle = maxAngle;
        maxAngle = temp;
    }
}

/**
 * @brief Enable the servo
 */
void ServoMotor::enable() {
    if (!attached) {
        servo.attach(servoPin);
        attached = true;
        writeAngle(currentAngleDeg);
    }
    enabled = true;
    state = DeviceState::IDLE;
}

/**
 * @brief Disable the servo
 */
void ServoMotor::disable() {
    stop();
    if (attached) {
        servo.detach();
        attached = false;
    }
    enabled = false;
    state = DeviceState::DISABLED;
}

/**
 * @brief Write angle to servo
 */
void ServoMotor::writeAngle(float angleDeg) {
    float clamped = angleDeg;
    if (clamped < minAngle) clamped = minAngle;
    if (clamped > maxAngle) clamped = maxAngle;
    servo.write((int)clamped);
}

/**
 * @brief Calculate interpolated position
 */
float ServoMotor::calculateInterpolatedPosition() {
    if (!speedMode || moveDuration <= 0) {
        return targetAngleDeg;
    }
    
    unsigned long elapsed = millis() - moveStartTime;
    
    if (elapsed >= moveDuration) {
        // Movement complete
        return targetAngleDeg;
    }
    
    // Linear interpolation
    float progress = (float)elapsed / moveDuration;
    float angleChange = targetAngleDeg - startAngle;
    return startAngle + (angleChange * progress);
}