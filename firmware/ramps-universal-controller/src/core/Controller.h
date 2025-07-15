/**
 * @file Controller.h
 * @brief Main controller class that manages all devices
 * 
 * Central controller for RAMPS board, manages all connected devices
 * and provides unified access and control
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include "Config.h"
#include "DeviceConfig.h"
#include "../devices/Device.h"
#include "../devices/Actuator.h"
#include "../devices/Sensor.h"
#include "Command.h"
#include "Reply.h"

// Forward declarations
class StepperMotor;
class ServoMotor;
class MosfetOutput;
class EndSwitch;
class AnalogSensor;
class Interface;

/**
 * @class Controller
 * @brief Central controller for all devices
 * 
 * Manages device initialization, updates, and command execution
 */
class Controller {
private:
    // Device arrays
    StepperMotor** steppers;
    ServoMotor** servos;
    MosfetOutput** mosfets;
    EndSwitch** switches;
    AnalogSensor** analogSensors;
    
    // Device counts
    int numSteppers;
    int numServos;
    int numMosfets;
    int numSwitches;
    int numAnalogSensors;
    
    // Interface for communication
    Interface* interface;
    
    // System state
    bool initialized;
    bool emergencyStop;
    unsigned long lastStatusTime;
    
public:
    /**
     * @brief Constructor
     */
    Controller();
    
    /**
     * @brief Destructor
     */
    ~Controller();
    
    /**
     * @brief Initialize the controller and all devices
     * @return true if successful
     */
    bool init();
    
    /**
     * @brief Update all devices (call in main loop)
     */
    void update();
    
    /**
     * @brief Execute a command
     * @param cmd Command to execute
     * @return Reply with result
     */
    Reply executeCommand(const Command& cmd);
    
    /**
     * @brief Get device by name
     * @param name Device name
     * @return Pointer to device or nullptr
     */
    Device* getDeviceByName(const String& name);
    
    /**
     * @brief Get all devices of a specific type
     * @param type Device type
     * @param devices Array to fill with device pointers
     * @param maxDevices Maximum array size
     * @return Number of devices found
     */
    int getDevicesByType(DeviceType type, Device** devices, int maxDevices);
    
    /**
     * @brief Get all devices in a group
     * @param groupName Group name (e.g., "STEPPERS")
     * @param devices Array to fill with device pointers
     * @param maxDevices Maximum array size
     * @return Number of devices found
     */
    int getDevicesByGroup(const String& groupName, Device** devices, int maxDevices);
    
    /**
     * @brief Stop all actuators
     */
    void stopAllActuators();
    
    /**
     * @brief Emergency stop all devices
     */
    void emergencyStopAll();
    
    /**
     * @brief Reset emergency stop
     */
    void resetEmergencyStop() { emergencyStop = false; }
    
    /**
     * @brief Check if emergency stop is active
     * @return true if e-stopped
     */
    bool isEmergencyStopped() const { return emergencyStop; }
    
    /**
     * @brief Get system status
     * @return Status string
     */
    String getSystemStatus() const;
    
    /**
     * @brief Get list of all devices
     * @return Device list string
     */
    String getDeviceList() const;
    
    /**
     * @brief Set interface handler
     * @param iface Interface instance
     */
    void setInterface(Interface* iface) { interface = iface; }
    
    /**
     * @brief Report event (for sensors)
     * @param device Device that triggered event
     * @param eventType Event type
     * @param value Event value
     */
    void reportEvent(const String& device, const String& eventType, const String& value);
    
private:
    /**
     * @brief Create and configure all devices
     */
    void createDevices();
    
    /**
     * @brief Initialize all devices
     * @return true if all successful
     */
    bool initializeDevices();
    
    /**
     * @brief Execute device-specific command
     * @param device Target device
     * @param cmd Command to execute
     * @return Reply with result
     */
    Reply executeDeviceCommand(Device* device, const Command& cmd);
    
    /**
     * @brief Execute system command
     * @param cmd Command to execute
     * @return Reply with result
     */
    Reply executeSystemCommand(const Command& cmd);
    
    /**
     * @brief Execute service command
     * @param cmd Command to execute
     * @return Reply with result
     */
    Reply executeServiceCommand(const Command& cmd);
    
    /**
     * @brief Handle switch state change
     * @param switchName Switch that changed
     * @param state New state
     */
    void handleSwitchChange(const String& switchName, bool state);
    
    /**
     * @brief Calibrate axis (homing)
     * @param axisName Axis to calibrate
     * @return true if successful
     */
    bool calibrateAxis(const String& axisName);
};

// Global controller instance
extern Controller controller;

#endif // CONTROLLER_H