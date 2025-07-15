/**
 * @file Message.h
 * @brief Base class for communication messages
 * 
 * Abstract base class for Command and Reply messages
 * Provides common functionality for message handling
 */

#ifndef MESSAGE_H
#define MESSAGE_H

#include <Arduino.h>
#include "Config.h"

/**
 * @enum MessageType
 * @brief Types of messages in the system
 */
enum class MessageType {
    COMMAND,        // Incoming command
    REPLY,          // Outgoing reply
    EVENT,          // Unsolicited event notification
    ERROR           // Error message
};

/**
 * @class Message
 * @brief Abstract base class for all messages
 * 
 * Provides common structure and methods for Commands and Replies
 */
class Message {
protected:
    MessageType type;           // Type of message
    String deviceName;          // Target/source device name
    String rawText;             // Original message text
    unsigned long timestamp;    // Message timestamp

public:
    /**
     * @brief Constructor
     * @param msgType Type of message
     */
    Message(MessageType msgType);
    
    /**
     * @brief Virtual destructor
     */
    virtual ~Message() = default;
    
    /**
     * @brief Parse message from string
     * @param input Raw input string
     * @return true if parsing successful
     */
    virtual bool parse(const String& input) = 0;
    
    /**
     * @brief Convert message to string for transmission
     * @return Formatted message string
     */
    virtual String toString() const = 0;
    
    /**
     * @brief Get message type
     * @return Message type enum
     */
    MessageType getType() const { return type; }
    
    /**
     * @brief Get device name
     * @return Device name string
     */
    const String& getDeviceName() const { return deviceName; }
    
    /**
     * @brief Set device name
     * @param name Device name
     */
    void setDeviceName(const String& name) { deviceName = name; }
    
    /**
     * @brief Get raw message text
     * @return Original message string
     */
    const String& getRawText() const { return rawText; }
    
    /**
     * @brief Get message timestamp
     * @return Timestamp in milliseconds
     */
    unsigned long getTimestamp() const { return timestamp; }
    
    /**
     * @brief Check if message is valid
     * @return true if message is properly formed
     */
    virtual bool isValid() const = 0;
    
protected:
    /**
     * @brief Utility function to split string by delimiter
     * @param str String to split
     * @param delimiter Delimiter character
     * @param parts Array to store parts
     * @param maxParts Maximum number of parts
     * @return Number of parts found
     */
    static int splitString(const String& str, char delimiter, String parts[], int maxParts);
    
    /**
     * @brief Trim whitespace from string
     * @param str String to trim
     * @return Trimmed string
     */
    static String trim(const String& str);
};

#endif // MESSAGE_H