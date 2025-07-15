# RAMPS Universal Controller - Deployment Guide

This guide provides comprehensive instructions for deploying and configuring the RAMPS Universal Controller firmware.

## Table of Contents
1. [Prerequisites](#prerequisites)
2. [Hardware Setup](#hardware-setup)
3. [Software Installation](#software-installation)
4. [Firmware Configuration](#firmware-configuration)
5. [Building and Uploading](#building-and-uploading)
6. [Initial Testing](#initial-testing)
7. [Device Configuration](#device-configuration)
8. [Advanced Configuration](#advanced-configuration)
9. [Troubleshooting](#troubleshooting)
10. [Usage Examples](#usage-examples)

## Prerequisites

### Hardware Requirements
- **Arduino Mega 2560** board
- **RAMPS 1.4** shield (or compatible)
- **12-24V power supply** (capacity depends on your motors)
- **USB cable** (Type A to Type B)
- **Stepper motors** (NEMA 17 or similar)
- **Stepper drivers** (A4988, DRV8825, or similar)
- Optional: Servos, switches, sensors

### Software Requirements
- **PlatformIO** (recommended) or Arduino IDE
- **VS Code** (recommended for PlatformIO)
- **Serial terminal** (PuTTY, CoolTerm, or VS Code Serial Monitor)
- **Git** (for cloning repository)

## Hardware Setup

### 1. Install RAMPS on Arduino Mega
1. Carefully align the RAMPS shield with the Arduino Mega pins
2. Press down firmly but gently until fully seated
3. Ensure no pins are bent or misaligned

### 2. Install Stepper Drivers
1. **IMPORTANT**: Set motor current BEFORE installing drivers
   - A4988: Adjust potentiometer (Vref = Current × 8 × Rs)
   - DRV8825: Vref = Current × 2
2. Install drivers in X, Y, Z sockets as needed
3. Ensure proper orientation (check EN, STEP, DIR labels)
4. Install heatsinks on driver chips

### 3. Connect Motors
```
X-axis: Connect to X motor connector
Y-axis: Connect to Y motor connector  
Z-axis: Connect to Z motor connector
E0/E1: Available for additional motors
```

### 4. Connect Servos
```
Servo 1: Pin D11
Servo 2: Pin D6
Servo 3: Pin D5  
Servo 4: Pin D4
```
Note: Servos need 5V power, use RAMPS servo power selection jumper

### 5. Connect Switches
```
X-MIN: Pin D3
X-MAX: Pin D2
Y-MIN: Pin D14
Y-MAX: Pin D15  
Z-MIN: Pin D18
Z-MAX: Pin D19
```

### 6. Connect Power
1. Connect 12-24V power to RAMPS power input
2. Ensure correct polarity (+ and -)
3. Use adequate wire gauge for current
4. Add capacitor if not included on RAMPS

## Software Installation

### Using PlatformIO (Recommended)

1. **Install VS Code**
   ```bash
   # Ubuntu/Debian
   sudo snap install code --classic
   
   # macOS (with Homebrew)
   brew install --cask visual-studio-code
   
   # Windows
   Download from https://code.visualstudio.com/
   ```

2. **Install PlatformIO Extension**
   - Open VS Code
   - Go to Extensions (Ctrl+Shift+X)
   - Search for "PlatformIO"
   - Click Install

3. **Clone Repository**
   ```bash
   git clone https://github.com/yourusername/ramps-universal-controller.git
   cd ramps-universal-controller
   ```

4. **Open Project**
   - File → Open Folder → Select project directory
   - PlatformIO will auto-detect platformio.ini

### Using Arduino IDE

1. **Install Arduino IDE** (1.8.x or 2.x)
2. **Install Libraries**:
   - Tools → Manage Libraries
   - Install: AccelStepper, Servo
3. **Configure Board**:
   - Tools → Board → Arduino Mega 2560
   - Tools → Port → Select your COM/USB port

## Firmware Configuration

### 1. Basic Configuration (`include/Config.h`)

```cpp
// Serial Communication
#define SERIAL_BAUD_RATE        115200  // Don't change unless necessary
#define DEFAULT_ACK_MODE        true    // Set false for less verbose

// Motion Settings
#define DEFAULT_MAX_SPEED       1000.0  // Adjust based on your motors
#define DEFAULT_ACCELERATION    500.0   // Start conservative

// Safety
#define ENABLE_ESTOP            true    // Keep enabled
#define SWITCH_DEBOUNCE_MS      50      // Increase if switches are noisy
```

### 2. Device Configuration (`include/DeviceConfig.h`)

```cpp
// Example: Configure a 3-axis CNC with gripper
#define STEPPER_X_NAME          "XAxis"
#define STEPPER_Y_NAME          "YAxis"  
#define STEPPER_Z_NAME          "ZAxis"
#define SERVO_0_NAME            "Gripper"
#define MOSFET_A_NAME           "SpindleMotor"
#define SWITCH_X_MIN_NAME       "XHome"
#define SWITCH_Y_MIN_NAME       "YHome"
#define SWITCH_Z_MIN_NAME       "ZHome"

// Set steps per revolution (motor steps × microstepping)
#define STEPPER_X_STEPS_PER_REV     3200    // 200 × 16
#define STEPPER_Y_STEPS_PER_REV     3200
#define STEPPER_Z_STEPS_PER_REV     3200

// Servo limits
#define SERVO_0_MIN_ANGLE       0
#define SERVO_0_MAX_ANGLE       180
```

### 3. Enable/Disable Devices

```cpp
// Only enable what you're using
#define STEPPER_X_ENABLED
#define STEPPER_Y_ENABLED
#define STEPPER_Z_ENABLED
// #define STEPPER_E0_ENABLED  // Uncomment if needed
// #define STEPPER_E1_ENABLED

#define SERVO_0_ENABLED
// #define SERVO_1_ENABLED

#define MOSFET_A_ENABLED  // All MOSFETs support PWM
#define MOSFET_B_ENABLED
#define MOSFET_C_ENABLED

#define SWITCH_X_MIN_ENABLED
#define SWITCH_Y_MIN_ENABLED
#define SWITCH_Z_MIN_ENABLED
```

## Building and Uploading

### PlatformIO Method

1. **Build Firmware**
   ```bash
   # Command line
   pio run
   
   # Or in VS Code
   Click PlatformIO icon → Build
   ```

2. **Upload Firmware**
   ```bash
   # Auto-detect port
   pio run -t upload
   
   # Specify port
   pio run -t upload --upload-port /dev/ttyUSB0
   ```

3. **Monitor Serial**
   ```bash
   pio device monitor -b 115200
   
   # Or in VS Code
   PlatformIO icon → Monitor
   ```

### Arduino IDE Method

1. Select Tools → Port → Your Arduino port
2. Click Upload button (→)
3. Open Serial Monitor (Ctrl+Shift+M)
4. Set baud rate to 115200

## Initial Testing

### 1. Connection Test
```
# You should see:
===========================================
RAMPS 1.4 Universal Controller
Firmware Version 1.0.0
===========================================
Ready for commands.
```

### 2. List Devices
```
>CONTROLLER LIST
```

Expected output shows all configured devices.

### 3. Test Each Device Type

**Stepper Motor Test**:
```
>XAxis position 6.28    # One full rotation
>XAxis velocity 1.0     # Continuous rotation at 1 rad/sec
>XAxis velocity 0       # Stop
>XAxis position 0       # Return to start
```

**Servo Test**:
```
>Gripper position 0     # Minimum position
>Gripper position 1.57  # 90 degrees
>Gripper position 3.14  # Maximum position
```

**MOSFET Test**:
```
>SpindleMotor ON        # Full power
>SpindleMotor position 0.5  # 50% PWM
>SpindleMotor OFF       # Turn off
```

**Switch Test**:
```
>XHome state?           # Query state
# Manually trigger switch
# Should see: XHome state 1 EVENT
```

## Device Configuration

### Stepper Motor Tuning

1. **Calculate Steps per Unit**:
   ```
   Steps/mm = (Motor Steps × Microstepping) / (Belt Pitch × Pulley Teeth)
   Steps/rad = (Motor Steps × Microstepping) / (2π)
   ```

2. **Set Maximum Speed**:
   ```cpp
   #define STEPPER_X_MAX_SPEED  2000.0  // Adjust based on testing
   ```

3. **Tune Acceleration**:
   - Start low (100-500)
   - Increase until motors skip
   - Back off 20-30%

### Servo Calibration

1. **Find actual limits**:
   ```
   >Gripper position 0
   # Measure actual angle
   >Gripper position 3.14
   # Measure actual angle
   ```

2. **Adjust in config**:
   ```cpp
   #define SERVO_0_MIN_ANGLE  10   // If 0 is too far
   #define SERVO_0_MAX_ANGLE  170  // If 180 is too far
   ```

### Thermistor Configuration

For temperature sensors:
```cpp
// In DeviceConfig.h
#define ANALOG_0_MODE           SENSOR_MODE_CUSTOM
#define ANALOG_0_R_PULLUP       4700     // Pullup resistor
#define ANALOG_0_THERMISTOR_R25 100000   // 100k thermistor
#define ANALOG_0_THERMISTOR_BETA 3950    // Beta value
```

## Advanced Configuration

### Custom Units

To use millimeters instead of radians:
```cpp
// In StepperMotor configuration
float stepsPerMm = 80.0;  // Example for 20-tooth GT2 pulley
steppers[0]->setStepsPerUnit(stepsPerMm);
```

### Event Handling

Enable automatic switch reporting:
```cpp
#define REPORT_SWITCH_EVENTS    true
#define SWITCH_DEBOUNCE_MS      50
```

### Service Commands

Implement custom calibration:
```cpp
// Add in Controller::executeServiceCommand()
if (service == "HOME_ALL") {
    success = calibrateAxis("XAxis") && 
              calibrateAxis("YAxis") && 
              calibrateAxis("ZAxis");
}
```

## Troubleshooting

### Common Issues

1. **No Serial Communication**
   - Check baud rate (115200)
   - Verify correct COM port
   - Try different USB cable/port
   - Reset Arduino

2. **Motors Not Moving**
   - Check power supply voltage/current
   - Verify motor wiring (pairs)
   - Check driver installation/orientation
   - Enable motors first: `>XAxis enable`

3. **Motors Moving Wrong Direction**
   - Reverse motor connector
   - Or set inversion in code:
     ```cpp
     steppers[0]->setInvertDirection(true);
     ```

4. **Servo Jittering**
   - Check 5V power supply
   - Add capacitor near servo
   - Reduce servo speed

5. **Switch Always Triggered**
   - Check INVERTED setting:
     ```cpp
     #define SWITCH_X_MIN_INVERTED true
     ```

### Debug Mode

Enable debugging in `Config.h`:
```cpp
#define DEBUG_ENABLED    true
#define DEBUG_LEVEL      3    // 0=off, 3=verbose
```

### LED Indicators
- **Rapid blink**: Initialization error
- **Slow blink**: Controller error
- **2-second pulse**: Normal operation

## Usage Examples

### Example 1: CNC Router Control
```bash
# Home all axes
>SERVICE CALIBRATE_ALL

# Move to position
>XAxis position 100
>YAxis position 50
>ZAxis position -10

# Start spindle
>SpindleMotor ON

# Cut a line
>XAxis velocity 0.5
# ... wait ...
>XAxis velocity 0

# Stop spindle
>SpindleMotor OFF
```

### Example 2: Pick and Place Robot
```bash
# Move to pick position
>XAxis position 200
>YAxis position 100
>ZAxis position 50

# Open gripper
>Gripper position 3.14

# Lower to object
>ZAxis position 10

# Close gripper
>Gripper position 0

# Lift object
>ZAxis position 50
```

### Example 3: Temperature Monitoring
```bash
# Read temperature
>TempSensor1 read
# Returns: TempSensor1 value 24.5

# Continuous monitoring with events
# (Configure threshold in sensor setup)
```

## Performance Tips

1. **Reduce ACK verbosity** if not needed:
   ```
   >CONTROLLER ackmode 0
   ```

2. **Use bulk commands** for efficiency:
   ```
   >STEPPERS velocity 0  # Stop all steppers
   ```

3. **Adjust loop delay** for responsiveness:
   ```cpp
   #define MAIN_LOOP_DELAY  0  // No delay for maximum speed
   ```

## Safety Recommendations

1. **Always implement E-stop**:
   - Physical button on interrupt pin
   - Software command: `>CONTROLLER ESTOP`

2. **Set reasonable limits**:
   - Maximum speeds
   - Acceleration rates
   - Position limits

3. **Use homing switches**:
   - Prevent over-travel
   - Establish reference position

4. **Monitor temperature**:
   - Motor drivers can overheat
   - Use heatsinks and cooling

## Next Steps

1. **Customize device names** for your application
2. **Implement service commands** for your workflow  
3. **Add new device types** by extending base classes
4. **Create host software** to automate control
5. **Share your configurations** with the community!

For questions or issues, please refer to the GitHub repository.
