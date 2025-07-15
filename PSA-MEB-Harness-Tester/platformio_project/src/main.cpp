#include <Arduino.h>
#include "MCP23017.h"

#define SDA1 8
#define SCL1 9
#define SDA2 0
#define SCL2 1

MCP23017 mcp[] = {
    MCP23017(0x20, SDA1, SCL1), // MCP1 - index 0
    MCP23017(0x21, SDA1, SCL1), // MCP2 - index 1
    MCP23017(0x22, SDA1, SCL1), // MCP3 - index 2
    MCP23017(0x23, SDA1, SCL1), // MCP4 - index 3
    MCP23017(0x24, SDA1, SCL1), // MCP5 - index 4
    MCP23017(0x25, SDA1, SCL1), // MCP6 - index 5
    MCP23017(0x26, SDA1, SCL1), // MCP7 - index 6
    MCP23017(0x27, SDA1, SCL1), // MCP8 - index 7
    MCP23017(0x20, SDA2, SCL2), // MCP9 - index 8
};

const int NUM_MCP = sizeof(mcp) / sizeof(MCP23017);

// Structure to hold pin mapping information
struct PinMapping {
  int mcp_index; // Which MCP chip (0-8)
  char port;     // 'A' or 'B'
  int pin;       // Pin number (0-7)
  bool valid;    // Is this pin connected?
};

// Lookup table: Pin -> MCP info
// Index = Pin number, Value = PinMapping struct
PinMapping pin_map[] = {
    {-1, 'X', 0, false}, // Index 0 - no pin 0
    {0, 'B', 0, true},   // Pin 1  -> MCP1 GPB0
    {0, 'A', 2, true},   // Pin 2  -> MCP1 GPA2
    {0, 'B', 1, true},   // Pin 3  -> MCP1 GPB1
    {0, 'A', 3, true},   // Pin 4  -> MCP1 GPA3
    {0, 'B', 2, true},   // Pin 5  -> MCP1 GPB2
    {0, 'A', 4, true},   // Pin 6  -> MCP1 GPA4
    {0, 'B', 3, true},   // Pin 7  -> MCP1 GPB3
    {0, 'A', 5, true},   // Pin 8  -> MCP1 GPA5
    {0, 'B', 4, true},   // Pin 9  -> MCP1 GPB4
    {0, 'A', 6, true},   // Pin 10 -> MCP1 GPA6
    {1, 'B', 3, true},   // Pin 11 -> MCP2 GPB3
    {0, 'A', 1, true},   // Pin 12 -> MCP1 GPA1
    {1, 'B', 2, true},   // Pin 13 -> MCP2 GPB2
    {0, 'A', 0, true},   // Pin 14 -> MCP1 GPA0
    {1, 'B', 1, true},   // Pin 15 -> MCP2 GPB1
    {0, 'B', 7, true},   // Pin 16 -> MCP1 GPB7
    {1, 'B', 0, true},   // Pin 17 -> MCP2 GPB0
    {0, 'B', 6, true},   // Pin 18 -> MCP1 GPB6
    {0, 'A', 7, true},   // Pin 19 -> MCP1 GPA7
    {0, 'B', 5, true},   // Pin 20 -> MCP1 GPB5
    {1, 'B', 4, true},   // Pin 21 -> MCP2 GPB4
    {1, 'A', 6, true},   // Pin 22 -> MCP2 GPA6
    {1, 'B', 5, true},   // Pin 23 -> MCP2 GPB5
    {1, 'A', 7, true},   // Pin 24 -> MCP2 GPA7
    {1, 'B', 6, true},   // Pin 25 -> MCP2 GPB6
    {2, 'B', 0, true},   // Pin 26 -> MCP3 GPB0
    {1, 'B', 7, true},   // Pin 27 -> MCP2 GPB7
    {2, 'B', 1, true},   // Pin 28 -> MCP3 GPB1
    {1, 'A', 0, true},   // Pin 29 -> MCP2 GPA0
    {2, 'B', 2, true},   // Pin 30 -> MCP3 GPB2
    {2, 'B', 7, true},   // Pin 31 -> MCP3 GPB7
    {1, 'A', 5, true},   // Pin 32 -> MCP2 GPA5
    {2, 'B', 6, true},   // Pin 33 -> MCP3 GPB6
    {1, 'A', 4, true},   // Pin 34 -> MCP2 GPA4
    {2, 'B', 5, true},   // Pin 35 -> MCP3 GPB5
    {1, 'A', 3, true},   // Pin 36 -> MCP2 GPA3
    {2, 'B', 4, true},   // Pin 37 -> MCP3 GPB4
    {1, 'A', 2, true},   // Pin 38 -> MCP2 GPA2
    {2, 'B', 3, true},   // Pin 39 -> MCP3 GPB3
    {1, 'A', 1, true},   // Pin 40 -> MCP2 GPA1
    {2, 'A', 0, true},   // Pin 41 -> MCP3 GPA0
    {3, 'B', 2, true},   // Pin 42 -> MCP4 GPB2
    {2, 'A', 1, true},   // Pin 43 -> MCP3 GPA1
    {3, 'B', 3, true},   // Pin 44 -> MCP4 GPB3
    {2, 'A', 2, true},   // Pin 45 -> MCP3 GPA2
    {3, 'B', 4, true},   // Pin 46 -> MCP4 GPB4
    {2, 'A', 3, true},   // Pin 47 -> MCP3 GPA3
    {3, 'B', 5, true},   // Pin 48 -> MCP4 GPB5
    {2, 'A', 4, true},   // Pin 49 -> MCP3 GPA4
    {3, 'B', 6, true},   // Pin 50 -> MCP4 GPB6
    {3, 'A', 3, true},   // Pin 51 -> MCP4 GPA3
    {3, 'B', 1, true},   // Pin 52 -> MCP4 GPB1
    {3, 'A', 2, true},   // Pin 53 -> MCP4 GPA2
    {3, 'B', 0, true},   // Pin 54 -> MCP4 GPB0
    {3, 'A', 1, true},   // Pin 55 -> MCP4 GPA1
    {2, 'A', 7, true},   // Pin 56 -> MCP3 GPA7
    {3, 'A', 0, true},   // Pin 57 -> MCP4 GPA0
    {2, 'A', 6, true},   // Pin 58 -> MCP3 GPA6
    {3, 'B', 7, true},   // Pin 59 -> MCP4 GPB7
    {2, 'A', 5, true},   // Pin 60 -> MCP3 GPA5
    {7, 'A', 4, true},   // Pin 61 -> MCP8 GPA4
    {7, 'A', 5, true},   // Pin 62 -> MCP8 GPA5
    {7, 'A', 6, true},   // Pin 63 -> MCP8 GPA6
    {7, 'A', 7, true},   // Pin 64 -> MCP8 GPA7
    {8, 'B', 0, true},   // Pin 65 -> MCP9 GPB0
    {8, 'B', 1, true},   // Pin 66 -> MCP9 GPB1
    {8, 'B', 2, true},   // Pin 67 -> MCP9 GPB2
    {8, 'B', 3, true},   // Pin 68 -> MCP9 GPB3
    {8, 'B', 4, true},   // Pin 69 -> MCP9 GPB4
    {8, 'B', 5, true},   // Pin 70 -> MCP9 GPB5
    {8, 'B', 6, true},   // Pin 71 -> MCP9 GPB6
    {8, 'B', 7, true},   // Pin 72 -> MCP9 GPB7
    {8, 'A', 0, true},   // Pin 73 -> MCP9 GPA0
    {8, 'A', 1, true},   // Pin 74 -> MCP9 GPA1
    {8, 'A', 2, true},   // Pin 75 -> MCP9 GPA2
    {8, 'A', 3, true},   // Pin 76 -> MCP9 GPA3
    {8, 'A', 4, true},   // Pin 77 -> MCP9 GPA4
    {8, 'A', 5, true},   // Pin 78 -> MCP9 GPA5
    {8, 'A', 6, true},   // Pin 79 -> MCP9 GPA6
};

const int MIN_PIN = 1;
const int MAX_PIN = 60;
const int MIN_USB = 61;
const int MAX_USB = 79;

// Helper function to get pin mapping
PinMapping *getPinMapping(int pin) {
  if (pin < 1 || pin > MAX_PIN) {
    return nullptr;
  }

  PinMapping *mapping = &pin_map[pin];
  if (!mapping->valid) {
    return nullptr;
  }

  return mapping;
}

// API functions
void digitalWritePin(int pin, int value) {
  PinMapping *mapping = getPinMapping(pin);
  if (!mapping)
    return;

  if (mapping->port == 'A') {
    mcp[mapping->mcp_index].digital_write_a(mapping->pin, value);
  } else {
    mcp[mapping->mcp_index].digital_write_b(mapping->pin, value);
  }
}

int digitalReadPin(int pin) {
  PinMapping *mapping = getPinMapping(pin);
  if (!mapping)
    return LOW;

  if (mapping->port == 'A') {
    return mcp[mapping->mcp_index].digital_read_a(mapping->pin);
  } else {
    return mcp[mapping->mcp_index].digital_read_b(mapping->pin);
  }
}

void pinModePin(int pin, int mode) {
  PinMapping *mapping = getPinMapping(pin);
  if (!mapping)
    return;

  if (mapping->port == 'A') {
    mcp[mapping->mcp_index].pin_mode_a(mapping->pin, mode);
  } else {
    mcp[mapping->mcp_index].pin_mode_b(mapping->pin, mode);
  }
}

// plug specific functions
void digitalWritePlugPin(int pin, int value) {
  PinMapping *mapping = getPinMapping(pin);
  if (!mapping)
    return;

  int plug_mcp_index = mapping->mcp_index + 4;
  if (plug_mcp_index >= NUM_MCP)
    return;

  if (mapping->port == 'A') {
    mcp[plug_mcp_index].digital_write_a(mapping->pin, value);
  } else {
    mcp[plug_mcp_index].digital_write_b(mapping->pin, value);
  }
}

int digitalReadPlugPin(int pin) {
  PinMapping *mapping = getPinMapping(pin);
  if (!mapping)
    return LOW;

  int plug_mcp_index = mapping->mcp_index + 4;
  if (plug_mcp_index >= NUM_MCP)
    return LOW;

  if (mapping->port == 'A') {
    return mcp[plug_mcp_index].digital_read_a(mapping->pin);
  } else {
    return mcp[plug_mcp_index].digital_read_b(mapping->pin);
  }
}

void pinModePlugPin(int pin, int mode) {
  PinMapping *mapping = getPinMapping(pin);
  if (!mapping)
    return;

  int plug_mcp_index = mapping->mcp_index + 4;
  if (plug_mcp_index >= NUM_MCP)
    return;

  if (mapping->port == 'A') {
    mcp[plug_mcp_index].pin_mode_a(mapping->pin, mode);
  } else {
    mcp[plug_mcp_index].pin_mode_b(mapping->pin, mode);
  }
}

void setup() {
  Serial.begin(115200);
  delay(3000);

  // Initialize MCP chips
  for (int i = 0; i < 9; i++) {
    mcp[i].begin();
    mcp[i].set_port_a_as_inputs(true);
    mcp[i].set_port_b_as_inputs(true);
  }

  // Test code: Set all pins to INPUT_PULLUP except pin 1 (OUTPUT, LOW)
  Serial.println("Setting up pins...");

  // SOCKET
  for (int pin = MIN_PIN; pin <= MAX_PIN; pin++) {
    if (pin == 1) {
      pinModePin(pin, OUTPUT);
      digitalWritePin(pin, LOW);
      Serial.print("Pin ");
      Serial.print(pin);
      Serial.println(" set to OUTPUT, LOW");
    } else {
      pinModePin(pin, INPUT_PULLUP);
    }
  }
  // PLUG
  for (int pin = MIN_PIN; pin <= MAX_PIN; pin++) {
    pinModePlugPin(pin, OUTPUT);
    digitalWritePlugPin(pin, LOW);
    Serial.print("Pin ");
    Serial.print(pin);
    Serial.println(" set to OUTPUT, LOW");
  }
  // USB
  for (int pin = MIN_USB; pin <= MAX_USB; pin++) {
    pinModePin(pin, OUTPUT);
    digitalWritePin(pin, LOW);
    Serial.print("Pin ");
    Serial.print(pin);
    Serial.println(" set to OUTPUT, LOW");
  }

  Serial.println("All other pins set to INPUT_PULLUP");
  Serial.println("Setup complete!");
}

void loop() {
  // Cycle through pins 1-79: set one as OUTPUT LOW, rest as INPUT_PULLUP
  static int current_output_pin = 1;
  static unsigned long last_change = 0;

  // Change every 500ms
  if (millis() - last_change > 500) {
    // Set previous pin back to INPUT_PULLUP
    pinModePin(current_output_pin, INPUT_PULLUP);
    pinModePlugPin(current_output_pin, INPUT_PULLUP);

    // Move to next pin
    current_output_pin++;
    if (current_output_pin > MAX_PIN) {
      current_output_pin = 1;
    }

    // Set new pin to OUTPUT LOW
    pinModePin(current_output_pin, OUTPUT);
    digitalWritePin(current_output_pin, LOW);

    Serial.print("Pin ");
    Serial.print(current_output_pin);
    Serial.println(" is now OUTPUT LOW");

    last_change = millis();
  }
}