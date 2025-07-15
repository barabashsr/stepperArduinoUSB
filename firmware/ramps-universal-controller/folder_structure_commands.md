# Terminal Commands to Create Project Structure

Execute these commands in your terminal to create the complete folder structure:

```bash
# Create main project directory
mkdir ramps-universal-controller
cd ramps-universal-controller

# Create PlatformIO directories
mkdir -p src/core
mkdir -p src/devices/actuators
mkdir -p src/devices/sensors
mkdir -p src/utils
mkdir -p include
mkdir -p lib
mkdir -p test
mkdir -p data

# Create all header files
touch platformio.ini
touch README.md

# Configuration headers
touch include/Config.h
touch include/PinDefinitions.h
touch include/DeviceConfig.h
touch include/README

# Core module files
touch src/main.cpp
touch src/core/Controller.h
touch src/core/Controller.cpp
touch src/core/Interface.h
touch src/core/Interface.cpp
touch src/core/Command.h
touch src/core/Command.cpp
touch src/core/Reply.h
touch src/core/Reply.cpp
touch src/core/Message.h
touch src/core/Message.cpp

# Base device class files
touch src/devices/Device.h
touch src/devices/Device.cpp
touch src/devices/Actuator.h
touch src/devices/Actuator.cpp
touch src/devices/Sensor.h
touch src/devices/Sensor.cpp

# Actuator implementation files
touch src/devices/actuators/StepperMotor.h
touch src/devices/actuators/StepperMotor.cpp
touch src/devices/actuators/Servo.h
touch src/devices/actuators/Servo.cpp
touch src/devices/actuators/MosfetOutput.h
touch src/devices/actuators/MosfetOutput.cpp

# Sensor implementation files
touch src/devices/sensors/EndSwitch.h
touch src/devices/sensors/EndSwitch.cpp
touch src/devices/sensors/AnalogSensor.h
touch src/devices/sensors/AnalogSensor.cpp

# Utility files (placeholders)
touch src/utils/Display.h
touch src/utils/Display.cpp
touch src/utils/Encoder.h
touch src/utils/Encoder.cpp

# README files for empty directories
touch data/README
touch lib/README
touch test/README
```

## Alternative: One-liner for Linux/Mac
```bash
mkdir -p ramps-universal-controller/{src/{core,devices/{actuators,sensors},utils},include,lib,test,data} && cd ramps-universal-controller && touch platformio.ini README.md include/{Config.h,PinDefinitions.h,DeviceConfig.h,README} src/main.cpp src/core/{Controller,Interface,Command,Reply,Message}.{h,cpp} src/devices/{Device,Actuator,Sensor}.{h,cpp} src/devices/actuators/{StepperMotor,Servo,MosfetOutput}.{h,cpp} src/devices/sensors/{EndSwitch,AnalogSensor}.{h,cpp} src/utils/{Display,Encoder}.{h,cpp} {data,lib,test}/README
```
