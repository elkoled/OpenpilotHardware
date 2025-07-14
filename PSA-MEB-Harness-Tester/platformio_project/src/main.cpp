#include <Arduino.h>
#include <U8g2lib.h>
#include <FastLED.h>
#include <Wire.h>

// OLED - 72x40 Pixel, Offset (30,12): GPIO5=SDA, GPIO6=SCL (I2C0 - Hardware I2C)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, U8X8_PIN_NONE, 6, 5);  // SCL, SDA

// LED: GPIO2
#define LED_PIN 2
CRGB leds[1];

// I2C Pin definitions
#define I2C1_SDA 8
#define I2C1_SCL 9
#define I2C2_SDA 0
#define I2C2_SCL 1

// Manual I2C functions for MCP23017
class SimpleMCP23017 {
private:
  uint8_t _addr;
  int _sda;
  int _scl;

  void i2c_start() {
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

  void i2c_stop() {
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

  bool i2c_write_byte(uint8_t data) {
    for (int i = 7; i >= 0; i--) {
      digitalWrite(_sda, (data >> i) & 1);
      delayMicroseconds(5);
      digitalWrite(_scl, HIGH);
      delayMicroseconds(5);
      digitalWrite(_scl, LOW);
      delayMicroseconds(5);
    }

    // Check ACK
    pinMode(_sda, INPUT_PULLUP);
    digitalWrite(_scl, HIGH);
    delayMicroseconds(5);
    bool ack = digitalRead(_sda) == LOW;
    digitalWrite(_scl, LOW);
    delayMicroseconds(5);
    pinMode(_sda, OUTPUT);

    return ack;
  }

  uint8_t i2c_read_byte(bool ack) {
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

    // Send ACK/NACK
    pinMode(_sda, OUTPUT);
    digitalWrite(_sda, ack ? LOW : HIGH);
    delayMicroseconds(5);
    digitalWrite(_scl, HIGH);
    delayMicroseconds(5);
    digitalWrite(_scl, LOW);
    delayMicroseconds(5);

    return data;
  }

  void writeRegister(uint8_t reg, uint8_t value) {
    i2c_start();
    i2c_write_byte((_addr << 1) | 0);  // Write mode
    i2c_write_byte(reg);
    i2c_write_byte(value);
    i2c_stop();
  }

  uint8_t readRegister(uint8_t reg) {
    i2c_start();
    i2c_write_byte((_addr << 1) | 0);  // Write mode
    i2c_write_byte(reg);

    i2c_start();  // Repeated start
    i2c_write_byte((_addr << 1) | 1);  // Read mode
    uint8_t value = i2c_read_byte(false);  // NACK
    i2c_stop();

    return value;
  }

public:
  SimpleMCP23017(uint8_t addr, int sda, int scl) : _addr(addr), _sda(sda), _scl(scl) {}

  void begin() {
    pinMode(_sda, OUTPUT);
    pinMode(_scl, OUTPUT);
    digitalWrite(_sda, HIGH);
    digitalWrite(_scl, HIGH);
    delay(10);

    // Initialize MCP23017
    writeRegister(0x0A, 0x00);  // IOCON: Sequential mode, no mirroring
    writeRegister(0x00, 0xFF);  // IODIRA: All inputs
    writeRegister(0x01, 0xFF);  // IODIRB: All inputs
    writeRegister(0x0C, 0xFF);  // GPPUA: Enable pullups on port A
    writeRegister(0x0D, 0xFF);  // GPPUB: Enable pullups on port B
  }

  uint8_t readPortA() {
    return readRegister(0x12);  // GPIOA
  }

  uint8_t readPortB() {
    return readRegister(0x13);  // GPIOB
  }

  void writePortA(uint8_t value) {
    writeRegister(0x12, value);  // GPIOA
  }

  void writePortB(uint8_t value) {
    writeRegister(0x13, value);  // GPIOB
  }

  // Test if device responds
  bool testConnection() {
    i2c_start();
    bool ack = i2c_write_byte((_addr << 1) | 0);  // Write mode
    i2c_stop();
    return ack;
  }
};

// MCP23017 instances using manual I2C
SimpleMCP23017 mcp1_0(0x20, I2C1_SDA, I2C1_SCL);
SimpleMCP23017 mcp1_1(0x21, I2C1_SDA, I2C1_SCL);
SimpleMCP23017 mcp1_2(0x22, I2C1_SDA, I2C1_SCL);
SimpleMCP23017 mcp1_3(0x23, I2C1_SDA, I2C1_SCL);
SimpleMCP23017 mcp1_4(0x24, I2C1_SDA, I2C1_SCL);
SimpleMCP23017 mcp1_5(0x25, I2C1_SDA, I2C1_SCL);
SimpleMCP23017 mcp1_6(0x26, I2C1_SDA, I2C1_SCL);
SimpleMCP23017 mcp1_7(0x27, I2C1_SDA, I2C1_SCL);

SimpleMCP23017 mcp2_0(0x20, I2C2_SDA, I2C2_SCL);

SimpleMCP23017* mcps_bus1[] = {&mcp1_0, &mcp1_1, &mcp1_2, &mcp1_3, &mcp1_4, &mcp1_5, &mcp1_6, &mcp1_7};
SimpleMCP23017* mcps_bus2[] = {&mcp2_0};

void setup() {
  delay(3000);  // wait for USB CDC
  Serial.begin(115200);
  delay(100);
  Serial.println("Booting...");

  // LED test
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, 1);
  FastLED.setBrightness(20);
  leds[0] = CRGB::Blue;
  FastLED.show();
  Serial.println("LED ON");

  // OLED test (Hardware I2C)
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x12_tr);
  u8g2.clearBuffer();
  u8g2.drawStr(0, 20, "OLED OK");
  u8g2.sendBuffer();
  Serial.println("OLED initialized");

  // Initialize MCP23017 chips on Bus 1
  Serial.println("Initializing MCP23017 chips on Bus 1...");
  for (int i = 0; i < 8; i++) {
    Serial.print("MCP Bus1 @0x");
    Serial.print(0x20 + i, HEX);
    Serial.print(" test connection... ");

    if (mcps_bus1[i]->testConnection()) {
      Serial.print("found, initializing... ");
      mcps_bus1[i]->begin();
      Serial.println("OK");
    } else {
      Serial.println("not found");
    }
    delay(10);
  }

  // Initialize MCP23017 chip on Bus 2
  Serial.println("Initializing MCP23017 chip on Bus 2...");
  Serial.print("MCP Bus2 @0x20 test connection... ");

  if (mcp2_0.testConnection()) {
    Serial.print("found, initializing... ");
    mcp2_0.begin();
    Serial.println("OK");
  } else {
    Serial.println("not found");
  }

  Serial.println("All MCP23017 chips initialized!");
  leds[0] = CRGB::Green;
  FastLED.show();
}

void readAllMCPs() {
  Serial.println("Reading all MCP23017 ports");

  // Read Bus 1 MCPs
  Serial.println("Bus 1 (GPIO8/9):");
  for (int i = 0; i < 8; i++) {
    if (mcps_bus1[i]->testConnection()) {
      uint8_t portA = mcps_bus1[i]->readPortA();
      uint8_t portB = mcps_bus1[i]->readPortB();

      Serial.print("  MCP@0x");
      Serial.print(0x20 + i, HEX);
      Serial.print(" - PortA: 0x");
      Serial.print(portA, HEX);
      Serial.print(", PortB: 0x");
      Serial.println(portB, HEX);
    }
  }

  // Read Bus 2 MCP
  Serial.println("Bus 2 (GPIO6/7):");
  if (mcp2_0.testConnection()) {
    uint8_t portA = mcp2_0.readPortA();
    uint8_t portB = mcp2_0.readPortB();

    Serial.print("  MCP@0x20 - PortA: 0x");
    Serial.print(portA, HEX);
    Serial.print(", PortB: 0x");
    Serial.println(portB, HEX);
  }
}

void loop() {
  static unsigned long lastRead = 0;

  if (millis() - lastRead > 2000) {
    readAllMCPs();
    lastRead = millis();

    static bool toggle = false;
    leds[0] = toggle ? CRGB::Green : CRGB::Blue;
    FastLED.show();
    toggle = !toggle;
  }

  u8g2.clearBuffer();
  u8g2.drawStr(0, 12, "MCP23017 Test");
  u8g2.drawStr(0, 24, "Bus1: 8 chips");
  u8g2.drawStr(0, 36, "Bus2: 1 chip");
  u8g2.drawStr(0, 48, "Manual I2C");
  u8g2.sendBuffer();

  delay(100);
}