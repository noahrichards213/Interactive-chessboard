#include "MCP23S17.h"
#include <SPI.h>

#define CS_pin1 47
#define CS_pin2 48
#define MOSI_pin 11
#define MISO_pin 13
#define CLK_pin 12


SPIClass *fspi = NULL;
ICM42670 *IMU = NULL;

void setup()
{
  fspi = new SPIClass(FSPI);
  pinMode(CS_pin1, OUTPUT);
  pinMode(CS_pin2, OUTPUT);

  MCP23S17 MCP_1(47, &fspi);
  MCP23S17 MCP_2(48, &fspi)

  int rv = 0;

  //prints information about the MCP23S17
  Serial.begin(115200);
  Serial.println();
  Serial.println(__FILE__);
  Serial.print("MCP23S17_LIB_VERSION: ");
  Serial.println(MCP23S17_LIB_VERSION);
  Serial.println();
  delay(100);

  SPI.begin();

  //check to see if MCP object intializes properly
  rv = MCP.begin();
  Serial.println(rv ? "true" : "false");

  //check to see if we are using HWSPI (should be yes since we're not assigning pins?)
  Serial.print("HWSPI: ");
  Serial.println(MCP.usesHWSPI());

  //will print true if successful
  //0xFF means they all become INPUT (can read from it)
  //0x00 means they all become OUTPUT (can write to it)
  rv = MCP.pinMode8(0, 0x00);
  Serial.println(rv);
  rv = MCP.pinMode8(1, 0x00);
  Serial.println(rv);

  //writing laternating HIGH and LOWS to it
  Serial.println("TEST write1(0)");
  delay(100);
  for (int i = 0; i < 16; i++)
  {
    MCP.write1(0, i % 2);  //  alternating HIGH/LOW
    Serial.print(i % 2);
    Serial.print(' ');
    delay(100);
  }
  Serial.println();
  Serial.println();

  //setting to read mode
  rv = MCP.pinMode8(0, 0xFF);
  Serial.println(rv);
  rv = MCP.pinMode8(1, 0xFF);
  Serial.println(rv); 

  //testing the read function
  Serial.println("TEST read1(pin)");
  for (int pin = 0; pin < 16; pin++)
  {
    int val = MCP.read1(pin);
    Serial.print(val);
    Serial.print(' ');
    delay(100);
  }
  Serial.println();
}


void loop()
{
  delay(1000);
  Serial.println("TEST read1(pin)");
  for (int pin = 0; pin < 16; pin++)
  {
    int val = MCP.read1(pin);
    Serial.print(val);
    Serial.print(' ');
    delay(100);
  }
  Serial.println();

}


//  -- END OF FILE --