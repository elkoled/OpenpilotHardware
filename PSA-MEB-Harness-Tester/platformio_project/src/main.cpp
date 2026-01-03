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
  static unsigned long last = 0;
  if (millis() - last < 500) return;
  last = millis();

  // Read plug side pins 1-60 (U1-U4)
  uint8_t u1b = U1.read_port_b(), u1a = U1.read_port_a();  // 1-16
  uint8_t u2b = U2.read_port_b(), u2a = U2.read_port_a();  // 17-32
  uint8_t u3b = U3.read_port_b(), u3a = U3.read_port_a();  // 33-48
  uint8_t u4b = U4.read_port_b(), u4a = U4.read_port_a();  // 49-60

  // Read USB-C pins 61-79 (U8 + U9)
  uint8_t u8a = U8.read_port_a();  // 61-64 (bits 4-7)
  uint8_t u9b = U9.read_port_b();  // 65-72
  uint8_t u9a = U9.read_port_a();  // 73-79 (bit 7 = NC)

  // Print pins 1-60
  Serial.print("[");
  for (int i = 0; i < 8; i++) Serial.print((u1b >> i) & 1);  // 1-8
  for (int i = 0; i < 8; i++) Serial.print((u1a >> i) & 1);  // 9-16
  for (int i = 0; i < 8; i++) Serial.print((u2b >> i) & 1);  // 17-24
  for (int i = 0; i < 8; i++) Serial.print((u2a >> i) & 1);  // 25-32
  for (int i = 0; i < 8; i++) Serial.print((u3b >> i) & 1);  // 33-40
  for (int i = 0; i < 8; i++) Serial.print((u3a >> i) & 1);  // 41-48
  for (int i = 0; i < 8; i++) Serial.print((u4b >> i) & 1);  // 49-56
  for (int i = 0; i < 4; i++) Serial.print((u4a >> i) & 1);  // 57-60

  // Print USB pins 61-79
  Serial.print("|");
  for (int i = 4; i < 8; i++) Serial.print((u8a >> i) & 1);  // 61-64
  for (int i = 0; i < 8; i++) Serial.print((u9b >> i) & 1);  // 65-72
  for (int i = 0; i < 7; i++) Serial.print((u9a >> i) & 1);  // 73-79

  Serial.println("]");
}
