#include <Arduino.h>
#include <AccelStepper.h>

// RAMPS 1.4 Pin Definitions
#define X_STEP_PIN 54
#define X_DIR_PIN 55
#define X_ENABLE_PIN 38

#define Y_STEP_PIN 60
#define Y_DIR_PIN 61
#define Y_ENABLE_PIN 56

#define Z_STEP_PIN 46
#define Z_DIR_PIN 48
#define Z_ENABLE_PIN 62

// Constants for continuous rotation
#define LONG_MAX_STEPS 2147483647L
#define LONG_MIN_STEPS -2147483648L

// Control structure for each motor
struct MotorControl {
    volatile float targetSpeed = 0;
    volatile bool direction = true;
    volatile bool enabled = false;
    volatile float acceleration = 500.0;
    volatile unsigned long enableTimestamp = 0;
    volatile bool stopping = false;
    volatile bool speedChange = false;
};

// Motor control instances
MotorControl xControl, yControl, zControl;

// AccelStepper instances
AccelStepper stepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepperY(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);
AccelStepper stepperZ(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);

void printMotorStatus(String axis, AccelStepper& stepper, MotorControl& control) {
    Serial.print(axis + " - Enabled: ");
    Serial.print(control.enabled ? "YES" : "NO");
    Serial.print(", Speed: ");
    Serial.print(control.targetSpeed);
    Serial.print(", Current: ");
    Serial.print(stepper.speed());
    Serial.print(", Dir: ");
    Serial.println(control.direction ? "CW" : "CCW");
}

void smoothDisable(AccelStepper& stepper, MotorControl& control, int enablePin) {
    if(abs(stepper.speed()) > 0) {
        control.stopping = true;
        control.enabled = false;
        stepper.stop();
    } else {
        digitalWrite(enablePin, HIGH);  // Disable driver (HIGH = disabled)
        control.enabled = false;
    }
}

void disableAllDrivers() {
    digitalWrite(X_ENABLE_PIN, HIGH);
    digitalWrite(Y_ENABLE_PIN, HIGH);
    digitalWrite(Z_ENABLE_PIN, HIGH);
    xControl.enabled = false;
    yControl.enabled = false;
    zControl.enabled = false;
}

void enableDriver(AccelStepper& stepper, MotorControl& control, int enablePin) {
    digitalWrite(enablePin, LOW);  // Enable driver (LOW = enabled)
    control.enabled = true;
    control.enableTimestamp = millis();
    control.stopping = false;
    control.speedChange = true;
    stepper.setCurrentPosition(0);
    
    // Start movement
    long target = control.direction ? LONG_MAX_STEPS : LONG_MIN_STEPS;
    stepper.move(target);
    Serial.println("Driver enabled");
}

void disableDriver(AccelStepper& stepper, MotorControl& control, int enablePin) {
    if(control.enabled) {
        smoothDisable(stepper, control, enablePin);
        Serial.println("Driver stopping...");
    }
}




void handleSerial() {
    if(!Serial.available()) return;
    
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    
    // X-axis commands
    if(cmd.startsWith("SPD_X ")) {
        float speed = cmd.substring(6).toFloat();
        speed = constrain(speed, 0, 1000);
        xControl.targetSpeed = speed;
        xControl.speedChange = true;
        Serial.print("X Speed: ");
        Serial.println(speed);
    }
    else if(cmd.startsWith("DIR_X ")) {
        bool newDir = cmd.substring(6).toInt() > 0;
        xControl.direction = newDir;
        Serial.print("X Direction: ");
        Serial.println(newDir ? "CW" : "CCW");
    }
    else if(cmd == "ENABLE_X") {
        enableDriver(stepperX, xControl, X_ENABLE_PIN);
    }
    else if(cmd == "DISABLE_X") {
        disableDriver(stepperX, xControl, X_ENABLE_PIN);
    }
    
    // Y-axis commands
    else if(cmd.startsWith("SPD_Y ")) {
        float speed = cmd.substring(6).toFloat();
        speed = constrain(speed, 0, 1000);
        yControl.targetSpeed = speed;
        yControl.speedChange = true;
        Serial.print("Y Speed: ");
        Serial.println(speed);
    }
    else if(cmd.startsWith("DIR_Y ")) {
        bool newDir = cmd.substring(6).toInt() > 0;
        yControl.direction = newDir;
        Serial.print("Y Direction: ");
        Serial.println(newDir ? "CW" : "CCW");
    }
    else if(cmd == "ENABLE_Y") {
        enableDriver(stepperY, yControl, Y_ENABLE_PIN);
    }
    else if(cmd == "DISABLE_Y") {
        disableDriver(stepperY, yControl, Y_ENABLE_PIN);
    }
    
    // Z-axis commands
    else if(cmd.startsWith("SPD_Z ")) {
        float speed = cmd.substring(6).toFloat();
        speed = constrain(speed, 0, 1000);
        zControl.targetSpeed = speed;
        zControl.speedChange = true;
        Serial.print("Z Speed: ");
        Serial.println(speed);
    }
    else if(cmd.startsWith("DIR_Z ")) {
        bool newDir = cmd.substring(6).toInt() > 0;
        zControl.direction = newDir;
        Serial.print("Z Direction: ");
        Serial.println(newDir ? "CW" : "CCW");
    }
    else if(cmd == "ENABLE_Z") {
        enableDriver(stepperZ, zControl, Z_ENABLE_PIN);
    }
    else if(cmd == "DISABLE_Z") {
        disableDriver(stepperZ, zControl, Z_ENABLE_PIN);
    }
    
    // General commands
    else if(cmd == "DISABLE_ALL") {
        disableAllDrivers();
        Serial.println("All drivers disabled");
    }
    else if(cmd == "STATUS") {
        Serial.println("\n=== MOTOR STATUS ===");
        printMotorStatus("X", stepperX, xControl);
        printMotorStatus("Y", stepperY, yControl);
        printMotorStatus("Z", stepperZ, zControl);
        Serial.println("===================");
    }
}
void updateMotorMotion(AccelStepper& stepper, MotorControl& control, int enablePin) {
    if(control.stopping) {
        if(!stepper.isRunning()) {
            control.stopping = false;
            digitalWrite(enablePin, HIGH);  // Physically disable driver
        }
        return;
    }

    if(!control.enabled) return;

    if(control.speedChange) {
        if(abs(stepper.speed()) > control.targetSpeed) {
            stepper.stop();
            if(!stepper.isRunning()) {
                control.speedChange = false;
                long target = control.direction ? LONG_MAX_STEPS : LONG_MIN_STEPS;
                stepper.move(target);
            }
            return;
        } else {
            control.speedChange = false;
        }
    }

    // Normal operation - maintain continuous rotation
    if(stepper.distanceToGo() == 0) {
        long target = control.direction ? LONG_MAX_STEPS : LONG_MIN_STEPS;
        stepper.move(target);
    }
    
    // Apply speed and acceleration settings
    stepper.setMaxSpeed(control.targetSpeed);
    stepper.setAcceleration(control.acceleration);
}
void updateMotion() {
    updateMotorMotion(stepperX, xControl, X_ENABLE_PIN);
    updateMotorMotion(stepperY, yControl, Y_ENABLE_PIN);
    updateMotorMotion(stepperZ, zControl, Z_ENABLE_PIN);
}



void setup() {
    // Configure enable pins as outputs
    pinMode(X_ENABLE_PIN, OUTPUT);
    pinMode(Y_ENABLE_PIN, OUTPUT);
    pinMode(Z_ENABLE_PIN, OUTPUT);
    
    // Initialize steppers
    stepperX.setMaxSpeed(1000);
    stepperX.setAcceleration(500);
    stepperX.setPinsInverted(false, false, true);
    
    stepperY.setMaxSpeed(1000);
    stepperY.setAcceleration(500);
    stepperY.setPinsInverted(false, false, true);
    
    stepperZ.setMaxSpeed(1000);
    stepperZ.setAcceleration(500);
    stepperZ.setPinsInverted(false, false, true);
    
    // Disable all drivers initially
    disableAllDrivers();
    
    Serial.begin(115200);
    Serial.println("RAMPS 1.4 Stepper Controller Ready");
    Serial.println("Commands: ENABLE_X, DISABLE_X, SPD_X 500, DIR_X 1, STATUS");
}

void loop() {
    handleSerial();
    updateMotion();
    
    // Always run all steppers
    stepperX.run();
    stepperY.run();
    stepperZ.run();
}











