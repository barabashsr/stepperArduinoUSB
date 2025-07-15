/**
 * @file EndSwitch.cpp
 * @brief Implementation of EndSwitch class
 */

#include "EndSwitch.h"

/**
 * @brief Constructor
 */
EndSwitch::EndSwitch(const String& name, int pin, bool invert, bool pullup)
    : Sensor(name, DeviceType::END_SWITCH) {
    switchPin = pin;
    inverted = invert;
    pullupEnabled = pullup;
    lastState = false;
    currentState = false;
    lastDebounce = 0;
    stateChanged = false;
    changeCallback = nullptr;
    
    // Set threshold for digital sensor (0.5)
    threshold = 0.5;
}

/**
 * @brief Initialize the switch
 */
bool EndSwitch::init() {
    // Configure pin
    if (pullupEnabled) {
        pinMode(switchPin, INPUT_PULLUP);
    } else {
        pinMode(switchPin, INPUT);
    }
    
    // Read initial state
    currentState = readRawState();
    lastState = currentState;
    updateValue(currentState ? 1.0 : 0.0);
    
    // Initialize state
    state = DeviceState::IDLE;
    enabled = true;
    
    return true;
}

/**
 * @brief Update switch state with debouncing
 */
void EndSwitch::update() {
    if (!enabled) return;
    
    // Read current raw state
    bool rawState = readRawState();
    
    // Check if state changed
    if (rawState != currentState) {
        // Reset debounce timer
        lastDebounce = millis();
        currentState = rawState;
    }
    
    // Check if debounce period has passed
    if ((millis() - lastDebounce) > SWITCH_DEBOUNCE_MS) {
        // State has been stable for debounce period
        if (currentState != lastState) {
            // State changed
            lastState = currentState;
            updateValue(currentState ? 1.0 : 0.0);
            stateChanged = true;
            
            // Call callback if registered
            if (changeCallback && REPORT_SWITCH_EVENTS) {
                changeCallback(name, currentState);
            }
            
            // Update state
            state = DeviceState::ACTIVE;
        } else {
            state = DeviceState::IDLE;
        }
    }
    
    updateTimestamp();
}

/**
 * @brief Read current switch value
 */
float EndSwitch::readValue() {
    // Force a fresh read
    update();
    return currentValue;
}

/**
 * @brief Get current status
 */
String EndSwitch::getStatus() const {
    String status = Device::getStatus();
    
    status += ", Switch: ";
    status += isPressed() ? "TRIGGERED" : "OPEN";
    
    if (inverted) {
        status += " (inverted)";
    }
    
    return status;
}

/**
 * @brief Read raw switch state
 */
bool EndSwitch::readRawState() {
    bool raw = digitalRead(switchPin);
    
    // Apply inversion if needed
    if (inverted) {
        raw = !raw;
    }
    
    return raw;
}