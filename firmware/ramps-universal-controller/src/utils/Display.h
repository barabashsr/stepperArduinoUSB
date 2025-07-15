/**
 * @file Display.h
 * @brief LCD display handler class (placeholder)
 * 
 * Future implementation for RepRapDiscount Smart Controller
 * or similar LCD displays
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "../devices/Device.h"

/**
 * @class Display
 * @brief Placeholder class for LCD display support
 * 
 * Will handle menu system, status display, and local control
 */
class Display : public Device {
private:
    bool hasDisplay;        // Is display connected?
    int currentMenu;        // Current menu index
    String statusLine1;     // Status display line 1
    String statusLine2;     // Status display line 2
    
public:
    /**
     * @brief Constructor
     */
    Display();
    
    /**
     * @brief Initialize display
     * @return true if successful
     */
    bool init() override;
    
    /**
     * @brief Update display
     */
    void update() override;
    
    /**
     * @brief Stop/clear display
     */
    void stop() override;
    
    /**
     * @brief Print message to display
     * @param message Message to display
     * @param line Line number (0-3)
     */
    void print(const String& message, int line = 0);
    
    /**
     * @brief Clear display
     */
    void clear();
    
    /**
     * @brief Set status lines
     * @param line1 First status line
     * @param line2 Second status line
     */
    void setStatus(const String& line1, const String& line2);
    
    /**
     * @brief Show menu
     * @param menuIndex Menu to display
     */
    void showMenu(int menuIndex);
    
    /**
     * @brief Get supported interfaces
     * @return Interface list
     */
    String getInterfaces() const override {
        return "print,clear,status,menu";
    }
    
    /**
     * @brief Check if display is available
     * @return true if display connected
     */
    bool isAvailable() const { return hasDisplay; }
};

#endif // DISPLAY_H