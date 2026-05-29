#include "MCP23S17.h"
#include <SPI.h>

#define CS_pin1 47
#define CS_pin2 48
#define MOSI_pin 11
#define MISO_pin 13
#define CLK_pin 12

SPIClass *fspi = NULL;
MCP23S17 *MCP_1 = NULL;
MCP23S17 *MCP_2 = NULL;

int read_MCP(uint8_t index);
void write_MCP(uint8_t index, uint8_t value);

void setup()
{
  fspi = new SPIClass(FSPI);
  pinMode(CS_pin1, OUTPUT);
  pinMode(CS_pin2, OUTPUT);

  MCP23S17 MCP_1(47, fspi);
  MCP23S17 MCP_2(48, fspi);

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
  rv = MCP_1.begin();
  Serial.println(rv ? "true_1" : "false_1");

  rv = MCP_2.begin();
  Serial.println(rv ? "true_2" : "false_2");


  //check to see if we are using HWSPI (should be yes since we're not assigning pins?)
  Serial.print("HWSPI_1: ");
  Serial.println(MCP_1.usesHWSPI());

  Serial.print("HWSPI_2: ");
  Serial.println(MCP_2.usesHWSPI());

  //0xFF means 11111111 they all become INPUT (can read from it)
  //0x00 means 00000000 they all become OUTPUT (can write to it)
  
  //we want hall effects to always be read mode and leds to be read mode
  //0 is port A = write and 1 is port B = read

  rv = MCP_1.pinMode8(0, 0x00);
  Serial.println(rv);
  rv = MCP_1.pinMode8(1, 0xFF);
  Serial.println(rv);

  rv = MCP_2.pinMode8(0, 0x00);
  Serial.println(rv);
  rv = MCP_2.pinMode8(1, 0xFF);
  Serial.println(rv);
}


void loop()
{
  //testing all the LED's
  for (int square = 0; square < 16; square++){
    write_MCP(square, HIGH);
    delay(100);
    write_MCP(square, LOW);
  }

  //testing hall effects
  while (1){
    for (int square = 0; square < 16; square++){
      if (read_MCP(square) == HIGH){
        write_MCP(square, HIGH);
      } else if (read_MCP(square) == LOW){
        write_MCP(square, LOW);
      }

    }
  }


}

//index from 0 to 7 is portB
//index from 8 - 15 is portA

//based off where the hall effects are
int read_MCP(uint8_t index){
if (index < 8){
 return MCP_1->read1(index);
} else if (index > 8){
 return MCP_2->read1(index - 8);
}
}

//based off where the leds are
void write_MCP(uint8_t index, uint8_t value){
if (index < 8){
  MCP_1->write1(index + 8, value);
} else if (index > 8){
  MCP_2->write1(index, value);
}
}

//  -- END OF FILE --