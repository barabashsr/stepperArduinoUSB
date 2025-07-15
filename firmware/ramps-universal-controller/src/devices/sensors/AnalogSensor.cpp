/**
 * @file AnalogSensor.cpp
 * @brief Implementation of AnalogSensor class
 */

#include "AnalogSensor.h"
#include <math.h>

/**
 * @brief Constructor
 */
AnalogSensor::AnalogSensor(const String& name, int pin, int mode)
    : Sensor(name, DeviceType::ANALOG_SENSOR) {
    analogPin = pin;
    sensorMode = mode;
    vref = 5.0;  // Default 5V reference
    
    // Smoothing setup
    if (ANALOG_SMOOTHING) {
        sampleBuffer = new float[ANALOG_SAMPLES];
        for (int i = 0; i < ANALOG_SAMPLES; i++) {
            sampleBuffer[i] = 0.0;
        }
    } else {
        sampleBuffer = nullptr;
    }
    bufferIndex = 0;
    sampleCount = 0;
    
    // Conversion defaults
    scale = 1.0;
    offset = 0.0;
    
    // Thermistor defaults
    pullupResistor = 10000.0;
    thermistorR25 = 10000.0;
    thermistorBeta = 3950.0;
}

/**
 * @brief Destructor
 */
AnalogSensor::~AnalogSensor() {
    if (sampleBuffer) {
        delete[] sampleBuffer;
    }
}

/**
 * @brief Initialize the sensor
 */
bool AnalogSensor::init() {
    // Analog pins don't need pinMode configuration
    
    // Take initial reading
    readValue();
    
    // Initialize state
    state = DeviceState::IDLE;
    enabled = true;
    
    return true;
}

/**
 * @brief Update sensor
 */
void AnalogSensor::update() {
    if (!enabled) return;
    
    // Check if we should take a new reading
    // (Could implement periodic reading here)
    
    updateTimestamp();
}

/**
 * @brief Read current sensor value
 */
float AnalogSensor::readValue() {
    if (!enabled) return currentValue;
    
    // Get raw or smoothed value
    float raw = readSmoothed();
    float value = raw;
    
    // Apply conversion based on mode
    switch (sensorMode) {
        case SENSOR_MODE_RAW:
            value = raw;
            break;
            
        case SENSOR_MODE_VOLTAGE:
            value = rawToVoltage(raw);
            break;
            
        case SENSOR_MODE_CUSTOM:
            // Check if configured as thermistor
            if (thermistorBeta > 0) {
                value = rawToTemperature(raw);
            } else {
                // Generic linear conversion
                value = (raw * scale) + offset;
            }
            break;
    }
    
    // Update value
    updateValue(value);
    
    return currentValue;
}

/**
 * @brief Get raw ADC value
 */
int AnalogSensor::getRawValue() {
    return analogRead(analogPin);
}

/**
 * @brief Get current status
 */
String AnalogSensor::getStatus() const {
    String status = Device::getStatus();
    
    status += ", Mode: ";
    switch (sensorMode) {
        case SENSOR_MODE_RAW:
            status += "RAW";
            break;
        case SENSOR_MODE_VOLTAGE:
            status += "VOLTAGE";
            break;
        case SENSOR_MODE_CUSTOM:
            status += "CUSTOM";
            break;
    }
    
    return status;
}

/**
 * @brief Read and smooth analog value
 */
float AnalogSensor::readSmoothed() {
    float raw = (float)analogRead(analogPin);
    
    if (ANALOG_SMOOTHING && sampleBuffer) {
        // Add to circular buffer
        sampleBuffer[bufferIndex] = raw;
        bufferIndex = (bufferIndex + 1) % ANALOG_SAMPLES;
        
        if (sampleCount < ANALOG_SAMPLES) {
            sampleCount++;
        }
        
        // Calculate average
        float sum = 0.0;
        for (int i = 0; i < sampleCount; i++) {
            sum += sampleBuffer[i];
        }
        raw = sum / sampleCount;
    }
    
    return raw;
}

/**
 * @brief Convert raw ADC to temperature
 */
float AnalogSensor::rawToTemperature(float raw) const {
    if (raw <= 0 || raw >= ANALOG_MAX_VALUE) {
        return -999.0;  // Error value
    }
    
    // Calculate resistance of thermistor
    float resistance = pullupResistor * (ANALOG_MAX_VALUE / raw - 1.0);
    
    // Use simplified Steinhart-Hart equation (Beta equation)
    // 1/T = 1/T0 + (1/B) * ln(R/R0)
    const float T0 = 273.15 + 25.0;  // 25°C in Kelvin
    float T = 1.0 / (1.0/T0 + (1.0/thermistorBeta) * log(resistance/thermistorR25));
    
    // Convert to Celsius
    return T - 273.15;
}