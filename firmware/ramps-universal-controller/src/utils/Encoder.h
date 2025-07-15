/**
 * @file Encoder.h
 * @brief Rotary encoder handler class (placeholder)
 * 
 * Future implementation for rotary encoder input
 * Used with LCD displays for menu navigation
 */

#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include "../devices/Device.h"

/**
 * @class Encoder
 * @brief Placeholder class for rotary encoder support
 * 
 * Will handle rotary encoder input for menu navigation
 * and manual control
 */
class Encoder : public Device {
private:
    int pinA;               // Encoder pin A
    int pinB;               // Encoder pin B
    int pinButton;          // Encoder button pin
    
    volatile long position; // Current position
    volatile bool buttonPressed; // Button state
    long lastPosition;      // Last reported position
    bool lastButtonState;   // Last button state
    
    // For encoder state tracking
    uint8_t lastEncoded;
    
public:
    /**
     * @brief Constructor
     * @param a Pin A
     * @param b Pin B
     * @param btn Button pin
     */
    Encoder(int a = -1, int b = -1, int btn = -1);
    
    /**
     * @brief Initialize encoder
     * @return true if successful
     */
    bool init() override;
    
    /**
     * @brief Update encoder state
     */
    void update() override;
    
    /**
     * @brief Stop encoder (no-op)
     */
    void stop() override;
    
    /**
     * @brief Get current position
     * @return Position value
     */
    long getPosition() const { return position; }
    
    /**
     * @brief Get position change since last read
     * @return Position delta
     */
    long getPositionDelta();
    
    /**
     * @brief Reset position to zero
     */
    void resetPosition() { position = 0; lastPosition = 0; }
    
    /**
     * @brief Check if button is pressed
     * @return true if pressed
     */
    bool isButtonPressed() const { return buttonPressed; }
    
    /**
     * @brief Check if button was clicked (press and release)
     * @return true if clicked
     */
    bool wasButtonClicked();
    
    /**
     * @brief Get supported interfaces
     * @return Interface list
     */
    String getInterfaces() const override {
        return "position,button,reset";
    }
    
    /**
     * @brief Interrupt service routine for encoder
     */
    void handleInterrupt();
    
private:
    /**
     * @brief Read encoder pins
     */
    void readEncoder();
};

#endif // ENCODER_H