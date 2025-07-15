# RAMPS 1.4 Universal Controller Firmware

A flexible, object-oriented firmware for Arduino Mega 2560 with RAMPS 1.4 shield that transforms your 3D printer controller into a universal robotics controller. Control stepper motors, servos, digital outputs, and read sensors via simple serial commands.

## Features

- **Universal Device Control**: Control various devices through a unified serial interface
  - Stepper motors with acceleration and velocity control
  - Servo motors with speed-limited movement
  - MOSFET outputs with PWM support
  - Digital switches with debouncing and event reporting
  - Analog sensors with smoothing and conversion options

- **Flexible Command System**: 
  - Human-readable ASCII commands
  - Individual device control
  - Bulk operations on device groups
  - System status queries
  - Service commands for complex operations

- **Object-Oriented Architecture**:
  - Modular design with device abstraction
  - Easy to extend with new device types
  - Clean separation of concerns
  - Portable to other platforms

## Hardware Requirements

- Arduino Mega 2560
- RAMPS 1.4 shield
- USB cable for serial communication
- Power supply (12-24V for motors)
- Stepper motors, servos, sensors as needed

## Quick Start

1. **Install PlatformIO** (VS Code extension recommended)

2. **Clone the repository**:
   ```bash
   git clone https://github.com/yourusername/ramps-universal-controller.git
   cd ramps-universal-controller
   ```

3. **Configure your devices** in `include/DeviceConfig.h`:
   ```cpp
   #define STEPPER_X_NAME "BaseRotation"
   #define SERVO_0_NAME "Gripper"
   // etc...
   ```

4. **Build and upload**:
   ```bash
   pio run -t upload
   ```

5. **Connect via serial** (115200 baud) and send commands:
   ```
   >BaseRotation position 3.14
   >Gripper position 1.57
   >MainLight ON
   ```

## Command Format

Commands follow this format:
```
>DeviceName interface value
```

Examples:
- `>BaseRotation position 3.14` - Move stepper to position (radians)
- `>BaseRotation velocity 1.0` - Set velocity (rad/sec)
- `>Gripper position 1.57` - Move servo to position
- `>MainLight ON` - Turn on MOSFET output
- `>MainLight position 0.5` - Set PWM to 50%
- `>BaseHomeSwitch state?` - Query switch state
- `>TempSensor1 read` - Read sensor value
- `>STEPPERS velocity 0` - Stop all steppers
- `>CONTROLLER LIST` - List all devices

## Device Types

### Stepper Motors
- **Interfaces**: position, velocity, stop, reset
- **Units**: radians or meters per second
- **Features**: Acceleration control, continuous rotation

### Servo Motors  
- **Interfaces**: position, velocity, stop, reset
- **Units**: radians (0 to π)
- **Features**: Speed-limited movement, position hold

### MOSFET Outputs
- **Interfaces**: position (0-1), state, ON, OFF
- **Features**: PWM control, binary on/off

### End Switches
- **Interfaces**: state, read
- **Features**: Debouncing, event reporting

### Analog Sensors
- **Interfaces**: read, value
- **Modes**: Raw ADC, voltage, custom conversion
- **Features**: Smoothing, thermistor support

## Configuration

### Device Configuration (`include/DeviceConfig.h`)
- Enable/disable devices
- Set custom names
- Configure pins and parameters

### System Configuration (`include/Config.h`)
- Serial settings
- Motion parameters  
- Behavior options
- Debug settings

### Pin Definitions (`include/PinDefinitions.h`)
- Complete RAMPS 1.4 pin mapping
- Compatible with Marlin pin definitions

## Architecture

The firmware follows an object-oriented design:

```
Controller (manages all devices)
├── Interface (handles serial communication)
├── Devices
│   ├── Actuators
│   │   ├── StepperMotor
│   │   ├── ServoMotor
│   │   └── MosfetOutput
│   └── Sensors
│       ├── EndSwitch
│       └── AnalogSensor
└── Commands/Replies (message handling)
```

## Extending the Firmware

To add a new device type:

1. Create a new class inheriting from `Actuator` or `Sensor`
2. Implement required virtual methods
3. Add device creation in `Controller::createDevices()`
4. Configure in `DeviceConfig.h`

## Safety Features

- Emergency stop command (`>CONTROLLER ESTOP`)
- Automatic switch event reporting
- Command timeout protection
- Range limiting for all parameters

## Troubleshooting

- **LED blinks rapidly**: Serial initialization failed
- **LED blinks slowly**: Controller initialization failed  
- **No response**: Check baud rate (115200)
- **Unknown device error**: Check device names in config

## License

This project is open source and available under the MIT License.

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## Acknowledgments

- Inspired by Marlin firmware architecture
- Uses AccelStepper library for smooth motion
- RAMPS 1.4 pin definitions based on Marlin project