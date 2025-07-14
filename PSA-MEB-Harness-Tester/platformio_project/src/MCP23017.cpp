#include "MCP23017.h"

MCP23017::MCP23017(uint8_t addr, int sda_pin, int scl_pin)
    : _addr(addr), _sda(sda_pin), _scl(scl_pin) {
}

bool MCP23017::begin() {
  // Initialize I2C pins
  pinMode(_sda, OUTPUT);
  pinMode(_scl, OUTPUT);
  digitalWrite(_sda, HIGH);
  digitalWrite(_scl, HIGH);
  delay(10);

  // Test connection
  if (!is_connected()) {
    return false;
  }

  // Default setup: all pins as inputs with pullups
  write_register(REG_IOCON, 0x00);    // Sequential mode
  write_register(REG_IODIRA, 0xFF);   // Port A inputs
  write_register(REG_IODIRB, 0xFF);   // Port B inputs
  write_register(REG_GPPUA, 0xFF);    // Port A pullups on
  write_register(REG_GPPUB, 0xFF);    // Port B pullups on

  return true;
}

bool MCP23017::is_connected() {
  i2c_start();
  bool ack = i2c_write_byte((_addr << 1) | 0);
  i2c_stop();
  return ack;
}

void MCP23017::write_port_a(uint8_t value) {
  write_register(REG_GPIOA, value);
}

void MCP23017::write_port_b(uint8_t value) {
  write_register(REG_GPIOB, value);
}

uint8_t MCP23017::read_port_a() {
  return read_register(REG_GPIOA);
}

uint8_t MCP23017::read_port_b() {
  return read_register(REG_GPIOB);
}

void MCP23017::set_port_a_as_inputs(bool pullups) {
  write_register(REG_IODIRA, 0xFF);  // All inputs
  write_register(REG_GPPUA, pullups ? 0xFF : 0x00);  // Pullups on/off
}

void MCP23017::set_port_b_as_inputs(bool pullups) {
  write_register(REG_IODIRB, 0xFF);  // All inputs
  write_register(REG_GPPUB, pullups ? 0xFF : 0x00);  // Pullups on/off
}

void MCP23017::set_port_a_as_outputs() {
  write_register(REG_IODIRA, 0x00);  // All outputs
  write_register(REG_GPPUA, 0x00);   // No pullups
}

void MCP23017::set_port_b_as_outputs() {
  write_register(REG_IODIRB, 0x00);  // All outputs
  write_register(REG_GPPUB, 0x00);   // No pullups
}

void MCP23017::pin_mode_a(uint8_t pin, uint8_t mode) {
  if (pin > 7) return;

  uint8_t current_dir = read_register(REG_IODIRA);
  uint8_t current_pullup = read_register(REG_GPPUA);

  if (mode == INPUT) {
    current_dir |= (1 << pin);      // Set as input
    current_pullup &= ~(1 << pin);  // Disable pullup
  } else if (mode == INPUT_PULLUP) {
    current_dir |= (1 << pin);      // Set as input
    current_pullup |= (1 << pin);   // Enable pullup
  } else { // OUTPUT
    current_dir &= ~(1 << pin);     // Set as output
    current_pullup &= ~(1 << pin);  // Disable pullup
  }

  write_register(REG_IODIRA, current_dir);
  write_register(REG_GPPUA, current_pullup);
}

void MCP23017::pin_mode_b(uint8_t pin, uint8_t mode) {
  if (pin > 7) return;

  uint8_t current_dir = read_register(REG_IODIRB);
  uint8_t current_pullup = read_register(REG_GPPUB);

  if (mode == INPUT) {
    current_dir |= (1 << pin);      // Set as input
    current_pullup &= ~(1 << pin);  // Disable pullup
  } else if (mode == INPUT_PULLUP) {
    current_dir |= (1 << pin);      // Set as input
    current_pullup |= (1 << pin);   // Enable pullup
  } else { // OUTPUT
    current_dir &= ~(1 << pin);     // Set as output
    current_pullup &= ~(1 << pin);  // Disable pullup
  }

  write_register(REG_IODIRB, current_dir);
  write_register(REG_GPPUB, current_pullup);
}

void MCP23017::digital_write_a(uint8_t pin, uint8_t value) {
  if (pin > 7) return;

  uint8_t current = read_register(REG_GPIOA);

  if (value == HIGH) {
    current |= (1 << pin);
  } else {
    current &= ~(1 << pin);
  }

  write_register(REG_GPIOA, current);
}

void MCP23017::digital_write_b(uint8_t pin, uint8_t value) {
  if (pin > 7) return;

  uint8_t current = read_register(REG_GPIOB);

  if (value == HIGH) {
    current |= (1 << pin);
  } else {
    current &= ~(1 << pin);
  }

  write_register(REG_GPIOB, current);
}

uint8_t MCP23017::digital_read_a(uint8_t pin) {
  if (pin > 7) return LOW;

  uint8_t port_value = read_register(REG_GPIOA);
  return (port_value & (1 << pin)) ? HIGH : LOW;
}

uint8_t MCP23017::digital_read_b(uint8_t pin) {
  if (pin > 7) return LOW;

  uint8_t port_value = read_register(REG_GPIOB);
  return (port_value & (1 << pin)) ? HIGH : LOW;
}

// Low-level I2C implementation
void MCP23017::i2c_start() {
  pinMode(_sda, OUTPUT);
  pinMode(_scl, OUTPUT);
  digitalWrite(_sda, HIGH);
  digitalWrite(_scl, HIGH);
  delayMicroseconds(5);
  digitalWrite(_sda, LOW);
  delayMicroseconds(5);
  digitalWrite(_scl, LOW);
  delayMicroseconds(5);
}

void MCP23017::i2c_stop() {
  pinMode(_sda, OUTPUT);
  pinMode(_scl, OUTPUT);
  digitalWrite(_sda, LOW);
  digitalWrite(_scl, LOW);
  delayMicroseconds(5);
  digitalWrite(_scl, HIGH);
  delayMicroseconds(5);
  digitalWrite(_sda, HIGH);
  delayMicroseconds(5);
}

bool MCP23017::i2c_write_byte(uint8_t data) {
  for (int i = 7; i >= 0; i--) {
    digitalWrite(_sda, (data >> i) & 1);
    delayMicroseconds(5);
    digitalWrite(_scl, HIGH);
    delayMicroseconds(5);
    digitalWrite(_scl, LOW);
    delayMicroseconds(5);
  }

  pinMode(_sda, INPUT_PULLUP);
  digitalWrite(_scl, HIGH);
  delayMicroseconds(5);
  bool ack = digitalRead(_sda) == LOW;
  digitalWrite(_scl, LOW);
  delayMicroseconds(5);
  pinMode(_sda, OUTPUT);

  return ack;
}

uint8_t MCP23017::i2c_read_byte(bool ack) {
  uint8_t data = 0;
  pinMode(_sda, INPUT_PULLUP);

  for (int i = 7; i >= 0; i--) {
    digitalWrite(_scl, HIGH);
    delayMicroseconds(5);
    if (digitalRead(_sda)) {
      data |= (1 << i);
    }
    digitalWrite(_scl, LOW);
    delayMicroseconds(5);
  }

  pinMode(_sda, OUTPUT);
  digitalWrite(_sda, ack ? LOW : HIGH);
  delayMicroseconds(5);
  digitalWrite(_scl, HIGH);
  delayMicroseconds(5);
  digitalWrite(_scl, LOW);
  delayMicroseconds(5);

  return data;
}

void MCP23017::write_register(uint8_t reg, uint8_t value) {
  i2c_start();
  i2c_write_byte((_addr << 1) | 0);
  i2c_write_byte(reg);
  i2c_write_byte(value);
  i2c_stop();
}

uint8_t MCP23017::read_register(uint8_t reg) {
  i2c_start();
  i2c_write_byte((_addr << 1) | 0);
  i2c_write_byte(reg);

  i2c_start();
  i2c_write_byte((_addr << 1) | 1);
  uint8_t value = i2c_read_byte(false);
  i2c_stop();

  return value;
}