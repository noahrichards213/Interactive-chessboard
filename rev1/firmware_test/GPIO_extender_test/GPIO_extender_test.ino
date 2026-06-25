#include "MCP23S17.h"
#include <SPI.h>

#define CS_pin1 47
#define CLK_pin 12
#define MISO_pin 13
#define MOSI_pin 11

SPIClass *fspi = nullptr;
MCP23S17 *MCP_1 = nullptr;

void setup() {
  Serial.begin(115200);

  fspi = new SPIClass(FSPI);
  fspi->begin(CLK_pin, MISO_pin, MOSI_pin, CS_pin1);

  MCP_1 = new MCP23S17(CS_pin1, fspi); 

  if (MCP_1->begin() == false) {
    Serial.println("Library begin() failed.");
  } else {
      MCP_1->pinMode8(0, 0x00); 
      MCP_1->pinMode8(1, 0xFF); 
    Serial.println("Configuration complete.");

  }
}

void loop()
{
  MCP_1->write1(7, HIGH);

  delay(1300);
  MCP_1->write1(7, LOW);

  delay(1300);


  // MCP_1->write1(7, LOW);
  // delay(5000);
  // Serial.println("its low");
  //testing all the LED's
  // for (int square = 0; square < 16; square++){
  //   Serial.print("We are on square: ");
  //   Serial.print(square);
  //   write_MCP(square, HIGH, MCP_1, MCP_2);
  //   delay(100);
  //   write_MCP(square, LOW, MCP_1, MCP_2);
  // }

  //testing hall effects
  // while (1){
  //   for (int square = 0; square < 16; square++){
  //     if (read_MCP(square) == HIGH){
  //       write_MCP(square, HIGH);
  //     } else if (read_MCP(square) == LOW){
  //       write_MCP(square, LOW);
  //     }

  //   }
  // }


}

