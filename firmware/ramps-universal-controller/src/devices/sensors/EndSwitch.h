/**
 * @file EndSwitch.h
 * @brief End switch/limit switch sensor class
 * 
 * Monitors digital switches with debouncing and event reporting
 */

#ifndef END_SWITCH_H
#define END_SWITCH_H

#include "../Sensor.h"

/**
 * @class EndSwitch
 * @brief Digital switch sensor with debouncing
 * 
 * Monitors limit switches and can report state changes as events
 */
class EndSwitch : public Sensor {
private:
    int switchPin;              // Switch input pin
    bool inverted;              // Invert logic (true = triggered when LOW)
    bool pullupEnabled;         // Use internal pullup
    bool lastState;             // Previous debounced state
    bool currentState;          // Current raw state
    unsigned long lastDebounce; // Last debounce time
    bool stateChanged;          // Flag for state change detection
    
    // For event callback (future implementation)
    void (*changeCallback)(const String& deviceName, bool state);
    
public:
    /**
     * @brief Constructor
     * @param name Device name
     * @param pin Input pin
     * @param invert true if switch logic is inverted
     * @param pullup true to enable internal pullup
     */
    EndSwitch(const String& name, int pin, bool invert = false, bool pullup = true);
    
    /**
     * @brief Initialize the switch
     * @return true if successful
     */
    bool init() override;
    
    /**
     * @brief Update switch state (with debouncing)
     */
    void update() override;
    
    /**
     * @brief Read current switch value
     * @return 1.0 if triggered, 0.0 if not
     */
    float readValue() override;
    
    /**
     * @brief Check if switch is pressed/triggered
     * @return true if triggered
     */
    bool isPressed() const {
        return (currentValue > 0.5);
    }
    
    /**
     * @brief Check if state changed since last check
     * @return true if state changed
     */
    bool hasStateChanged() {
        bool changed = stateChanged;
        stateChanged = false;  // Clear flag
        return changed;
    }
    
    /**
     * @brief Get state as boolean
     * @return true if triggered
     */
    bool getState() const {
        return lastState;
    }
    
    /**
     * @brief Set change callback (for future use)
     * @param callback Function to call on state change
     */
    void setChangeCallback(void (*callback)(const String&, bool)) {
        changeCallback = callback;
    }
    
    /**
     * @brief Get list of supported interfaces
     * @return Comma-separated list
     */
    String getInterfaces() const override {
        return "read,state,value,status";
    }
    
    /**
     * @brief Get current status
     * @return Status string
     */
    String getStatus() const override;
    
private:
    /**
     * @brief Read raw switch state
     * @return Raw state (considering inversion)
     */
    bool readRawState();
};

#endif // END_SWITCH_H