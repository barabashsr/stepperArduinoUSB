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
#define STEPPER_X_NAME          "BaseRotation"      // Example: base rotation motor
#define STEPPER_Y_NAME          "ArmExtension"      // Example: arm extension motor
#define STEPPER_Z_NAME          "VerticalLift"      // Example: vertical lift motor
#define STEPPER_E0_NAME         "GripperRotate"     // Example: gripper rotation
#define STEPPER_E1_NAME         "Extruder2"         // Example: second extruder

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
#define SERVO_0_NAME            "GripperServo"      // Example: gripper open/close
#define SERVO_1_NAME            "CameraServo"       // Example: camera tilt
#define SERVO_2_NAME            "Servo3"
#define SERVO_3_NAME            "Servo4"

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
#define MOSFET_A_NAME           "MainLight"         // Example: main LED light
#define MOSFET_B_NAME           "CoolingFan"        // Example: cooling fan
#define MOSFET_C_NAME           "VacuumPump"        // Example: vacuum pump

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
#define SWITCH_X_MIN_NAME       "BaseHomeSwitch"    // Example: base home position
#define SWITCH_Y_MIN_NAME       "ArmHomeSwitch"     // Example: arm home position
#define SWITCH_Z_MIN_NAME       "VerticalHome"      // Example: vertical home
#define SWITCH_X_MAX_NAME       "BaseLimitSwitch"   // Example: base limit
#define SWITCH_Y_MAX_NAME       "ArmLimitSwitch"    // Example: arm limit
#define SWITCH_Z_MAX_NAME       "VerticalLimit"     // Example: vertical limit

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
#define ANALOG_0_NAME           "TempSensor1"       // Example: temperature sensor
#define ANALOG_1_NAME           "PressureSensor"    // Example: pressure sensor
#define ANALOG_2_NAME           "LightSensor"       // Example: light sensor

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