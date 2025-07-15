/**
 * @file Reply.cpp
 * @brief Implementation of Reply class
 */

#include "Reply.h"

/**
 * @brief Constructor for standard reply
 */
Reply::Reply() : Message(MessageType::REPLY) {
    status = ReplyStatus::OK;
    interface = "";
    value = "";
    errorMessage = "";
    errorCode = ERROR_NONE;
    isEvent = false;
}

/**
 * @brief Constructor for event notification
 */
Reply::Reply(const String& eventDevice) : Message(MessageType::EVENT) {
    deviceName = eventDevice;
    status = ReplyStatus::EVENT;
    interface = "";
    value = "";
    errorMessage = "";
    errorCode = ERROR_NONE;
    isEvent = true;
}

/**
 * @brief Parse reply from string (mainly for testing)
 */
bool Reply::parse(const String& input) {
    rawText = input;
    // Basic parsing for testing - not fully implemented
    // as replies are typically generated, not parsed
    return true;
}

/**
 * @brief Convert reply to string for transmission
 */
String Reply::toString() const {
    String result = "";
    
    switch (status) {
        case ReplyStatus::OK:
            if (DEFAULT_ACK_MODE || value.length() > 0) {
                result = deviceName;
                if (interface.length() > 0) {
                    result += " " + interface;
                }
                if (value.length() > 0) {
                    result += " " + value;
                }
                result += " OK";
            }
            break;
            
        case ReplyStatus::ERROR:
            result = "ERROR: ";
            if (errorMessage.length() > 0) {
                result += errorMessage;
            } else {
                result += "Unknown error";
            }
            if (deviceName.length() > 0) {
                result += " (" + deviceName + ")";
            }
            break;
            
        case ReplyStatus::VALUE:
            result = deviceName;
            if (interface.length() > 0) {
                result += " " + interface;
            }
            result += " " + value;
            break;
            
        case ReplyStatus::EVENT:
            result = deviceName;
            if (interface.length() > 0) {
                result += " " + interface;
            }
            result += " " + value;
            if (isEvent) {
                result += " EVENT";
            }
            break;
            
        case ReplyStatus::INFO:
            result = value;  // Info replies are just the value
            break;
            
        case ReplyStatus::ACK:
            if (DEFAULT_ACK_MODE) {
                result = "ACK";
                if (deviceName.length() > 0) {
                    result += " " + deviceName;
                }
            }
            break;
    }
    
    return result;
}

/**
 * @brief Check if reply is valid
 */
bool Reply::isValid() const {
    switch (status) {
        case ReplyStatus::ERROR:
            return errorMessage.length() > 0;
        case ReplyStatus::INFO:
            return value.length() > 0;
        case ReplyStatus::VALUE:
        case ReplyStatus::EVENT:
            return deviceName.length() > 0 && value.length() > 0;
        default:
            return true;
    }
}

/**
 * @brief Set as OK/ACK reply
 */
void Reply::setOK(const String& device, const String& iface, const String& val) {
    status = ReplyStatus::OK;
    deviceName = device;
    interface = iface;
    value = val;
    errorCode = ERROR_NONE;
    isEvent = false;
}

/**
 * @brief Set as error reply
 */
void Reply::setError(const String& device, ErrorCode code, const String& message) {
    status = ReplyStatus::ERROR;
    deviceName = device;
    errorCode = code;
    errorMessage = message;
    isEvent = false;
    
    // Build error message if not provided
    if (errorMessage.length() == 0) {
        switch (code) {
            case ERROR_UNKNOWN_DEVICE:
                errorMessage = "Unknown device";
                break;
            case ERROR_UNKNOWN_COMMAND:
                errorMessage = "Unknown command";
                break;
            case ERROR_INVALID_PARAM:
                errorMessage = "Invalid parameter";
                break;
            case ERROR_OUT_OF_RANGE:
                errorMessage = "Value out of range";
                break;
            case ERROR_DEVICE_BUSY:
                errorMessage = "Device busy";
                break;
            case ERROR_TIMEOUT:
                errorMessage = "Operation timeout";
                break;
            case ERROR_HARDWARE_FAULT:
                errorMessage = "Hardware fault";
                break;
            case ERROR_NOT_IMPLEMENTED:
                errorMessage = "Not implemented";
                break;
            default:
                errorMessage = "Unknown error";
                break;
        }
        
        if (device.length() > 0) {
            errorMessage += " " + device;
        }
    }
}

/**
 * @brief Set as value reply
 */
void Reply::setValue(const String& device, const String& iface, const String& val) {
    status = ReplyStatus::VALUE;
    deviceName = device;
    interface = iface;
    value = val;
    errorCode = ERROR_NONE;
    isEvent = false;
}

/**
 * @brief Set as event notification
 */
void Reply::setEvent(const String& device, const String& iface, const String& val) {
    status = ReplyStatus::EVENT;
    deviceName = device;
    interface = iface;
    value = val;
    errorCode = ERROR_NONE;
    isEvent = true;
    type = MessageType::EVENT;
}

/**
 * @brief Set as info reply
 */
void Reply::setInfo(const String& info) {
    status = ReplyStatus::INFO;
    value = info;
    errorCode = ERROR_NONE;
    isEvent = false;
}