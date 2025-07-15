/**
 * @file Message.cpp
 * @brief Implementation of Message base class
 */

#include "Message.h"

/**
 * @brief Constructor
 */
Message::Message(MessageType msgType) : type(msgType), timestamp(millis()) {
    deviceName = "";
    rawText = "";
}

/**
 * @brief Split string by delimiter
 */
int Message::splitString(const String& str, char delimiter, String parts[], int maxParts) {
    int partCount = 0;
    int startIndex = 0;
    int endIndex = 0;
    
    // Skip leading delimiters
    while (startIndex < str.length() && str[startIndex] == delimiter) {
        startIndex++;
    }
    
    while (startIndex < str.length() && partCount < maxParts) {
        // Find next delimiter
        endIndex = str.indexOf(delimiter, startIndex);
        
        if (endIndex == -1) {
            // Last part
            parts[partCount] = str.substring(startIndex);
            partCount++;
            break;
        } else {
            // Extract part
            parts[partCount] = str.substring(startIndex, endIndex);
            partCount++;
            startIndex = endIndex + 1;
            
            // Skip consecutive delimiters
            while (startIndex < str.length() && str[startIndex] == delimiter) {
                startIndex++;
            }
        }
    }
    
        return partCount;
}

/**
 * @brief Trim whitespace from string
 */
String Message::trim(const String& str) {
    int start = 0;
    int end = str.length() - 1;
    
    // Find first non-whitespace character
    while (start <= end && isspace(str[start])) {
        start++;
    }
    
    // Find last non-whitespace character
    while (end >= start && isspace(str[end])) {
        end--;
    }
    
    // Return trimmed string
    if (start <= end) {
        return str.substring(start, end + 1);
    } else {
        return "";
    }
}