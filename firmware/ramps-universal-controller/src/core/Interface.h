/**
 * @file Interface.h
 * @brief Serial communication interface class
 * 
 * Handles command parsing, execution, and reply formatting
 * over USB serial connection
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include <Arduino.h>
#include "Config.h"
#include "Command.h"
#include "Reply.h"

// Forward declaration
class Controller;

/**
 * @class Interface
 * @brief Manages serial communication and command processing
 * 
 * Provides command reception, parsing, execution, and reply transmission
 */
class Interface {
private:
    Controller* controller;         // Reference to controller
    String inputBuffer;             // Command input buffer
    unsigned long lastCharTime;     // Time of last received character
    bool ackMode;                   // Acknowledgment mode
    unsigned long commandCount;     // Total commands processed
    unsigned long errorCount;       // Total errors
    
public:
    /**
     * @brief Constructor
     * @param ctrl Reference to controller
     */
    Interface(Controller* ctrl);
    
    /**
     * @brief Initialize the interface
     * @param baudRate Serial baud rate
     * @return true if successful
     */
    bool init(unsigned long baudRate = SERIAL_BAUD_RATE);
    
    /**
     * @brief Update interface (process incoming data)
     */
    void update();
    
    /**
     * @brief Send a reply
     * @param reply Reply to send
     */
    void sendReply(const Reply& reply);
    
    /**
     * @brief Send raw message
     * @param message Message string
     */
    void sendMessage(const String& message);
    
    /**
     * @brief Set acknowledgment mode
     * @param enabled true to enable ACK mode
     */
    void setAckMode(bool enabled) { ackMode = enabled; }
    
    /**
     * @brief Get acknowledgment mode
     * @return true if ACK mode enabled
     */
    bool getAckMode() const { return ackMode; }
    
    /**
     * @brief Get command statistics
     * @return Statistics string
     */
    String getStatistics() const;
    
    /**
     * @brief Send startup message
     */
    void sendStartupMessage();
    
private:
    /**
     * @brief Process incoming serial data
     */
    void processSerialInput();
    
    /**
     * @brief Process complete command
     * @param commandStr Command string
     */
    void processCommand(const String& commandStr);
    
    /**
     * @brief Check for command timeout
     * @return true if timeout occurred
     */
    bool checkTimeout();
    
    /**
     * @brief Clear input buffer
     */
    void clearBuffer() { 
        inputBuffer = ""; 
        lastCharTime = millis(); 
    }
};

#endif // INTERFACE_H