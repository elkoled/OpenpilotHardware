#include <Arduino.h>
#include "MCP23017.h"

#define SDA1 8
#define SCL1 9
#define SDA2 0
#define SCL2 1

MCP23017 mcp1(0x20, SDA1, SCL1);
MCP23017 mcp2(0x21, SDA1, SCL1);
MCP23017 mcp3(0x22, SDA1, SCL1);
MCP23017 mcp4(0x23, SDA1, SCL1);
MCP23017 mcp5(0x24, SDA1, SCL1);
MCP23017 mcp6(0x25, SDA1, SCL1);
MCP23017 mcp7(0x26, SDA1, SCL1);
MCP23017 mcp8(0x27, SDA1, SCL1);

MCP23017 mcp9(0x20, SDA2, SCL2);

void setup() {
  Serial.begin(115200);
  delay(500);

  // Initialize all devices
  mcp1.begin();
  mcp5.begin();
  mcp9.begin();

  // Setup mcp1 as outputs (turn all pins off initially)
  mcp1.set_port_a_as_outputs();
  mcp1.set_port_b_as_outputs();
  mcp1.write_port_a(0x00);
  mcp1.write_port_b(0x00);

  // Setup mcp5 and mcp9 as inputs with pullups
  mcp5.set_port_a_as_inputs(true);
  mcp5.set_port_b_as_inputs(true);
  mcp9.set_port_a_as_inputs(true);
  mcp9.set_port_b_as_inputs(true);
}

void loop() {
  // Read all inputs
  uint8_t input_1 = mcp5.read_port_a();
  uint8_t input_2 = mcp5.read_port_b();
  uint8_t input_3 = mcp9.read_port_a();
  uint8_t input_4 = mcp9.read_port_b();

  // Print same format as before
  Serial.print("[");
  for (int i = 7; i >= 0; i--) {
    Serial.print((input_2 >> i) & 1);
    Serial.print(",");
  }
  for (int i = 7; i >= 0; i--) {
    Serial.print((input_1 >> i) & 1);
    if (i > 0) Serial.print(",");
  }
  Serial.print("] [");
  for (int i = 7; i >= 0; i--) {
    Serial.print((input_4 >> i) & 1);
    Serial.print(",");
  }
  for (int i = 7; i >= 0; i--) {
    Serial.print((input_3 >> i) & 1);
    if (i > 0) Serial.print(",");
  }
  Serial.println("]");

  delay(500);
}

// Example using individual pins (just like regular Arduino)
void example_individual_pins() {
  // Set pin 3 on port A as output
  mcp1.pin_mode_a(3, OUTPUT);
  mcp1.digital_write_a(3, HIGH);  // Turn on LED

  // Set pin 5 on port B as input with pullup
  mcp4.pin_mode_b(5, INPUT_PULLUP);

  // Read button on pin 5
  if (mcp4.digital_read_b(5) == LOW) {
    Serial.println("Button pressed!");
  }
}

// Example blinking LED on expander
void example_blink() {
  mcp1.set_port_a_as_outputs();

  while (true) {
    mcp1.write_port_a(0xFF);  // All LEDs on
    delay(500);
    mcp1.write_port_a(0x00);  // All LEDs off
    delay(500);
  }
}