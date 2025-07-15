/**
 * @file main.cpp
 * @brief Main entry point for RAMPS Universal Controller
 * 
 * Initializes the system and runs the main control loop
 */

#include <Arduino.h>
#include "Config.h"
#include "core/Controller.h"
#include "core/Interface.h"

// Global interface instance
Interface* interface = nullptr;

/**
 * @brief Arduino setup function
 */
void setup() {
    // Initialize LED for status indication
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);  // LED on during init
    
    // Create interface
    interface = new Interface(&controller);
    
    // Initialize interface (serial communication)
    if (!interface->init(SERIAL_BAUD_RATE)) {
        // Fatal error - blink LED rapidly
        while (true) {
            digitalWrite(LED_PIN, !digitalRead(LED_PIN));
            delay(100);
        }
    }
    
    // Send startup message
    interface->sendStartupMessage();
    
    // Initialize controller
    if (!controller.init()) {
        interface->sendMessage("ERROR: Controller initialization failed!");
        // Fatal error - blink LED slowly
        while (true) {
            digitalWrite(LED_PIN, !digitalRead(LED_PIN));
            delay(500);
        }
    }
    
    // Set interface in controller
    controller.setInterface(interface);
    
    // Initialization complete
    interface->sendMessage("Initialization complete!");
    interface->sendMessage("");  // Blank line
    
    // Turn off LED to indicate ready
    digitalWrite(LED_PIN, LOW);
}

/**
 * @brief Arduino main loop
 */
void loop() {
    // Update interface (process commands)
    interface->update();
    
    // Update controller (update all devices)
    controller.update();
    
    // Small delay to prevent CPU hogging
    if (MAIN_LOOP_DELAY > 0) {
        delay(MAIN_LOOP_DELAY);
    }
    
    // Heartbeat LED (optional)
    static unsigned long lastBlink = 0;
    if (millis() - lastBlink > 2000) {
        digitalWrite(LED_PIN, HIGH);
        delay(50);
        digitalWrite(LED_PIN, LOW);
        lastBlink = millis();
    }
}

/**
 * @brief Handle emergency stop interrupt (future implementation)
 * 
 * Could be connected to a physical E-stop button
 */
void emergencyStopISR() {
    controller.emergencyStopAll();
}