#pragma once
#include <Arduino.h>

class MCP23017 {
private:
  // MCP23017 Register addresses
  static constexpr uint8_t REG_IODIRA = 0x00;
  static constexpr uint8_t REG_IODIRB = 0x01;
  static constexpr uint8_t REG_GPPUA = 0x0C;
  static constexpr uint8_t REG_GPPUB = 0x0D;
  static constexpr uint8_t REG_GPIOA = 0x12;
  static constexpr uint8_t REG_GPIOB = 0x13;
  static constexpr uint8_t REG_IOCON = 0x0A;

  uint8_t _addr;
  int _sda;
  int _scl;

  // Low-level I2C functions
  void i2c_start();
  void i2c_stop();
  bool i2c_write_byte(uint8_t data);
  uint8_t i2c_read_byte(bool ack);
  void write_register(uint8_t reg, uint8_t value);
  uint8_t read_register(uint8_t reg);

public:
  MCP23017(uint8_t addr, int sda_pin, int scl_pin);

  // Simple initialization
  bool begin();
  bool is_connected();

  // Simple port functions - just like Arduino digitalRead/Write but for 8 pins at once
  void write_port_a(uint8_t value);
  void write_port_b(uint8_t value);
  uint8_t read_port_a();
  uint8_t read_port_b();

  // Simple port configuration
  void set_port_a_as_inputs(bool pullups = true);
  void set_port_b_as_inputs(bool pullups = true);
  void set_port_a_as_outputs();
  void set_port_b_as_outputs();

  // Individual pin functions (0-7)
  void pin_mode_a(uint8_t pin, uint8_t mode);  // Use Arduino INPUT/OUTPUT
  void pin_mode_b(uint8_t pin, uint8_t mode);
  void digital_write_a(uint8_t pin, uint8_t value);  // Use Arduino HIGH/LOW
  void digital_write_b(uint8_t pin, uint8_t value);
  uint8_t digital_read_a(uint8_t pin);
  uint8_t digital_read_b(uint8_t pin);

  // Legacy compatibility (so old code still works)
  void writePortA(uint8_t value) { write_port_a(value); }
  void writePortB(uint8_t value) { write_port_b(value); }
  uint8_t readPortA() { return read_port_a(); }
  uint8_t readPortB() { return read_port_b(); }
};