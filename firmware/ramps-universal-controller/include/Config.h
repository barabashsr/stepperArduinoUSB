/**
 * @file Config.h
 * @brief Main configuration file for RAMPS Universal Controller
 * 
 * This file contains all user-configurable parameters including:
 * - Communication settings
 * - Device counts and configurations
 * - System behavior settings
 * - Feature enables/disables
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================
// COMMUNICATION SETTINGS
// ============================================
#define SERIAL_BAUD_RATE        115200  // USB serial baud rate
#define COMMAND_BUFFER_SIZE     128     // Maximum command length
#define COMMAND_TIMEOUT_MS      1000    // Timeout for incomplete commands
#define DEFAULT_ACK_MODE        true    // true = verbose (send ACK), false = quiet

// ============================================
// DEVICE CONFIGURATION
// ============================================
// Number of each device type
#define NUM_STEPPERS            3       // X, Y, Z axes
#define NUM_SERVOS              2       // Servo motors
#define NUM_MOSFETS             3       // MOSFET outputs (heaters/fans)
#define NUM_ENDSWITCHES         6       // Limit switches
#define NUM_ANALOG_SENSORS      3       // Thermistors/analog inputs

// ============================================
// MOTION SETTINGS
// ============================================
// Default motion parameters
#define DEFAULT_MAX_SPEED       1000.0  // steps/sec or rad/sec
#define DEFAULT_ACCELERATION    500.0   // steps/sec^2 or rad/sec^2
#define DEFAULT_MIN_SPEED       1.0     // Minimum speed

// Servo settings
#define SERVO_MIN_ANGLE         0       // degrees
#define SERVO_MAX_ANGLE         180     // degrees
#define SERVO_DEFAULT_SPEED     1.0     // rad/sec
#define SERVO_UPDATE_INTERVAL   20      // ms between position updates

// ============================================
// SENSOR SETTINGS
// ============================================
// Analog sensor configuration
#define ANALOG_RESOLUTION       10      // bits (10 for Arduino)
#define ANALOG_MAX_VALUE        1023    // 2^10 - 1
#define ANALOG_SMOOTHING        true    // Enable averaging
#define ANALOG_SAMPLES          5       // Number of samples to average

// Sensor value modes
#define SENSOR_MODE_RAW         0       // Raw ADC values
#define SENSOR_MODE_VOLTAGE     1       // Converted to voltage
#define SENSOR_MODE_CUSTOM      2       // Custom conversion
#define DEFAULT_SENSOR_MODE     SENSOR_MODE_RAW

// Placeholder for future threshold support
#define ENABLE_THRESHOLDS       false
#define DEFAULT_THRESHOLD       512     // Middle of ADC range

// ============================================
// SYSTEM BEHAVIOR
// ============================================
// Event reporting
#define REPORT_SWITCH_EVENTS    true    // Auto-report switch state changes
#define SWITCH_DEBOUNCE_MS      50      // Debounce time for switches

// Safety features
#define ENABLE_ESTOP            true    // Emergency stop functionality
#define ESTOP_STOPS_ALL         true    // E-stop affects all devices

// Service command settings
#define SERVICE_NOTIFY_START    true    // Send notification when service starts
#define SERVICE_NOTIFY_DONE     true    // Send notification when service completes
#define CALIBRATION_SPEED       100.0   // Speed for homing/calibration
#define CALIBRATION_TIMEOUT_MS  30000   // Maximum time for calibration

// ============================================
// DEBUG SETTINGS
// ============================================
#define DEBUG_ENABLED           true
#define DEBUG_LEVEL             2       // 0=off, 1=errors, 2=warnings, 3=info

// ============================================
// FEATURE TOGGLES
// ============================================
#define ENABLE_JSON_MODE        false   // JSON protocol support (future)
#define ENABLE_DISPLAY          false   // LCD display support (future)
#define ENABLE_ENCODER          false   // Rotary encoder support (future)
#define ENABLE_SD_CARD          false   // SD card support (future)

// ============================================
// TIMING SETTINGS
// ============================================
#define MAIN_LOOP_DELAY         1       // ms delay in main loop (0 = no delay)
#define STATUS_UPDATE_INTERVAL  1000    // ms between status updates (if enabled)

// ============================================
// PROTOCOL SETTINGS
// ============================================
#define COMMAND_START_CHAR      '>'     // Command prefix character
#define COMMAND_DELIMITER       ' '     // Field separator in commands
#define COMMAND_TERMINATOR      '\n'    // End of command character
#define USE_START_MARKER        true    // Require start character

// ============================================
// ERROR CODES
// ============================================
enum ErrorCode {
    ERROR_NONE = 0,
    ERROR_UNKNOWN_DEVICE,
    ERROR_UNKNOWN_COMMAND,
    ERROR_INVALID_PARAM,
    ERROR_OUT_OF_RANGE,
    ERROR_DEVICE_BUSY,
    ERROR_TIMEOUT,
    ERROR_HARDWARE_FAULT,
    ERROR_NOT_IMPLEMENTED
};

#endif // CONFIG_H