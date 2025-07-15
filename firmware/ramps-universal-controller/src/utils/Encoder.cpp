/**
 * @file Encoder.cpp
 * @brief Implementation of Encoder class (placeholder)
 */

#include "Encoder.h"
#include "Config.h"
#include "PinDefinitions.h"

/**
 * @brief Constructor
 */
Encoder::Encoder(int a, int b, int btn) : Device("Encoder", DeviceType::ENCODER) {
    // Use default pins if not specified
    pinA = (a >= 0) ? a : BTN_EN1;
    pinB = (b >= 0) ? b : BTN_EN2;
    pinButton = (btn >= 0) ? btn : BTN_ENC;
    
    position = 0;
    buttonPressed = false;
    lastPosition = 0;
    lastButtonState = false;
    lastEncoded = 0;
}

/**
 * @brief Initialize encoder
 */
bool Encoder::init() {
    #if ENABLE_ENCODER
        // Configure pins
        if (pinA >= 0 && pinB >= 0) {
            pinMode(pinA, INPUT_PULLUP);
            pinMode(pinB, INPUT_PULLUP);
            
            // Read initial state
            readEncoder();
            
            // TODO: Attach interrupts for encoder pins
            // attachInterrupt(digitalPinToInterrupt(pinA), encoderISR, CHANGE);
            // attachInterrupt(digitalPinToInterrupt(pinB), encoderISR, CHANGE);
        }
        
        if (pinButton >= 0) {
            pinMode(pinButton, INPUT_PULLUP);
            buttonPressed = !digitalRead(pinButton);  // Active low
            lastButtonState = buttonPressed;
        }
        
        state = DeviceState::IDLE;
        enabled = true;
    #else
        state = DeviceState::DISABLED;
        enabled = false;
    #endif
    
    return true;
}

/**
 * @brief Update encoder state
 */
void Encoder::update() {
    if (!enabled) return;
    
    #if ENABLE_ENCODER
        // Read encoder (if not using interrupts)
        readEncoder();
        
        // Read button state
        if (pinButton >= 0) {
            bool currentButton = !digitalRead(pinButton);  // Active low
            
            // Simple debouncing
            if (currentButton != lastButtonState) {
                delay(10);  // Debounce delay
                currentButton = !digitalRead(pinButton);
                
                if (currentButton != lastButtonState) {
                    buttonPressed = currentButton;
                    lastButtonState = currentButton;
                }
            }
        }
    #endif
    
    updateTimestamp();
}

/**
 * @brief Stop encoder
 */
void Encoder::stop() {
    // Nothing to stop for encoder
    state = DeviceState::IDLE;
}

/**
 * @brief Get position change since last read
 */
long Encoder::getPositionDelta() {
    long delta = position - lastPosition;
    lastPosition = position;
    return delta;
}

/**
 * @brief Check if button was clicked
 */
bool Encoder::wasButtonClicked() {
    static bool lastPressed = false;
    bool clicked = false;
    
    if (!lastPressed && buttonPressed) {
        // Button just pressed
        lastPressed = true;
    } else if (lastPressed && !buttonPressed) {
        // Button just released - this is a click
        clicked = true;
        lastPressed = false;
    }
    
    return clicked;
}

/**
 * @brief Read encoder pins
 */
void Encoder::readEncoder() {
    if (pinA < 0 || pinB < 0) return;
    
    // Read current state
    uint8_t MSB = digitalRead(pinA);  // Most significant bit
    uint8_t LSB = digitalRead(pinB);  // Least significant bit
    
    uint8_t encoded = (MSB << 1) | LSB;  // Convert to single value
    uint8_t sum = (lastEncoded << 2) | encoded;  // Add to previous value
    
    // Determine direction based on state transition
    // This is a simplified quadrature decoder
    if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
        position++;
    } else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
        position--;
    }
    
    lastEncoded = encoded;
}

/**
 * @brief Handle interrupt (for future implementation)
 */
void Encoder::handleInterrupt() {
    // This would be called from ISR
    readEncoder();
}