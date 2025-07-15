/**
 * @file Command.h
 * @brief Command message class for incoming commands
 */

#ifndef COMMAND_H
#define COMMAND_H

#include "Message.h"
#include <Arduino.h>

/**
 * @enum CommandType
 * @brief Types of commands supported
 */
enum class CommandType {
    // Actuator commands
    POSITION,
    VELOCITY,
    STATE,
    ON,
    OFF,
    
    // Query commands
    GET,
    READ,
    STATUS,
    
    // Configuration commands
    CONFIG,
    CALIBRATE,
    RESET,
    
    // Control commands
    ENABLE,
    DISABLE,
    
    // System commands
    LIST,
    CMD_PING,    // Renamed from PING to avoid AVR macro conflict
    STOP,
    ESTOP,
    
    // Service commands
    SERVICE,
    
    // Unknown
    UNKNOWN
};

/**
 * @class Command
 * @brief Represents an incoming command
 */
class Command : public Message {
private:
    CommandType commandType;    // Type of command
    String interface;           // Interface name (position, velocity, etc.)
    String value;               // Command value/parameter
    bool isQuery;               // Is this a query command?
    bool isBulk;                // Is this a bulk command?
    
public:
    /**
     * @brief Constructor
     */
    Command();
    
    /**
     * @brief Parse command from input string
     * @param input Command string (with or without '>' prefix)
     * @return true if parsing successful
     */
    bool parse(const String& input) override;
    
    /**
     * @brief Convert command to string
     * @return Formatted command string
     */
    String toString() const override;
    
    /**
     * @brief Check if command is valid
     * @return true if command is properly formed
     */
    bool isValid() const override;
    
    /**
     * @brief Get command type
     * @return Command type enum
     */
    CommandType getCommandType() const { return commandType; }
    
    /**
     * @brief Get interface name
     * @return Interface string (position, velocity, etc.)
     */
    const String& getInterface() const { return interface; }
    
    /**
     * @brief Get command value
     * @return Value string
     */
    const String& getValue() const { return value; }
    
    /**
     * @brief Get numeric value
     * @return Value as float
     */
    float getNumericValue() const { return value.toFloat(); }
    
    /**
     * @brief Check if this is a query
     * @return true if query command
     */
    bool getIsQuery() const { return isQuery; }
    
    /**
     * @brief Check if this is a bulk command
     * @return true if bulk command
     */
    bool getIsBulk() const { return isBulk; }
    
private:
    /**
     * @brief Parse command type from interface string
     * @param interfaceStr Interface string
     * @return Command type
     */
    CommandType parseCommandType(const String& interfaceStr);
    
    /**
     * @brief Check if device name is a bulk group
     * @param name Device/group name
     * @return true if bulk group
     */
    bool isBulkGroup(const String& name);
};

#endif // COMMAND_H