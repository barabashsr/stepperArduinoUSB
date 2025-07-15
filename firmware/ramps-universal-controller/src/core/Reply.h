/**
 * @file Reply.h
 * @brief Reply message class for outgoing responses
 */

#ifndef REPLY_H
#define REPLY_H

#include "Message.h"
#include "Config.h"

/**
 * @enum ReplyStatus
 * @brief Status codes for replies
 */
enum class ReplyStatus {
    OK,             // Command executed successfully
    ERROR,          // Command failed
    ACK,            // Command acknowledged
    VALUE,          // Value response
    EVENT,          // Event notification
    INFO            // Information response
};

/**
 * @class Reply
 * @brief Represents an outgoing reply or event
 */
class Reply : public Message {
private:
    ReplyStatus status;         // Reply status
    String interface;           // Interface name (for value replies)
    String value;               // Reply value/data
    String errorMessage;        // Error description
    ErrorCode errorCode;        // Error code
    bool isEvent;               // Is this an unsolicited event?
    
public:
    /**
     * @brief Constructor for standard reply
     */
    Reply();
    
    /**
     * @brief Constructor for event notification
     * @param eventDevice Device that triggered event
     */
    Reply(const String& eventDevice);
    
    /**
     * @brief Parse reply from string (for testing/simulation)
     * @param input Reply string
     * @return true if parsing successful
     */
    bool parse(const String& input) override;
    
    /**
     * @brief Convert reply to string for transmission
     * @return Formatted reply string
     */
    String toString() const override;
    
    /**
     * @brief Check if reply is valid
     * @return true if reply is properly formed
     */
    bool isValid() const override;
    
    /**
     * @brief Set as OK/ACK reply
     * @param device Device name
     * @param iface Interface name
     * @param val Value (optional)
     */
    void setOK(const String& device, const String& iface = "", const String& val = "");
    
    /**
     * @brief Set as error reply
     * @param device Device name
     * @param code Error code
     * @param message Error message
     */
    void setError(const String& device, ErrorCode code, const String& message);
    
    /**
     * @brief Set as value reply
     * @param device Device name
     * @param iface Interface name
     * @param val Value
     */
    void setValue(const String& device, const String& iface, const String& val);
    
    /**
     * @brief Set as event notification
     * @param device Device name
     * @param iface Interface name
     * @param val Event value
     */
    void setEvent(const String& device, const String& iface, const String& val);
    
    /**
     * @brief Set as info reply (for LIST, STATUS, etc.)
     * @param info Information string
     */
    void setInfo(const String& info);
    
    /**
     * @brief Get reply status
     * @return Reply status enum
     */
    ReplyStatus getStatus() const { return status; }
    
    /**
     * @brief Get error code
     * @return Error code
     */
    ErrorCode getErrorCode() const { return errorCode; }
    
    /**
     * @brief Check if this is an event
     * @return true if event notification
     */
    bool getIsEvent() const { return isEvent; }
};

#endif // REPLY_H