/**
 * @file Command.cpp
 * @brief Implementation of Command class
 */

#include "Command.h"
#include "DeviceConfig.h"

/**
 * @brief Constructor
 */
Command::Command() : Message(MessageType::COMMAND) {
    commandType = CommandType::UNKNOWN;
    interface = "";
    value = "";
    isQuery = false;
    isBulk = false;
}

/**
 * @brief Parse command from input string
 */
bool Command::parse(const String& input) {
    rawText = input;
    String workingText = trim(input);
    
    // Check for empty command
    if (workingText.length() == 0) {
        return false;
    }
    
    // Remove command start character if present
    if (USE_START_MARKER && workingText.startsWith(String(COMMAND_START_CHAR))) {
        workingText = workingText.substring(1);
        workingText = trim(workingText);
    }
    
    // Split command into parts
    String parts[4];  // Maximum 4 parts: device, interface, value, extra
    int partCount = splitString(workingText, COMMAND_DELIMITER, parts, 4);
    
    if (partCount < 1) {
        return false;
    }
    
    // Parse device name
    deviceName = trim(parts[0]);
    if (deviceName.length() == 0) {
        return false;
    }
    
    // Check if bulk command
    isBulk = isBulkGroup(deviceName);
    
    // Parse interface/command if present
    if (partCount >= 2) {
        interface = trim(parts[1]);
        interface.toLowerCase();  // Normalize to lowercase
        
        // Check for query markers
        if (interface.endsWith("?")) {
            isQuery = true;
            interface = interface.substring(0, interface.length() - 1);
        } else if (interface == "get" || interface == "read" || interface == "status") {
            isQuery = true;
        }
        
        // Parse command type
        commandType = parseCommandType(interface);
    } else {
        // Single word commands (e.g., "CONTROLLER LIST")
        interface = deviceName;
        interface.toLowerCase();
        commandType = parseCommandType(interface);
    }
    
    // Parse value if present
    if (partCount >= 3) {
        value = trim(parts[2]);
        
        // Handle special values
        if (value.equalsIgnoreCase("ON")) {
            commandType = CommandType::ON;
        } else if (value.equalsIgnoreCase("OFF")) {
            commandType = CommandType::OFF;
        }
    }
    
    return true;
}

/**
 * @brief Convert command to string
 */
String Command::toString() const {
    String result = "";
    
    if (USE_START_MARKER) {
        result += COMMAND_START_CHAR;
    }
    
    result += deviceName;
    
    if (interface.length() > 0) {
        result += COMMAND_DELIMITER;
        result += interface;
        
        if (isQuery) {
            result += "?";
        }
        
        if (value.length() > 0) {
            result += COMMAND_DELIMITER;
            result += value;
        }
    }
    
    return result;
}

/**
 * @brief Check if command is valid
 */
bool Command::isValid() const {
    return (deviceName.length() > 0) && (commandType != CommandType::UNKNOWN);
}

/**
 * @brief Parse command type from interface string
 */
CommandType Command::parseCommandType(const String& interfaceStr) {
    if (interfaceStr == "position" || interfaceStr == "pos") {
        return CommandType::POSITION;
    } else if (interfaceStr == "velocity" || interfaceStr == "vel" || interfaceStr == "speed") {
        return CommandType::VELOCITY;
    } else if (interfaceStr == "state") {
        return CommandType::STATE;
    } else if (interfaceStr == "on") {
        return CommandType::ON;
    } else if (interfaceStr == "off") {
        return CommandType::OFF;
    } else if (interfaceStr == "get" || interfaceStr == "read") {
        return CommandType::GET;
    } else if (interfaceStr == "status") {
        return CommandType::STATUS;
    } else if (interfaceStr == "config" || interfaceStr == "configure") {
        return CommandType::CONFIG;
    } else if (interfaceStr == "calibrate" || interfaceStr == "home") {
        return CommandType::CALIBRATE;
    } else if (interfaceStr == "reset") {
        return CommandType::RESET;
    } else if (interfaceStr == "list") {
        return CommandType::LIST;
    } else if (interfaceStr == "ping") {
        return CommandType::CMD_PING;
    } else if (interfaceStr == "stop") {
        return CommandType::STOP;
    } else if (interfaceStr == "estop" || interfaceStr == "emergency") {
        return CommandType::ESTOP;
    } else if (interfaceStr == "service") {
        return CommandType::SERVICE;
    } else {
        return CommandType::UNKNOWN;
    }
}

/**
 * @brief Check if device name is a bulk group
 */
bool Command::isBulkGroup(const String& name) {
    return (name == GROUP_ALL_STEPPERS ||
            name == GROUP_ALL_SERVOS ||
            name == GROUP_ALL_OUTPUTS ||
            name == GROUP_ALL_SWITCHES ||
            name == GROUP_ALL_SENSORS ||
            name == GROUP_ALL_ACTUATORS ||
            name == GROUP_ALL_DEVICES);
}