/**
 * @file Controller.cpp
 * @brief Implementation of Controller class
 */

#include "Controller.h"
#include "Interface.h"
#include "../devices/actuators/StepperMotor.h"
#include "../devices/actuators/Servo.h"
#include "../devices/actuators/MosfetOutput.h"
#include "../devices/sensors/EndSwitch.h"
#include "../devices/sensors/AnalogSensor.h"
#include "PinDefinitions.h"

// Global controller instance
Controller controller;

/**
 * @brief Constructor
 */
Controller::Controller() {
    steppers = nullptr;
    servos = nullptr;
    mosfets = nullptr;
    switches = nullptr;
    analogSensors = nullptr;
    
    numSteppers = 0;
    numServos = 0;
    numMosfets = 0;
    numSwitches = 0;
    numAnalogSensors = 0;
    
    interface = nullptr;
    initialized = false;
    emergencyStop = false;
    lastStatusTime = 0;
}

/**
 * @brief Destructor
 */
Controller::~Controller() {
    // Delete all devices
    if (steppers) {
        for (int i = 0; i < numSteppers; i++) {
            if (steppers[i]) delete steppers[i];
        }
        delete[] steppers;
    }
    
    if (servos) {
        for (int i = 0; i < numServos; i++) {
            if (servos[i]) delete servos[i];
        }
        delete[] servos;
    }
    
    if (mosfets) {
        for (int i = 0; i < numMosfets; i++) {
            if (mosfets[i]) delete mosfets[i];
        }
        delete[] mosfets;
    }
    
    if (switches) {
        for (int i = 0; i < numSwitches; i++) {
            if (switches[i]) delete switches[i];
        }
        delete[] switches;
    }
    
    if (analogSensors) {
        for (int i = 0; i < numAnalogSensors; i++) {
            if (analogSensors[i]) delete analogSensors[i];
        }
        delete[] analogSensors;
    }
}

/**
 * @brief Initialize the controller
 */
bool Controller::init() {
    if (initialized) return true;
    
    // Create all devices based on configuration
    createDevices();
    
    // Initialize all devices
    if (!initializeDevices()) {
        return false;
    }
    
    initialized = true;
    return true;
}

/**
 * @brief Create and configure all devices
 */
void Controller::createDevices() {
    // Create stepper motors
    numSteppers = 0;
    #ifdef STEPPER_X_ENABLED
        numSteppers++;
    #endif
    #ifdef STEPPER_Y_ENABLED
        numSteppers++;
    #endif
    #ifdef STEPPER_Z_ENABLED
        numSteppers++;
    #endif
    
    if (numSteppers > 0) {
        steppers = new StepperMotor*[numSteppers];
        int idx = 0;
        
        #ifdef STEPPER_X_ENABLED
            steppers[idx++] = new StepperMotor(STEPPER_X_NAME, X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, STEPPER_X_STEPS_PER_REV);
        #endif
        #ifdef STEPPER_Y_ENABLED
            steppers[idx++] = new StepperMotor(STEPPER_Y_NAME, Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, STEPPER_Y_STEPS_PER_REV);
        #endif
        #ifdef STEPPER_Z_ENABLED
            steppers[idx++] = new StepperMotor(STEPPER_Z_NAME, Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, STEPPER_Z_STEPS_PER_REV);
        #endif
    }
    
    // Create servo motors
    numServos = 0;
    #ifdef SERVO_0_ENABLED
        numServos++;
    #endif
    #ifdef SERVO_1_ENABLED
        numServos++;
    #endif
    
    if (numServos > 0) {
        servos = new ServoMotor*[numServos];
        int idx = 0;
        
        #ifdef SERVO_0_ENABLED
            servos[idx++] = new ServoMotor(SERVO_0_NAME, SERVO0_PIN, SERVO_0_MIN_ANGLE, SERVO_0_MAX_ANGLE);
        #endif
        #ifdef SERVO_1_ENABLED
            servos[idx++] = new ServoMotor(SERVO_1_NAME, SERVO1_PIN, SERVO_1_MIN_ANGLE, SERVO_1_MAX_ANGLE);
        #endif
    }
    
    // Create MOSFET outputs
    numMosfets = 0;
    #ifdef MOSFET_A_ENABLED
        numMosfets++;
    #endif
    #ifdef MOSFET_B_ENABLED
        numMosfets++;
    #endif
    #ifdef MOSFET_C_ENABLED
        numMosfets++;
    #endif
    
    if (numMosfets > 0) {
        mosfets = new MosfetOutput*[numMosfets];
        int idx = 0;
        
        #ifdef MOSFET_A_ENABLED
            mosfets[idx++] = new MosfetOutput(MOSFET_A_NAME, MOSFET_A_PIN, MOSFET_A_PWM);
        #endif
        #ifdef MOSFET_B_ENABLED
            mosfets[idx++] = new MosfetOutput(MOSFET_B_NAME, MOSFET_B_PIN, MOSFET_B_PWM);
        #endif
        #ifdef MOSFET_C_ENABLED
            mosfets[idx++] = new MosfetOutput(MOSFET_C_NAME, MOSFET_C_PIN, MOSFET_C_PWM);
        #endif
    }
    
    // Create end switches
    numSwitches = 0;
    #ifdef SWITCH_X_MIN_ENABLED
        numSwitches++;
    #endif
    #ifdef SWITCH_Y_MIN_ENABLED
        numSwitches++;
    #endif
    #ifdef SWITCH_Z_MIN_ENABLED
        numSwitches++;
    #endif
    
    if (numSwitches > 0) {
        switches = new EndSwitch*[numSwitches];
        int idx = 0;
        
        #ifdef SWITCH_X_MIN_ENABLED
            switches[idx++] = new EndSwitch(SWITCH_X_MIN_NAME, X_MIN_PIN, SWITCH_X_MIN_INVERTED, SWITCH_PULLUP);
        #endif
        #ifdef SWITCH_Y_MIN_ENABLED
            switches[idx++] = new EndSwitch(SWITCH_Y_MIN_NAME, Y_MIN_PIN, SWITCH_Y_MIN_INVERTED, SWITCH_PULLUP);
        #endif
        #ifdef SWITCH_Z_MIN_ENABLED
            switches[idx++] = new EndSwitch(SWITCH_Z_MIN_NAME, Z_MIN_PIN, SWITCH_Z_MIN_INVERTED, SWITCH_PULLUP);
        #endif
    }
    
    // Create analog sensors
    numAnalogSensors = 0;
    #ifdef ANALOG_0_ENABLED
        numAnalogSensors++;
    #endif
    #ifdef ANALOG_1_ENABLED
        numAnalogSensors++;
    #endif
    
    if (numAnalogSensors > 0) {
        analogSensors = new AnalogSensor*[numAnalogSensors];
        int idx = 0;
        
        #ifdef ANALOG_0_ENABLED
            analogSensors[idx] = new AnalogSensor(ANALOG_0_NAME, ANALOG_0_PIN, ANALOG_0_MODE);
            // Configure thermistor if needed
            #if ANALOG_0_MODE == SENSOR_MODE_CUSTOM
                analogSensors[idx]->configureThermistor(ANALOG_0_R_PULLUP, ANALOG_0_THERMISTOR_R25, ANALOG_0_THERMISTOR_BETA);
            #endif
            idx++;
        #endif
        #ifdef ANALOG_1_ENABLED
            analogSensors[idx] = new AnalogSensor(ANALOG_1_NAME, ANALOG_1_PIN, ANALOG_1_MODE);
            idx++;
        #endif
    }
}

/**
 * @brief Initialize all devices
 */
bool Controller::initializeDevices() {
    bool success = true;
    
    // Initialize steppers
    for (int i = 0; i < numSteppers; i++) {
        if (steppers[i] && !steppers[i]->init()) {
            success = false;
        }
    }
    
    // Initialize servos
    for (int i = 0; i < numServos; i++) {
        if (servos[i] && !servos[i]->init()) {
            success = false;
        }
    }
    
    // Initialize MOSFETs
    for (int i = 0; i < numMosfets; i++) {
        if (mosfets[i] && !mosfets[i]->init()) {
            success = false;
        }
    }
    
    // Initialize switches
    for (int i = 0; i < numSwitches; i++) {
        if (switches[i] && !switches[i]->init()) {
            success = false;
        }
    }
    
    // Initialize analog sensors
    for (int i = 0; i < numAnalogSensors; i++) {
        if (analogSensors[i] && !analogSensors[i]->init()) {
            success = false;
        }
    }
    
    return success;
}

/**
 * @brief Update all devices
 */
void Controller::update() {
    if (!initialized || emergencyStop) return;
    
    // Update all actuators
    for (int i = 0; i < numSteppers; i++) {
        if (steppers[i]) steppers[i]->update();
    }
    
    for (int i = 0; i < numServos; i++) {
        if (servos[i]) servos[i]->update();
    }
    
    for (int i = 0; i < numMosfets; i++) {
        if (mosfets[i]) mosfets[i]->update();
    }
    
    // Update all sensors
    for (int i = 0; i < numSwitches; i++) {
        if (switches[i]) {
            switches[i]->update();
            
            // Check for state changes
            if (REPORT_SWITCH_EVENTS && switches[i]->hasStateChanged()) {
                handleSwitchChange(switches[i]->getName(), switches[i]->getState());
            }
        }
    }
    
    for (int i = 0; i < numAnalogSensors; i++) {
        if (analogSensors[i]) analogSensors[i]->update();
    }
}

/**
 * @brief Execute a command
 */
Reply Controller::executeCommand(const Command& cmd) {
    Reply reply;
    
    // Check for emergency stop
    if (emergencyStop && cmd.getCommandType() != CommandType::RESET) {
        reply.setError("", ERROR_DEVICE_BUSY, "Emergency stop active");
        return reply;
    }
    
    // Handle system commands
    if (cmd.getDeviceName() == "CONTROLLER" || cmd.getCommandType() == CommandType::LIST) {
        return executeSystemCommand(cmd);
    }
    
    // Handle service commands
    if (cmd.getCommandType() == CommandType::SERVICE) {
        return executeServiceCommand(cmd);
    }
    
    // Handle bulk commands
    if (cmd.getIsBulk()) {
        Device* devices[20];  // Max 20 devices in a group
        int count = getDevicesByGroup(cmd.getDeviceName(), devices, 20);
        
        if (count == 0) {
            reply.setError(cmd.getDeviceName(), ERROR_UNKNOWN_DEVICE, "Unknown group");
            return reply;
        }
        
        // Execute command on all devices in group
        for (int i = 0; i < count; i++) {
            executeDeviceCommand(devices[i], cmd);
        }
        
        reply.setOK(cmd.getDeviceName(), cmd.getInterface());
        return reply;
    }
    
    // Handle individual device commands
    Device* device = getDeviceByName(cmd.getDeviceName());
    if (!device) {
        reply.setError(cmd.getDeviceName(), ERROR_UNKNOWN_DEVICE, "Unknown device");
        return reply;
    }
    
    return executeDeviceCommand(device, cmd);
}

/**
 * @brief Execute device-specific command
 */
Reply Controller::executeDeviceCommand(Device* device, const Command& cmd) {
    Reply reply;
    
    if (!device) {
        reply.setError("", ERROR_UNKNOWN_DEVICE, "Device is null");
        return reply;
    }
    
    // Determine device category by type
    DeviceType devType = device->getType();
    bool isActuator = (devType == DeviceType::STEPPER_MOTOR || 
                       devType == DeviceType::SERVO_MOTOR || 
                       devType == DeviceType::MOSFET_OUTPUT);
    bool isSensor = (devType == DeviceType::END_SWITCH || 
                     devType == DeviceType::ANALOG_SENSOR);
    
    // Handle actuator commands
    if (isActuator) {
        Actuator* actuator = static_cast<Actuator*>(device);
        
        switch (cmd.getCommandType()) {
            case CommandType::POSITION:
                if (cmd.getIsQuery()) {
                    reply.setValue(device->getName(), "position", String(actuator->getPosition(), 3));
                } else {
                    if (actuator->setPosition(cmd.getNumericValue())) {
                        reply.setOK(device->getName(), "position", cmd.getValue());
                    } else {
                        reply.setError(device->getName(), ERROR_INVALID_PARAM, "Failed to set position");
                    }
                }
                break;
                
            case CommandType::VELOCITY:
                if (cmd.getIsQuery()) {
                    reply.setValue(device->getName(), "velocity", String(actuator->getVelocity(), 3));
                } else {
                    if (actuator->setVelocity(cmd.getNumericValue())) {
                        reply.setOK(device->getName(), "velocity", cmd.getValue());
                    } else {
                        reply.setError(device->getName(), ERROR_INVALID_PARAM, "Failed to set velocity");
                    }
                }
                break;
                
            case CommandType::STOP:
                actuator->stop();
                reply.setOK(device->getName(), "stop");
                break;
                
            case CommandType::ENABLE:
                actuator->enable();
                reply.setOK(device->getName(), "enable");
                break;
                
            case CommandType::DISABLE:
                actuator->disable();
                reply.setOK(device->getName(), "disable");
                break;
                
            case CommandType::ON:
                if (devType == DeviceType::MOSFET_OUTPUT) {
                    MosfetOutput* mosfet = static_cast<MosfetOutput*>(actuator);
                    mosfet->turnOn();
                    reply.setOK(device->getName(), "state", "ON");
                } else {
                    reply.setError(device->getName(), ERROR_UNKNOWN_COMMAND, "ON not supported");
                }
                break;
                
            case CommandType::OFF:
                if (devType == DeviceType::MOSFET_OUTPUT) {
                    MosfetOutput* mosfet = static_cast<MosfetOutput*>(actuator);
                    mosfet->turnOff();
                    reply.setOK(device->getName(), "state", "OFF");
                } else {
                    reply.setError(device->getName(), ERROR_UNKNOWN_COMMAND, "OFF not supported");
                }
                break;
                
            default:
                // Check for device-specific commands by interface name
                if (cmd.getInterface() == "acceleration" || cmd.getInterface() == "accel") {
                    if (cmd.getIsQuery()) {
                        reply.setValue(device->getName(), "acceleration", String(actuator->getAcceleration(), 3));
                    } else {
                        actuator->setAcceleration(cmd.getNumericValue());
                        reply.setOK(device->getName(), "acceleration", cmd.getValue());
                    }
                } else if ((cmd.getInterface() == "zero" || cmd.getInterface() == "setzero") && devType == DeviceType::STEPPER_MOTOR) {
                    StepperMotor* stepper = static_cast<StepperMotor*>(actuator);
                    stepper->setZeroPosition();
                    reply.setOK(device->getName(), "zero");
                } else {
                    reply.setError(device->getName(), ERROR_UNKNOWN_COMMAND, "Unknown command: " + cmd.getInterface());
                }
                break;
        }
        return reply;
    }
    
    // Handle sensor commands
    if (isSensor) {
        Sensor* sensor = static_cast<Sensor*>(device);
        
        switch (cmd.getCommandType()) {
            case CommandType::GET:
            case CommandType::READ:
                reply.setValue(device->getName(), "value", String(sensor->readValue(), 2));
                break;
                
            case CommandType::STATE:
                if (devType == DeviceType::END_SWITCH) {
                    EndSwitch* sw = static_cast<EndSwitch*>(sensor);
                    reply.setValue(device->getName(), "state", sw->getState() ? "1" : "0");
                } else {
                    reply.setValue(device->getName(), "value", String(sensor->getValue(), 2));
                }
                break;
                
            default:
                reply.setError(device->getName(), ERROR_UNKNOWN_COMMAND, "Unknown sensor command");
                break;
        }
        return reply;
    }
    
    // Handle generic device commands
    switch (cmd.getCommandType()) {
        case CommandType::STATUS:
            reply.setInfo(device->getStatus());
            break;
            
        case CommandType::RESET:
            device->reset();
            reply.setOK(device->getName(), "reset");
            break;
            
        default:
            reply.setError(device->getName(), ERROR_UNKNOWN_COMMAND, "Unknown command");
            break;
    }
    
    return reply;
}

/**
 * @brief Execute system command
 */
Reply Controller::executeSystemCommand(const Command& cmd) {
    Reply reply;
    
    switch (cmd.getCommandType()) {
        case CommandType::LIST:
            reply.setInfo(getDeviceList());
            break;
            
        case CommandType::STATUS:
            reply.setInfo(getSystemStatus());
            break;
            
        case CommandType::CMD_PING:
            reply.setInfo("PONG");
            break;
            
        case CommandType::ESTOP:
            emergencyStopAll();
            reply.setOK("CONTROLLER", "ESTOP");
            break;
            
        case CommandType::RESET:
            resetEmergencyStop();
            reply.setOK("CONTROLLER", "reset");
            break;
            
        default:
            reply.setError("CONTROLLER", ERROR_UNKNOWN_COMMAND, "Unknown system command");
            break;
    }
    
    return reply;
}

/**
 * @brief Execute service command
 */
Reply Controller::executeServiceCommand(const Command& cmd) {
    Reply reply;
    
    // Parse service type from value
    String service = cmd.getValue();
    service.toUpperCase();
    
    if (SERVICE_NOTIFY_START && interface) {
        Reply startReply;
        startReply.setInfo("SERVICE " + service + " STARTED");
        interface->sendReply(startReply);
    }
    
    bool success = false;
    
    if (service == "CALIBRATE_X") {
        success = calibrateAxis("X");
    } else if (service == "CALIBRATE_Y") {
        success = calibrateAxis("Y");
    } else if (service == "CALIBRATE_Z") {
        success = calibrateAxis("Z");
    } else if (service == "CALIBRATE_ALL") {
        success = calibrateAxis("X") && 
                  calibrateAxis("Y") && 
                  calibrateAxis("Z");
    } else if (service == "FULL_STATUS") {
        reply.setInfo(getSystemStatus() + "\n" + getDeviceList());
        return reply;
    } else if (service == "ESTOP") {
        emergencyStopAll();
        success = true;
    } else {
        reply.setError("SERVICE", ERROR_UNKNOWN_COMMAND, "Unknown service: " + service);
        return reply;
    }
    
    if (SERVICE_NOTIFY_DONE) {
        reply.setInfo("SERVICE " + service + (success ? " DONE" : " FAILED"));
    } else {
        reply.setOK("SERVICE", service);
    }
    
    return reply;
}

/**
 * @brief Get device by name
 */
Device* Controller::getDeviceByName(const String& name) {
    // Search steppers
    for (int i = 0; i < numSteppers; i++) {
        if (steppers[i] && steppers[i]->getName() == name) {
            return steppers[i];
        }
    }
    
    // Search servos
    for (int i = 0; i < numServos; i++) {
        if (servos[i] && servos[i]->getName() == name) {
            return servos[i];
        }
    }
    
    // Search MOSFETs
    for (int i = 0; i < numMosfets; i++) {
        if (mosfets[i] && mosfets[i]->getName() == name) {
            return mosfets[i];
        }
    }
    
    // Search switches
    for (int i = 0; i < numSwitches; i++) {
        if (switches[i] && switches[i]->getName() == name) {
            return switches[i];
        }
    }
    
    // Search analog sensors
    for (int i = 0; i < numAnalogSensors; i++) {
        if (analogSensors[i] && analogSensors[i]->getName() == name) {
            return analogSensors[i];
        }
    }
    
    return nullptr;
}

/**
 * @brief Get devices by group
 */
int Controller::getDevicesByGroup(const String& groupName, Device** devices, int maxDevices) {
    int count = 0;
    
    if (groupName == GROUP_ALL_STEPPERS) {
        for (int i = 0; i < numSteppers && count < maxDevices; i++) {
            if (steppers[i]) devices[count++] = steppers[i];
        }
    } else if (groupName == GROUP_ALL_SERVOS) {
        for (int i = 0; i < numServos && count < maxDevices; i++) {
            if (servos[i]) devices[count++] = servos[i];
        }
    } else if (groupName == GROUP_ALL_OUTPUTS) {
        for (int i = 0; i < numMosfets && count < maxDevices; i++) {
            if (mosfets[i]) devices[count++] = mosfets[i];
        }
    } else if (groupName == GROUP_ALL_SWITCHES) {
        for (int i = 0; i < numSwitches && count < maxDevices; i++) {
            if (switches[i]) devices[count++] = switches[i];
        }
    } else if (groupName == GROUP_ALL_SENSORS) {
        for (int i = 0; i < numSwitches && count < maxDevices; i++) {
            if (switches[i]) devices[count++] = switches[i];
        }
        for (int i = 0; i < numAnalogSensors && count < maxDevices; i++) {
            if (analogSensors[i]) devices[count++] = analogSensors[i];
        }
    } else if (groupName == GROUP_ALL_ACTUATORS) {
        for (int i = 0; i < numSteppers && count < maxDevices; i++) {
            if (steppers[i]) devices[count++] = steppers[i];
        }
        for (int i = 0; i < numServos && count < maxDevices; i++) {
            if (servos[i]) devices[count++] = servos[i];
        }
        for (int i = 0; i < numMosfets && count < maxDevices; i++) {
            if (mosfets[i]) devices[count++] = mosfets[i];
        }
    }
    
    return count;
}

/**
 * @brief Stop all actuators
 */
void Controller::stopAllActuators() {
    for (int i = 0; i < numSteppers; i++) {
        if (steppers[i]) steppers[i]->stop();
    }
    
    for (int i = 0; i < numServos; i++) {
        if (servos[i]) servos[i]->stop();
    }
    
    for (int i = 0; i < numMosfets; i++) {
        if (mosfets[i]) mosfets[i]->stop();
    }
}

/**
 * @brief Emergency stop all devices
 */
void Controller::emergencyStopAll() {
    emergencyStop = true;
    
    // Immediately stop all steppers
    for (int i = 0; i < numSteppers; i++) {
        if (steppers[i]) steppers[i]->emergencyStop();
    }
    
    // Stop servos
    for (int i = 0; i < numServos; i++) {
        if (servos[i]) servos[i]->stop();
    }
    
    // Turn off all outputs
    for (int i = 0; i < numMosfets; i++) {
        if (mosfets[i]) mosfets[i]->turnOff();
    }
}

/**
 * @brief Get system status
 */
String Controller::getSystemStatus() const {
    String status = "=== SYSTEM STATUS ===\n";
    status += "Controller: " + String(initialized ? "INITIALIZED" : "NOT INITIALIZED");
    status += ", E-Stop: " + String(emergencyStop ? "ACTIVE" : "INACTIVE");
    status += "\nDevices: ";
    status += String(numSteppers) + " steppers, ";
    status += String(numServos) + " servos, ";
    status += String(numMosfets) + " outputs, ";
    status += String(numSwitches) + " switches, ";
    status += String(numAnalogSensors) + " analog sensors";
    status += "\nUptime: " + String(millis() / 1000) + " seconds";
    
    return status;
}

/**
 * @brief Get list of all devices
 */
String Controller::getDeviceList() const {
    String list = "=== DEVICE LIST ===\n";
    
    // List steppers
    for (int i = 0; i < numSteppers; i++) {
        if (steppers[i]) {
            list += "- " + steppers[i]->getName() + " (" + steppers[i]->getTypeString() + "): ";
            list += "interfaces [" + steppers[i]->getInterfaces() + "]\n";
            list += "  Commands: >" + steppers[i]->getName() + " enable | position <rad> | velocity <rad/s> | acceleration <rad/s²> | zero | stop\n";
        }
    }
    
    // List servos
    for (int i = 0; i < numServos; i++) {
        if (servos[i]) {
            list += "- " + servos[i]->getName() + " (" + servos[i]->getTypeString() + "): ";
            list += "interfaces [" + servos[i]->getInterfaces() + "]\n";
            list += "  Commands: >" + servos[i]->getName() + " position <rad> | velocity <rad/s> | stop\n";
        }
    }
    
    // List MOSFETs
    for (int i = 0; i < numMosfets; i++) {
        if (mosfets[i]) {
            list += "- " + mosfets[i]->getName() + " (" + mosfets[i]->getTypeString() + "): ";
            list += "interfaces [" + mosfets[i]->getInterfaces() + "]\n";
            list += "  Commands: >" + mosfets[i]->getName() + " ON | OFF | position <0-1> | velocity <change/s>\n";
        }
    }
    
    // List switches
    for (int i = 0; i < numSwitches; i++) {
        if (switches[i]) {
            list += "- " + switches[i]->getName() + " (" + switches[i]->getTypeString() + "): ";
            list += "interfaces [" + switches[i]->getInterfaces() + "]\n";
            list += "  Commands: >" + switches[i]->getName() + " read | state?\n";
        }
    }
    
    // List analog sensors
    for (int i = 0; i < numAnalogSensors; i++) {
        if (analogSensors[i]) {
            list += "- " + analogSensors[i]->getName() + " (" + analogSensors[i]->getTypeString() + "): ";
            list += "interfaces [" + analogSensors[i]->getInterfaces() + "]\n";
            list += "  Commands: >" + analogSensors[i]->getName() + " read | value?\n";
        }
    }
    
    list += "\nBulk commands: >STEPPERS velocity 0 | >SERVOS position 0 | >OUTPUTS OFF\n";
    list += "System: >CONTROLLER STATUS | PING | ESTOP\n";
    
    return list;
}

/**
 * @brief Handle switch state change
 */
void Controller::handleSwitchChange(const String& switchName, bool state) {
    if (interface) {
        Reply event(switchName);
        event.setEvent(switchName, "state", state ? "1" : "0");
        interface->sendReply(event);
    }
}

/**
 * @brief Calibrate axis (homing)
 */
bool Controller::calibrateAxis(const String& axisName) {
    // Find the stepper motor
    StepperMotor* stepper = nullptr;
    for (int i = 0; i < numSteppers; i++) {
        if (steppers[i] && steppers[i]->getName() == axisName) {
            stepper = steppers[i];
            break;
        }
    }
    
    if (!stepper) return false;
    
    // Find corresponding home switch
    EndSwitch* homeSwitch = nullptr;
    String switchName = "";
    
    if (axisName == STEPPER_X_NAME) {
        switchName = SWITCH_X_MIN_NAME;
    } else if (axisName == STEPPER_Y_NAME) {
        switchName = SWITCH_Y_MIN_NAME;
    } else if (axisName == STEPPER_Z_NAME) {
        switchName = SWITCH_Z_MIN_NAME;
    }
    
    for (int i = 0; i < numSwitches; i++) {
        if (switches[i] && switches[i]->getName() == switchName) {
            homeSwitch = switches[i];
            break;
        }
    }
    
    if (!homeSwitch) return false;
    
    // Enable stepper
    stepper->enable();
    
    // Move slowly towards home switch
    stepper->setVelocity(-CALIBRATION_SPEED / stepper->getStepsPerUnit());
    
    // Wait for switch to trigger
    unsigned long startTime = millis();
    while (!homeSwitch->isPressed() && (millis() - startTime) < CALIBRATION_TIMEOUT_MS) {
        stepper->update();
        homeSwitch->update();
        delay(1);
    }
    
    // Stop motor
    stepper->stop();
    
    // Check if we hit the switch
    if (!homeSwitch->isPressed()) {
        return false;  // Timeout
    }
    
    // Back off slightly
    stepper->setPosition(5.0 / stepper->getStepsPerUnit());  // 5 steps back
    while (!stepper->isAtTarget()) {
        stepper->update();
        delay(1);
    }
    
    // Set zero position
    stepper->setZeroPosition();
    
    return true;
}

/**
 * @brief Report event
 */
void Controller::reportEvent(const String& device, const String& eventType, const String& value) {
    if (interface) {
        Reply event(device);
        event.setEvent(device, eventType, value);
        interface->sendReply(event);
    }
}