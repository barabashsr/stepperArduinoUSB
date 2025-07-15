/**
 * @file Interface.cpp
 * @brief Implementation of Interface class
 */

#include "Interface.h"
#include "Controller.h"

/**
 * @brief Constructor
 */
Interface::Interface(Controller* ctrl) {
    controller = ctrl;
    inputBuffer = "";
    lastCharTime = millis();
    ackMode = DEFAULT_ACK_MODE;
    commandCount = 0;
    errorCount = 0;
}

/**
 * @brief Initialize the interface
 */
bool Interface::init(unsigned long baudRate) {
    // Initialize serial port
    Serial.begin(baudRate);
    
    // Wait for serial port to be ready
    unsigned long startTime = millis();
    while (!Serial && (millis() - startTime) < 1000) {
        // Wait up to 1 second for serial
    }
    
    // Clear any garbage in buffer
    while (Serial.available()) {
        Serial.read();
    }
    
    clearBuffer();
    
    return true;
}

/**
 * @brief Update interface
 */
void Interface::update() {
    // Process incoming serial data
    processSerialInput();
    
    // Check for command timeout
    if (inputBuffer.length() > 0 && checkTimeout()) {
        if (DEBUG_ENABLED && DEBUG_LEVEL >= 2) {
            sendMessage("WARNING: Command timeout, buffer cleared");
        }
        clearBuffer();
    }
}

/**
 * @brief Process incoming serial data
 */
void Interface::processSerialInput() {
    while (Serial.available()) {
        char c = Serial.read();
        lastCharTime = millis();
        
        // Check for command terminator
        if (c == COMMAND_TERMINATOR) {
            if (inputBuffer.length() > 0) {
                processCommand(inputBuffer);
                clearBuffer();
            }
        } else if (c >= 32 && c < 127) {  // Printable ASCII
            // Add to buffer if not full
            if (inputBuffer.length() < COMMAND_BUFFER_SIZE - 1) {
                inputBuffer += c;
            } else {
                // Buffer overflow
                if (DEBUG_ENABLED && DEBUG_LEVEL >= 1) {
                    sendMessage("ERROR: Command buffer overflow");
                }
                errorCount++;
                clearBuffer();
            }
        }
        // Ignore other characters (CR, control chars, etc.)
    }
}

/**
 * @brief Process complete command
 */
void Interface::processCommand(const String& commandStr) {
    commandCount++;
    
    // Parse command
    Command cmd;
    if (!cmd.parse(commandStr)) {
        Reply reply;
        reply.setError("", ERROR_INVALID_PARAM, "Invalid command format");
        sendReply(reply);
        errorCount++;
        return;
    }
    
    // Execute command
    if (controller) {
        Reply reply = controller->executeCommand(cmd);
        
        // Send reply if not empty or if ACK mode is on
        if (reply.isValid() && (reply.toString().length() > 0 || ackMode)) {
            sendReply(reply);
        }
        
        // Count errors
        if (reply.getErrorCode() != ERROR_NONE) {
            errorCount++;
        }
    } else {
        Reply reply;
        reply.setError("", ERROR_HARDWARE_FAULT, "Controller not initialized");
        sendReply(reply);
        errorCount++;
    }
}

/**
 * @brief Send a reply
 */
void Interface::sendReply(const Reply& reply) {
    String replyStr = reply.toString();
    if (replyStr.length() > 0) {
        sendMessage(replyStr);
    }
}

/**
 * @brief Send raw message
 */
void Interface::sendMessage(const String& message) {
    Serial.println(message);
}

/**
 * @brief Check for command timeout
 */
bool Interface::checkTimeout() {
    return (millis() - lastCharTime) > COMMAND_TIMEOUT_MS;
}

/**
 * @brief Get command statistics
 */
String Interface::getStatistics() const {
    String stats = "Interface Statistics:\n";
    stats += "Commands processed: " + String(commandCount) + "\n";
    stats += "Errors: " + String(errorCount) + "\n";
    stats += "Error rate: ";
    if (commandCount > 0) {
        stats += String((float)errorCount * 100.0 / commandCount, 1) + "%";
    } else {
        stats += "N/A";
    }
    stats += "\nACK mode: " + String(ackMode ? "ON" : "OFF");
    
    return stats;
}

/**
 * @brief Send startup message
 */
void Interface::sendStartupMessage() {
    sendMessage("===========================================");
    sendMessage("RAMPS 1.4 Universal Controller");
    sendMessage("Firmware Version 1.0.0");
    sendMessage("===========================================");
    sendMessage("Ready for commands.");
    sendMessage("Type 'CONTROLLER LIST' for device list");
    sendMessage("Commands start with '" + String(COMMAND_START_CHAR) + "'");
    sendMessage("===========================================");
}