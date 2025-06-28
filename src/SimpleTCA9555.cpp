#include "SimpleTCA9555.h"

SimpleTCA9555 *SimpleTCA9555::instance = nullptr;

SimpleTCA9555::SimpleTCA9555(uint8_t address)
    : _address(address), _inputs(0), _outputs(0), _interruptPin(-1)
{
    instance = this;
    input_1 = input_2 = input_3 = input_4 = input_5 = input_6 = input_7 = input_8 =
        input_9 = input_10 = input_11 = input_12 = input_13 = input_14 = input_15 = input_16 = false;
}

void SimpleTCA9555::begin(int sda, int scl, int interruptPin, uint32_t clockSpeed)
{
    _interruptPin = interruptPin;

    if (sda != -1 && scl != -1)
        Wire.begin(sda, scl);
    else
        Wire.begin();

    Wire.setClock(clockSpeed);
    updateConfig();

    if (_interruptPin != -1)
    {
        pinMode(_interruptPin, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(_interruptPin), interruptHandler, FALLING);
    }

    _outputs = 0x0000;
    writeOutputs();
    update();
}

void SimpleTCA9555::updateConfig()
{
    Wire.beginTransmission(_address);
    Wire.write(0x06);
    Wire.write(_config & 0xFF);
    Wire.write((_config >> 8) & 0xFF);
    Wire.endTransmission();
}

void SimpleTCA9555::writeOutputs()
{
    Wire.beginTransmission(_address);
    Wire.write(0x02);
    Wire.write(_outputs & 0xFF);
    Wire.write((_outputs >> 8) & 0xFF);
    Wire.endTransmission();
}

void SimpleTCA9555::setOutputPin(uint8_t pin, bool state)
{
    if (pin >= 1 && pin <= 16)
    {
        if (state)
            _outputs |= (1 << (pin - 1));
        else
            _outputs &= ~(1 << (pin - 1));
        writeOutputs();
    }
}

void SimpleTCA9555::setPortAsOutput(uint8_t portNumber)
{
    if (portNumber == 0)
        _config &= 0xFF00;
    else if (portNumber == 1)
        _config &= 0x00FF;
    updateConfig();
}

void SimpleTCA9555::setPortAsInput(uint8_t portNumber)
{
    if (portNumber == 0)
        _config |= 0x00FF;
    else if (portNumber == 1)
        _config |= 0xFF00;
    updateConfig();
}

void SimpleTCA9555::update()
{
    Wire.beginTransmission(_address);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.requestFrom(_address, (uint8_t)2);
    uint8_t port0 = Wire.read();
    uint8_t port1 = Wire.read();

    _inputs = (port1 << 8) | port0;

    input_1 = _inputs & (1 << 0);
    input_2 = _inputs & (1 << 1);
    input_3 = _inputs & (1 << 2);
    input_4 = _inputs & (1 << 3);
    input_5 = _inputs & (1 << 4);
    input_6 = _inputs & (1 << 5);
    input_7 = _inputs & (1 << 6);
    input_8 = _inputs & (1 << 7);
    input_9 = _inputs & (1 << 8);
    input_10 = _inputs & (1 << 9);
    input_11 = _inputs & (1 << 10);
    input_12 = _inputs & (1 << 11);
    input_13 = _inputs & (1 << 12);
    input_14 = _inputs & (1 << 13);
    input_15 = _inputs & (1 << 14);
    input_16 = _inputs & (1 << 15);
}

void SimpleTCA9555::updateFromISR()
{
    update();
}

void IRAM_ATTR SimpleTCA9555::interruptHandler()
{
    if (instance != nullptr)
        instance->updateFromISR();
}