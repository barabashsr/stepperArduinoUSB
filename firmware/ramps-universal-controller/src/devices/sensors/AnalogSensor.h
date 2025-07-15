/**
 * @file AnalogSensor.h
 * @brief Analog sensor class for ADC inputs
 * 
 * Reads analog values with optional smoothing and conversion
 */

#ifndef ANALOG_SENSOR_H
#define ANALOG_SENSOR_H

#include "../Sensor.h"

/**
 * @class AnalogSensor
 * @brief Analog sensor with smoothing and conversion options
 * 
 * Can read raw ADC values or convert to voltage/custom units
 * Supports moving average smoothing
 */
class AnalogSensor : public Sensor {
private:
    int analogPin;              // Analog input pin
    int sensorMode;             // Reading mode (raw/voltage/custom)
    float vref;                 // Reference voltage (5.0V default)
    
    // Smoothing
    float* sampleBuffer;        // Circular buffer for samples
    int bufferIndex;            // Current buffer position
    int sampleCount;            // Number of samples collected
    
    // Conversion parameters
    float scale;                // Scale factor for custom conversion
    float offset;               // Offset for custom conversion
    
    // For thermistor mode
    float pullupResistor;       // Pullup resistor value
    float thermistorR25;        // Thermistor resistance at 25°C
    float thermistorBeta;       // Thermistor beta value
    
public:
    /**
     * @brief Constructor
     * @param name Device name
     * @param pin Analog input pin
     * @param mode Sensor mode (raw/voltage/custom)
     */
    AnalogSensor(const String& name, int pin, int mode = SENSOR_MODE_RAW);
    
    /**
     * @brief Destructor
     */
    ~AnalogSensor();
    
    /**
     * @brief Initialize the sensor
     * @return true if successful
     */
    bool init() override;
    
    /**
     * @brief Update sensor (read if needed)
     */
    void update() override;
    
    /**
     * @brief Read current sensor value
     * @return Sensor value (raw/voltage/converted)
     */
    float readValue() override;
    
    /**
     * @brief Get raw ADC value (0-1023)
     * @return Raw ADC reading
     */
    int getRawValue();
    
    /**
     * @brief Set sensor mode
     * @param mode New mode (raw/voltage/custom)
     */
    void setSensorMode(int mode) { sensorMode = mode; }
    
    /**
     * @brief Get sensor mode
     * @return Current mode
     */
    int getSensorMode() const { return sensorMode; }
    
    /**
     * @brief Set reference voltage
     * @param v Reference voltage (typically 5.0)
     */
    void setReferenceVoltage(float v) { vref = v; }
    
    /**
     * @brief Set custom conversion parameters
     * @param s Scale factor
     * @param o Offset
     */
    void setConversion(float s, float o) {
        scale = s;
        offset = o;
    }
    
    /**
     * @brief Configure as thermistor
     * @param pullup Pullup resistor value
     * @param r25 Resistance at 25°C
     * @param beta Beta coefficient
     */
    void configureThermistor(float pullup, float r25, float beta) {
        pullupResistor = pullup;
        thermistorR25 = r25;
        thermistorBeta = beta;
        sensorMode = SENSOR_MODE_CUSTOM;
    }
    
    /**
     * @brief Get list of supported interfaces
     * @return Comma-separated list
     */
    String getInterfaces() const override {
        return "read,value,raw,voltage,status";
    }
    
    /**
     * @brief Get current status
     * @return Status string
     */
    String getStatus() const override;
    
private:
    /**
     * @brief Read and smooth analog value
     * @return Smoothed raw ADC value
     */
    float readSmoothed();
    
    /**
     * @brief Convert raw ADC to voltage
     * @param raw Raw ADC value
     * @return Voltage
     */
    float rawToVoltage(float raw) const {
        return (raw / ANALOG_MAX_VALUE) * vref;
    }
    
    /**
     * @brief Convert raw ADC to temperature (thermistor)
     * @param raw Raw ADC value
     * @return Temperature in Celsius
     */
    float rawToTemperature(float raw) const;
};

#endif // ANALOG_SENSOR_H