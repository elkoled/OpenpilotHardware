#include <Arduino.h>
#include "MCP23017.h"

// I2C pins
#define SDA1 8
#define SCL1 9
#define SDA2 0
#define SCL2 1

// Plug side - inputs (directly tests pins 1-60)
MCP23017 U1(0x20, SDA1, SCL1);  // Pins 1-16
MCP23017 U2(0x21, SDA1, SCL1);  // Pins 17-32
MCP23017 U3(0x22, SDA1, SCL1);  // Pins 33-48
MCP23017 U4(0x23, SDA1, SCL1);  // Pins 49-60

// Socket side - outputs driving LOW (pins 1a-60a)
MCP23017 U5(0x24, SDA1, SCL1);  // Pins 1a-16a
MCP23017 U6(0x25, SDA1, SCL1);  // Pins 17a-32a
MCP23017 U7(0x26, SDA1, SCL1);  // Pins 33a-48a
MCP23017 U8(0x27, SDA1, SCL1);  // Pins 49a-60a + USB pins 61-64

// USB-C connector - inputs
MCP23017 U9(0x20, SDA2, SCL2);  // USB pins 65-79

void setup() {
  Serial.begin(115200);
  delay(2000);

  // Plug side (U1-U4): inputs with pullups
  U1.begin(); U1.set_port_a_as_inputs(true); U1.set_port_b_as_inputs(true);
  U2.begin(); U2.set_port_a_as_inputs(true); U2.set_port_b_as_inputs(true);
  U3.begin(); U3.set_port_a_as_inputs(true); U3.set_port_b_as_inputs(true);
  U4.begin(); U4.set_port_a_as_inputs(true); U4.set_port_b_as_inputs(true);

  // Socket side (U5-U8): outputs driving LOW
  U5.begin(); U5.set_port_a_as_outputs(); U5.set_port_b_as_outputs();
  U6.begin(); U6.set_port_a_as_outputs(); U6.set_port_b_as_outputs();
  U7.begin(); U7.set_port_a_as_outputs(); U7.set_port_b_as_outputs();
  U8.begin(); U8.set_port_b_as_outputs();  // 49a-56a
  // U8 port A: bits 0-3 = outputs (57a-60a), bits 4-7 = inputs (61-64)
  for (int i = 0; i < 4; i++) U8.pin_mode_a(i, OUTPUT);
  for (int i = 4; i < 8; i++) U8.pin_mode_a(i, INPUT);
  U5.write_port_a(0); U5.write_port_b(0);
  U6.write_port_a(0); U6.write_port_b(0);
  U7.write_port_a(0); U7.write_port_b(0);
  U8.write_port_b(0);
  for (int i = 0; i < 4; i++) U8.digital_write_a(i, LOW);

  // USB-C (U9): inputs with pullups
  U9.begin(); U9.set_port_a_as_inputs(true); U9.set_port_b_as_inputs(true);

  Serial.println("Cable Tester Ready");
}

void loop() {
  // Test 1: All LOW
  Serial.println("\n=== ALL LOW ===");
  U5.write_port_b(0x00); U5.write_port_a(0x00);
  U6.write_port_b(0x00); U6.write_port_a(0x00);
  U7.write_port_b(0x00); U7.write_port_a(0x00);
  U8.write_port_b(0x00);
  for (int i = 0; i < 4; i++) U8.digital_write_a(i, LOW);
  delay(10);
  Serial.printf("U1: B=%02X A=%02X\n", U1.read_port_b(), U1.read_port_a());
  Serial.printf("U2: B=%02X A=%02X\n", U2.read_port_b(), U2.read_port_a());
  Serial.printf("U3: B=%02X A=%02X\n", U3.read_port_b(), U3.read_port_a());
  Serial.printf("U4: B=%02X A=%02X\n", U4.read_port_b(), U4.read_port_a());

  // Test 2: All HIGH
  Serial.println("\n=== ALL HIGH ===");
  U5.write_port_b(0xFF); U5.write_port_a(0xFF);
  U6.write_port_b(0xFF); U6.write_port_a(0xFF);
  U7.write_port_b(0xFF); U7.write_port_a(0xFF);
  U8.write_port_b(0xFF);
  for (int i = 0; i < 4; i++) U8.digital_write_a(i, HIGH);
  delay(10);
  Serial.printf("U1: B=%02X A=%02X\n", U1.read_port_b(), U1.read_port_a());
  Serial.printf("U2: B=%02X A=%02X\n", U2.read_port_b(), U2.read_port_a());
  Serial.printf("U3: B=%02X A=%02X\n", U3.read_port_b(), U3.read_port_a());
  Serial.printf("U4: B=%02X A=%02X\n", U4.read_port_b(), U4.read_port_a());

  // Test 3: Pattern 0xAA (10101010)
  Serial.println("\n=== PATTERN 0xAA ===");
  U5.write_port_b(0xAA); U5.write_port_a(0xAA);
  U6.write_port_b(0xAA); U6.write_port_a(0xAA);
  U7.write_port_b(0xAA); U7.write_port_a(0xAA);
  U8.write_port_b(0xAA);
  delay(10);
  Serial.printf("U1: B=%02X A=%02X (expect AA)\n", U1.read_port_b(), U1.read_port_a());
  Serial.printf("U2: B=%02X A=%02X (expect AA)\n", U2.read_port_b(), U2.read_port_a());
  Serial.printf("U3: B=%02X A=%02X (expect AA)\n", U3.read_port_b(), U3.read_port_a());
  Serial.printf("U4: B=%02X A=%02X (expect AA)\n", U4.read_port_b(), U4.read_port_a());

  // Test 4: Pattern 0x55 (01010101)
  Serial.println("\n=== PATTERN 0x55 ===");
  U5.write_port_b(0x55); U5.write_port_a(0x55);
  U6.write_port_b(0x55); U6.write_port_a(0x55);
  U7.write_port_b(0x55); U7.write_port_a(0x55);
  U8.write_port_b(0x55);
  delay(10);
  Serial.printf("U1: B=%02X A=%02X (expect 55)\n", U1.read_port_b(), U1.read_port_a());
  Serial.printf("U2: B=%02X A=%02X (expect 55)\n", U2.read_port_b(), U2.read_port_a());
  Serial.printf("U3: B=%02X A=%02X (expect 55)\n", U3.read_port_b(), U3.read_port_a());
  Serial.printf("U4: B=%02X A=%02X (expect 55)\n", U4.read_port_b(), U4.read_port_a());

  delay(5000);
}
