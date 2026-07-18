  #include "MCP23S17.h"
  #include <SPI.h>

  #define CS_pin1 47
  #define CS_pin2 48
  #define CLK_pin 12
  #define MISO_pin 13
  #define MOSI_pin 11

  SPIClass *fspi_1 = nullptr;
  MCP23S17 *MCP_1 = nullptr;
  MCP23S17 *MCP_2 = nullptr;

  void setup() {  
    Serial.begin(115200);

    fspi_1 = new SPIClass(FSPI);
    fspi_1->begin(CLK_pin, MISO_pin, MOSI_pin);

    MCP_1 = new MCP23S17(CS_pin1, fspi_1); 
    MCP_2 = new MCP23S17(CS_pin2, fspi_1);

    if (MCP_1->begin() == false) {
      Serial.println("Library begin1() failed.");
    } else {
        MCP_1->pinMode8(0, 0x00); 
        MCP_1->pinMode8(1, 0xFF); 
      Serial.println("Configuration 1 complete.");

    }

    if (MCP_2->begin() == false) {
      Serial.println("Library begin2() failed.");
    } else {
        MCP_2->pinMode8(0, 0x00); 
        MCP_2->pinMode8(1, 0xFF); 
      Serial.println("Configuration 2 complete.");

    }
    MCP_2->write1(6, HIGH);
  }

  void loop()
  {

    // Serial.println("HIGH");

    // Serial.println(MCP_1->write1(7, HIGH));
    // Serial.println(MCP_1->write1(6, HIGH));

    // Serial.println(MCP_2->write1(6, HIGH));
    // Serial.println(MCP_2->write1(5, HIGH));

    // delay(1000);

    
    // Serial.println("LOW");
    // MCP_1->write1(7, LOW);
    // MCP_1->write1(6, LOW);

    // MCP_2->write1(6, LOW);
    // MCP_2->write1(5, LOW);

    // delay(1000);
    // MCP_1->write1(6, LOW);
    // MCP_1->write1(5, LOW);

    // delay(1300);


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

