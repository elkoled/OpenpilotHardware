#include <Arduino.h>
#include "MCP23017.h"

#define SDA1 8
#define SCL1 9
#define SDA2 0
#define SCL2 1

MCP23017 mcp[] = {
    MCP23017(0x20, SDA1, SCL1),
    MCP23017(0x21, SDA1, SCL1),
    MCP23017(0x22, SDA1, SCL1),
    MCP23017(0x23, SDA1, SCL1),
    MCP23017(0x24, SDA1, SCL1),
    MCP23017(0x25, SDA1, SCL1),
    MCP23017(0x26, SDA1, SCL1),
    MCP23017(0x27, SDA1, SCL1),
    MCP23017(0x20, SDA2, SCL2),
};

const int NUM_MCP = sizeof(mcp) / sizeof(MCP23017);

// pin mapping
const uint8_t mapping[80] = {0, 1, 3, 5, 7, 9, 20, 18, 16, 14, 12, 2, 4, 6, 8, 10, 19, 17, 15, 13, 11, 21, 23, 25, 27, 29, 40, 38, 36, 34, 32, 22, 24, 26, 28, 30, 39, 37, 35, 33, 31, 41, 43, 45, 47, 49, 60, 58, 56, 54, 52, 42, 44, 46, 48, 50, 59, 57, 55, 53, 51, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79};

void setup()
{
  Serial.begin(115200);
  delay(3000);

  for (int i = 4; i < 9; i++)
  {
    mcp[i].begin();
    mcp[i].set_port_a_as_inputs(true);
    mcp[i].set_port_b_as_inputs(true);
  }
  for (int i = 0; i < 4; i++)
  {
    mcp[i].begin();
    mcp[i].set_port_a_as_outputs();
    mcp[i].set_port_b_as_outputs();
    mcp[i].write_port_a(0x00);
    mcp[i].write_port_b(0x00);
  }
}

void loop()
{
  uint8_t port_a, port_b;
  int index = 0;

  for (int i = 5; i < 9; ++i)
  {
    port_a = mcp[i].read_port_a();
    port_b = mcp[i].read_port_b();

    for (int j = 0; j < 8; j++)
    {
      if (!(port_b & (1 << j)))
        Serial.println(mapping[index]);
      index++;
    }

    for (int j = 0; j < 8; j++)
    {
      if (!(port_a & (1 << j)))
        Serial.println(mapping[index]);
      index++;
    }
  }
  delay(500);
}

