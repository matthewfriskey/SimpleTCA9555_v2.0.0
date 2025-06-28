#ifndef SIMPLETCA9555_H
#define SIMPLETCA9555_H

#include <Arduino.h>
#include <Wire.h>

/**
 * @class SimpleTCA9555
 * @brief A simple library to control the TCA9555 I/O expander in input mode with interrupt support.
 *
 * This library provides access to all 16 inputs (P00-P07 on Port 0, P10-P17 on Port 1) as public
 * boolean variables (`input_1` to `input_16`). Inputs can be renamed using `#define` for readability
 * (e.g., `#define reed_switch expander.input_1`). Supports interrupt-driven updates and adjustable
 * I2C clock speed.
 */
class SimpleTCA9555
{
public:
    /**
     * @brief Constructor for the SimpleTCA9555 class.
     * @param address The I2C address of the TCA9555 (typically 0x20 to 0x27).
     */
    SimpleTCA9555(uint8_t address);

    /**
     * @brief Initializes the I2C bus and configures the TCA9555 for input mode.
     * @param sda The SDA pin for I2C (default: -1 uses ESP32 default SDA pin).
     * @param scl The SCL pin for I2C (default: -1 uses ESP32 default SCL pin).
     * @param interruptPin The ESP32 GPIO pin connected to TCA9555 INT (default: -1 disables interrupts).
     * @param clockSpeed I2C clock speed in Hz (default: 100000, max 400000 for TCA9555).
     */
    void begin(int sda = -1, int scl = -1, int interruptPin = -1, uint32_t clockSpeed = 100000);

    /**
     * @brief Manually updates the internal state by reading all inputs from the TCA9555.
     */
    void update();

    // Methods for output control
    void setOutputPin(uint8_t pin, bool state);
    void setPortAsOutput(uint8_t portNumber);
    void setPortAsInput(uint8_t portNumber);

    // Public boolean variables for each input
    bool input_1;  // P00
    bool input_2;  // P01
    bool input_3;  // P02
    bool input_4;  // P03
    bool input_5;  // P04
    bool input_6;  // P05
    bool input_7;  // P06
    bool input_8;  // P07
    bool input_9;  // P10
    bool input_10; // P11
    bool input_11; // P12
    bool input_12; // P13
    bool input_13; // P14
    bool input_14; // P15
    bool input_15; // P16
    bool input_16; // P17

private:
    uint8_t _address;
    volatile uint16_t _inputs;
    uint16_t _outputs;
    uint16_t _config = 0xFFFF; // Default all inputs
    int _interruptPin;

    void updateConfig();
    void writeOutputs();

    static void IRAM_ATTR interruptHandler();
    static SimpleTCA9555 *instance;
    void updateFromISR();
};

#endif