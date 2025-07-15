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

const int MIN_PIN = 1;
const int MAX_PIN = 60;
const int MIN_USB = 61;
const int MAX_USB = 79;

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
    {1, 'B', 0, true},   // Pin 1
    {1, 'B', 2, true},   // Pin 2
    {1, 'B', 4, true},   // Pin 3
    {1, 'B', 6, true},   // Pin 4
    {1, 'A', 0, true},   // Pin 5
    {2, 'B', 3, true},   // Pin 6
    {2, 'B', 1, true},   // Pin 7
    {1, 'A', 7, true},   // Pin 8
    {1, 'A', 5, true},   // Pin 9
    {1, 'A', 3, true},   // Pin 10
    {1, 'B', 1, true},   // Pin 11
    {1, 'B', 3, true},   // Pin 12
    {1, 'B', 5, true},   // Pin 13
    {1, 'B', 7, true},   // Pin 14
    {1, 'A', 1, true},   // Pin 15
    {2, 'B', 2, true},   // Pin 16
    {2, 'B', 0, true},   // Pin 17
    {1, 'A', 6, true},   // Pin 18
    {1, 'A', 4, true},   // Pin 19
    {1, 'A', 2, true},   // Pin 20
    {2, 'B', 4, true},   // Pin 21
    {2, 'B', 6, true},   // Pin 22
    {2, 'A', 0, true},   // Pin 23
    {2, 'A', 2, true},   // Pin 24
    {2, 'A', 4, true},   // Pin 25
    {3, 'B', 7, true},   // Pin 26
    {3, 'B', 5, true},   // Pin 27
    {3, 'B', 3, true},   // Pin 28
    {3, 'B', 1, true},   // Pin 29
    {2, 'A', 7, true},   // Pin 30
    {2, 'B', 5, true},   // Pin 31
    {2, 'B', 7, true},   // Pin 32
    {2, 'A', 1, true},   // Pin 33
    {2, 'A', 3, true},   // Pin 34
    {2, 'A', 5, true},   // Pin 35
    {3, 'B', 6, true},   // Pin 36
    {3, 'B', 4, true},   // Pin 37
    {3, 'B', 2, true},   // Pin 38
    {3, 'B', 0, true},   // Pin 39
    {2, 'A', 6, true},   // Pin 40
    {3, 'A', 0, true},   // Pin 41
    {3, 'A', 2, true},   // Pin 42
    {3, 'A', 4, true},   // Pin 43
    {3, 'A', 6, true},   // Pin 44
    {4, 'B', 0, true},   // Pin 45
    {4, 'A', 3, true},   // Pin 46
    {4, 'A', 1, true},   // Pin 47
    {4, 'B', 7, true},   // Pin 48
    {4, 'B', 5, true},   // Pin 49
    {4, 'B', 3, true},   // Pin 50
    {3, 'A', 1, true},   // Pin 51
    {3, 'A', 3, true},   // Pin 52
    {3, 'A', 5, true},   // Pin 53
    {3, 'A', 7, true},   // Pin 54
    {4, 'B', 1, true},   // Pin 55
    {4, 'A', 2, true},   // Pin 56
    {4, 'A', 0, true},   // Pin 57
    {4, 'B', 6, true},   // Pin 58
    {4, 'B', 4, true},   // Pin 59
    {4, 'B', 2, true},   // Pin 60
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
  for (int i = 5; i < 9; i++) {
    mcp[i].begin();
    mcp[i].set_port_a_as_inputs(true);
    mcp[i].set_port_b_as_inputs(true);
  }

  for (int i = 0; i < 4; i++) {
    mcp[i].begin();
    mcp[i].set_port_a_as_outputs();
    mcp[i].set_port_b_as_outputs();
    mcp[i].write_port_a(0x00);
    mcp[i].write_port_b(0x00);
  }

  Serial.println("Setup complete!");
}

void loop() {
  static int current_output_pin = 1;
  static unsigned long last_change = 0;

  if (millis() - last_change > 500) {
    for (int i = MIN_PIN; i <= MAX_PIN; i++) {
      Serial.print(digitalReadPin(i));
      if (i < MAX_PIN)
        Serial.print(",");
    }

    Serial.println("]");

    last_change = millis();
  }
}
