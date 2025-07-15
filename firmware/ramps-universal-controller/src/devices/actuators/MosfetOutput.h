/**
 * @file MosfetOutput.h
 * @brief MOSFET output control class
 * 
 * Controls high-current MOSFET outputs on RAMPS
 * Supports PWM control for variable power
 */

#ifndef MOSFET_OUTPUT_H
#define MOSFET_OUTPUT_H

#include "../Actuator.h"

/**
 * @class MosfetOutput
 * @brief Controls a MOSFET output channel
 * 
 * Provides ON/OFF and PWM control for fans, heaters, lights, etc.
 * Position interface maps to PWM duty cycle (0.0-1.0)
 */
class MosfetOutput : public Actuator {
private:
    int outputPin;              // Output pin number
    bool supportsPWM;           // Does this pin support PWM?
    uint8_t currentPWM;         // Current PWM value (0-255)
    uint8_t targetPWM;          // Target PWM value (0-255)
    bool isOn;                  // Current ON/OFF state
    
public:
    /**
     * @brief Constructor
     * @param name Device name
     * @param pin Output pin
     * @param pwm true if pin supports PWM
     */
    MosfetOutput(const String& name, int pin, bool pwm = true);
    
    /**
     * @brief Initialize the output
     * @return true if successful
     */
    bool init() override;
    
    /**
     * @brief Update output state
     */
    void update() override;
    
    /**
     * @brief Stop/turn off the output
     */
    void stop() override;
    
    /**
     * @brief Reset to default state (OFF)
     */
    void reset() override;
    
    /**
     * @brief Set output level (0.0-1.0)
     * @param position Maps to duty cycle (0.0=OFF, 1.0=full ON)
     * @return true if accepted
     */
    bool setPosition(float position) override;
    
    /**
     * @brief Set fade velocity (PWM change per second)
     * @param velocity Rate of change (0.0-1.0 per second)
     * @return true if accepted
     */
    bool setVelocity(float velocity) override;
    
    /**
     * @brief Turn output ON (full power)
     * @return true if successful
     */
    bool turnOn();
    
    /**
     * @brief Turn output OFF
     * @return true if successful
     */
    bool turnOff();
    
    /**
     * @brief Set PWM duty cycle
     * @param duty Duty cycle (0-255)
     * @return true if successful
     */
    bool setPWM(uint8_t duty);
    
    /**
     * @brief Get current PWM value
     * @return PWM duty cycle (0-255)
     */
    uint8_t getPWM() const { return currentPWM; }
    
    /**
     * @brief Check if output is ON
     * @return true if ON (PWM > 0)
     */
    bool getIsOn() const { return isOn; }
    
    /**
     * @brief Get list of supported interfaces
     * @return Comma-separated list
     */
    String getInterfaces() const override {
        return "position,velocity,state,ON,OFF,stop,reset";
    }
    
    /**
     * @brief Get current status
     * @return Status string
     */
    String getStatus() const override;
    
private:
    /**
     * @brief Write output value
     * @param value PWM value (0-255)
     */
    void writeOutput(uint8_t value);
    
    /**
     * @brief Convert normalized value to PWM
     * @param normalized Value 0.0-1.0
     * @return PWM value 0-255
     */
    uint8_t normalizeToPWM(float normalized) const {
        float clamped = normalized;
        if (clamped < 0.0) clamped = 0.0;
        if (clamped > 1.0) clamped = 1.0;
        return (uint8_t)(clamped * 255.0);
    }
    
    /**
     * @brief Convert PWM to normalized value
     * @param pwm PWM value 0-255
     * @return Normalized value 0.0-1.0
     */
    float PWMToNormalized(uint8_t pwm) const {
        return (float)pwm / 255.0;
    }
};

#endif // MOSFET_OUTPUT_H