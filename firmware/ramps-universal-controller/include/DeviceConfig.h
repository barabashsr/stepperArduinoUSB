/**
 * @file DeviceConfig.h
 * @brief Device-specific configuration and naming
 * 
 * Configure which devices are connected and their names
 * Modify this file to match your specific hardware setup
 */

#ifndef DEVICE_CONFIG_H
#define DEVICE_CONFIG_H

#include "PinDefinitions.h"
#include "Config.h"

// ============================================
// STEPPER MOTOR CONFIGURATION
// ============================================
// Define which steppers are enabled (comment out unused)
#define STEPPER_X_ENABLED
#define STEPPER_Y_ENABLED
#define STEPPER_Z_ENABLED
// #define STEPPER_E0_ENABLED
// #define STEPPER_E1_ENABLED

// Stepper names (user-configurable)
#define STEPPER_X_NAME          "X"          // X axis motor
#define STEPPER_Y_NAME          "Y"          // Y axis motor
#define STEPPER_Z_NAME          "Z"          // Z axis motor
#define STEPPER_E0_NAME         "E0"         // Extruder 0 / E axis
#define STEPPER_E1_NAME         "E1"         // Extruder 1

// Stepper parameters (steps per revolution * microstepping)
#define STEPPER_X_STEPS_PER_REV     200 * 16    // 200 steps * 16 microsteps
#define STEPPER_Y_STEPS_PER_REV     200 * 16
#define STEPPER_Z_STEPS_PER_REV     200 * 16
#define STEPPER_E0_STEPS_PER_REV    200 * 16
#define STEPPER_E1_STEPS_PER_REV    200 * 16

// Maximum speeds (steps/sec)
#define STEPPER_X_MAX_SPEED         1000.0
#define STEPPER_Y_MAX_SPEED         1000.0
#define STEPPER_Z_MAX_SPEED         500.0
#define STEPPER_E0_MAX_SPEED        800.0
#define STEPPER_E1_MAX_SPEED        800.0

// ============================================
// SERVO CONFIGURATION
// ============================================
// Define which servos are enabled
#define SERVO_0_ENABLED
#define SERVO_1_ENABLED
// #define SERVO_2_ENABLED
// #define SERVO_3_ENABLED

// Servo names
#define SERVO_0_NAME            "Servo0"      // Servo on D11
#define SERVO_1_NAME            "Servo1"      // Servo on D6
#define SERVO_2_NAME            "Servo2"      // Servo on D5
#define SERVO_3_NAME            "Servo3"      // Servo on D4

// Servo parameters
#define SERVO_0_MIN_ANGLE       0
#define SERVO_0_MAX_ANGLE       180
#define SERVO_1_MIN_ANGLE       0
#define SERVO_1_MAX_ANGLE       180

// ============================================
// MOSFET OUTPUT CONFIGURATION
// ============================================
// All MOSFETs are typically enabled
#define MOSFET_A_ENABLED
#define MOSFET_B_ENABLED
#define MOSFET_C_ENABLED

// MOSFET names
#define MOSFET_A_NAME           "D10"        // D10 - typically hotend
#define MOSFET_B_NAME           "D9"         // D9 - typically fan
#define MOSFET_C_NAME           "D8"         // D8 - typically bed

// MOSFET capabilities (all support PWM on RAMPS)
#define MOSFET_A_PWM            true
#define MOSFET_B_PWM            true
#define MOSFET_C_PWM            true

// ============================================
// END SWITCH CONFIGURATION
// ============================================
// Define which switches are enabled
#define SWITCH_X_MIN_ENABLED
#define SWITCH_Y_MIN_ENABLED
#define SWITCH_Z_MIN_ENABLED
// #define SWITCH_X_MAX_ENABLED
// #define SWITCH_Y_MAX_ENABLED
// #define SWITCH_Z_MAX_ENABLED

// Switch names
#define SWITCH_X_MIN_NAME       "XMin"       // X min endstop
#define SWITCH_Y_MIN_NAME       "YMin"       // Y min endstop
#define SWITCH_Z_MIN_NAME       "ZMin"       // Z min endstop
#define SWITCH_X_MAX_NAME       "XMax"       // X max endstop
#define SWITCH_Y_MAX_NAME       "YMax"       // Y max endstop
#define SWITCH_Z_MAX_NAME       "ZMax"       // Z max endstop

// Switch configuration
#define SWITCH_X_MIN_INVERTED   false   // true if switch reads LOW when triggered
#define SWITCH_Y_MIN_INVERTED   false
#define SWITCH_Z_MIN_INVERTED   false
#define SWITCH_X_MAX_INVERTED   false
#define SWITCH_Y_MAX_INVERTED   false
#define SWITCH_Z_MAX_INVERTED   false

// ============================================
// ANALOG SENSOR CONFIGURATION
// ============================================
// Define which analog sensors are enabled
#define ANALOG_0_ENABLED
#define ANALOG_1_ENABLED
// #define ANALOG_2_ENABLED

// Analog sensor names
#define ANALOG_0_NAME           "T0"         // Thermistor 0 / Analog 0
#define ANALOG_1_NAME           "T1"         // Thermistor 1 / Analog 1
#define ANALOG_2_NAME           "T2"         // Thermistor 2 / Analog 2

// Sensor conversion modes
#define ANALOG_0_MODE           SENSOR_MODE_CUSTOM  // Will convert to temperature
#define ANALOG_1_MODE           SENSOR_MODE_VOLTAGE // Will convert to voltage
#define ANALOG_2_MODE           SENSOR_MODE_RAW     // Raw ADC values

// Sensor-specific parameters
#define ANALOG_0_R_PULLUP       4700    // Pullup resistor value for thermistor
#define ANALOG_0_THERMISTOR_R25 100000  // Thermistor resistance at 25°C
#define ANALOG_0_THERMISTOR_BETA 3950   // Thermistor beta value

// ============================================
// DEVICE COUNT VERIFICATION
// ============================================
// These should match NUM_* in Config.h
#define CONFIGURED_STEPPERS     3   // X, Y, Z
#define CONFIGURED_SERVOS       2   // Servo 0, 1
#define CONFIGURED_MOSFETS      3   // A, B, C
#define CONFIGURED_SWITCHES     3   // X_MIN, Y_MIN, Z_MIN
#define CONFIGURED_ANALOG       2   // Analog 0, 1

// ============================================
// BULK COMMAND GROUP NAMES
// ============================================
#define GROUP_ALL_STEPPERS      "STEPPERS"
#define GROUP_ALL_SERVOS        "SERVOS"
#define GROUP_ALL_OUTPUTS       "OUTPUTS"
#define GROUP_ALL_SWITCHES      "SWITCHES"
#define GROUP_ALL_SENSORS       "SENSORS"
#define GROUP_ALL_ACTUATORS     "ACTUATORS"
#define GROUP_ALL_DEVICES       "ALL"

#endif // DEVICE_CONFIG_H