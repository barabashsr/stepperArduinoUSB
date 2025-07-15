/**
 * @file PinDefinitions.h
 * @brief Complete RAMPS 1.4 pin definitions for Arduino Mega 2560
 * 
 * This file contains all pin mappings for RAMPS 1.4 shield
 * Based on Marlin firmware pin definitions
 */

#ifndef PIN_DEFINITIONS_H
#define PIN_DEFINITIONS_H

// ============================================
// SERVO PINS
// ============================================
#define SERVO0_PIN              11      // Servo 1
#define SERVO1_PIN              6       // Servo 2
#define SERVO2_PIN              5       // Servo 3
#define SERVO3_PIN              4       // Servo 4

// ============================================
// LIMIT SWITCH PINS
// ============================================
#define X_MIN_PIN               3       // X-
#define X_MAX_PIN               2       // X+
#define Y_MIN_PIN               14      // Y-
#define Y_MAX_PIN               15      // Y+
#define Z_MIN_PIN               18      // Z-
#define Z_MAX_PIN               19      // Z+

// ============================================
// STEPPER MOTOR PINS
// ============================================
// X Axis
#define X_STEP_PIN              54      // A0
#define X_DIR_PIN               55      // A1
#define X_ENABLE_PIN            38
#define X_CS_PIN                -1      // Chip select for TMC drivers

// Y Axis
#define Y_STEP_PIN              60      // A6
#define Y_DIR_PIN               61      // A7
#define Y_ENABLE_PIN            56      // A2
#define Y_CS_PIN                -1

// Z Axis
#define Z_STEP_PIN              46
#define Z_DIR_PIN               48
#define Z_ENABLE_PIN            62      // A8
#define Z_CS_PIN                -1

// E0 Axis (Extruder 0)
#define E0_STEP_PIN             26
#define E0_DIR_PIN              28
#define E0_ENABLE_PIN           24
#define E0_CS_PIN               -1

// E1 Axis (Extruder 1)
#define E1_STEP_PIN             36
#define E1_DIR_PIN              34
#define E1_ENABLE_PIN           30
#define E1_CS_PIN               -1

// ============================================
// TEMPERATURE SENSOR PINS (Analog Inputs)
// ============================================
#define TEMP_0_PIN              13      // Analog Input (Hotend 0)
#define TEMP_1_PIN              15      // Analog Input (Hotend 1)
#define TEMP_BED_PIN            14      // Analog Input (Bed)

// Alternative names for generic use
#define ANALOG_0_PIN            TEMP_0_PIN
#define ANALOG_1_PIN            TEMP_1_PIN
#define ANALOG_2_PIN            TEMP_BED_PIN

// ============================================
// HEATER/FAN MOSFET PINS
// ============================================
#define HEATER_0_PIN            10      // MOSFET 1 - PWM capable
#define HEATER_1_PIN            9       // MOSFET 2 - PWM capable
#define HEATER_BED_PIN          8       // MOSFET 3 - PWM capable

// Alternative names for generic use
#define MOSFET_A_PIN            HEATER_0_PIN
#define MOSFET_B_PIN            HEATER_1_PIN
#define MOSFET_C_PIN            HEATER_BED_PIN

// ============================================
// MISCELLANEOUS PINS
// ============================================
#define LED_PIN                 13      // Built-in LED
#define PS_ON_PIN               12      // Power supply control
#define SDSS                    53      // SD card SS pin
#define SD_DETECT_PIN           49      // SD card detect

// ============================================
// LCD PINS (RepRapDiscount Smart Controller)
// ============================================
#define LCD_PINS_RS             16      // LCD control
#define LCD_PINS_ENABLE         17      // LCD control
#define LCD_PINS_D4             23      // LCD data
#define LCD_PINS_D5             25      // LCD data
#define LCD_PINS_D6             27      // LCD data
#define LCD_PINS_D7             29      // LCD data

// Encoder pins
#define BTN_EN1                 31      // Encoder rotary
#define BTN_EN2                 33      // Encoder rotary
#define BTN_ENC                 35      // Encoder button
#define BEEPER_PIN              37      // Beeper

// ============================================
// AUX PINS (for expansion)
// ============================================
// AUX-1
#define AUX1_00                 0       // RX0
#define AUX1_01                 1       // TX0
#define AUX1_A3                 57      // A3
#define AUX1_A4                 58      // A4

// AUX-2
#define AUX2_03                 59      // A5
#define AUX2_04                 63      // A9
#define AUX2_05                 64      // A10
#define AUX2_06                 40
#define AUX2_07                 44
#define AUX2_08                 42
#define AUX2_09                 66      // A12
#define AUX2_10                 65      // A11

// AUX-3
#define AUX3_02                 49
#define AUX3_03                 50      // MISO
#define AUX3_04                 51      // MOSI
#define AUX3_05                 52      // SCK
#define AUX3_06                 53      // SS

// AUX-4 (16-pin expansion)
#define AUX4_03                 32
#define AUX4_04                 47
#define AUX4_05                 45
#define AUX4_06                 43
#define AUX4_07                 41
#define AUX4_08                 39
#define AUX4_09                 37
#define AUX4_10                 35
#define AUX4_11                 33
#define AUX4_12                 31
#define AUX4_13                 29
#define AUX4_14                 27
#define AUX4_15                 25
#define AUX4_16                 23
#define AUX4_17                 17
#define AUX4_18                 16

// ============================================
// PIN STATE DEFINITIONS
// ============================================
#define STEPPER_ENABLE_ON       LOW     // Logic level to enable steppers
#define STEPPER_ENABLE_OFF      HIGH    // Logic level to disable steppers
#define SWITCH_PULLUP           true    // Enable internal pullups for switches

#endif // PIN_DEFINITIONS_H