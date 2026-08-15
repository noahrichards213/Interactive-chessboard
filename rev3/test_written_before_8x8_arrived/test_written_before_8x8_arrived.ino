//LED driver
#define GATE_DELAY 20

#define SER_IN 4
#define SRCK   48
#define RCK    47

//for the hall effects
#define ON 0
#define OFF 1
#define THRESHOLD 250

#define BOARDSIZE 4


//Multiplexer
#include <CD74HC4067.h>


//VScode main function

// Recall that all the #'s are referring to GPIO
#define S0 7
#define S1 8
#define S2 9
#define S3 10

#define A 0
#define B 1

//for muxes
const int a_common_pin = 5;
const int b_common_pin = 6;
const int c_common_pin = 11;
const int d_common_pin = 12;


// Initialize the multiplexer with control pins S0, S1, S2, S3
CD74HC4067 mux_A(S0, S1, S2, S3);
CD74HC4067 mux_B(S0, S1, S2, S3);
CD74HC4067 mux_C(S0, S1, S2, S3);
CD74HC4067 mux_D(S0, S1, S2, S3);


void b595_write(unsigned int data)
{
    // 1. Shift out the high byte first (this goes past IC1 and ends up in IC2)

    for (int reg_num; reg_num >= 0; reg_num--){
      shiftOut(SER_IN, SRCK, MSBFIRST, ((data >> (8 * reg_num)) & 0xFF));
    }

    
    // 3. Pulse the latch (RCK) AFTER all 16 bits are sent
    // This makes both ICs update their outputs at the exact same time
    delayMicroseconds(GATE_DELAY);
    digitalWrite(RCK, HIGH);
    delayMicroseconds(GATE_DELAY);
    digitalWrite(RCK, LOW);
}



//let's start with just one piece moving around
int boardStates[BOARDSIZE][BOARDSIZE];

//helper functions to map boardState array to loopThroughSensors()
int findBoardRow(int mux_number, int loop_number){
  if (mux_number == A){
    if (loop_number >= 12 && loop_number <= 16){
      return 0;
    } else if (loop_number >= 0 && loop_number <= 3){
      return 1;
    }
  } else if (mux_number == B){
    if (loop_number >= 12 && loop_number <= 16){
      return 3;
    } else if (loop_number >= 0 && loop_number <= 3){
      return 2;
    }
  }
}

int findBoardCol(int mux_number, int loop_number){  
  if (mux_number == A){
    if (loop_number == 12 || loop_number == 3){
      return 0;
    } else if (loop_number == 13 || loop_number == 2){
      return 1;
    } else if (loop_number == 14 || loop_number == 1){
      return 2;
    } else if (loop_number == 15 || loop_number == 0){
      return 3;
    }
  } else if (mux_number == B){
    if (loop_number == 12 || loop_number == 3){
      return 3;
    } else if (loop_number == 13 || loop_number == 2){
      return 2;
    } else if (loop_number == 14 || loop_number == 1){
      return 1;
    } else if (loop_number == 15 || loop_number == 0){
      return 0;
    }
  }
}

void changedSquare(int mux_number, int loop_number, int state){

  // for (int i = 0; i < BOARDSIZE; i++){
  //   for (int j = 0; j < BOARDSIZE; j++){
  //     Serial.print(boardStates[i][j]);
  //   }
  //   Serial.println(" ");
  // }

  delay(50);

  
  if (state == OFF){
    Serial.print("AWAY ");
    Serial.print(mux_number);
    Serial.println(loop_number);
  } else if (state == ON){
    Serial.print("ON ");
    Serial.print(mux_number);
    Serial.println(loop_number);
  }

}

// uint64_t calculateallLED(int row, int col){
//   uint64_t sum = 0ull;
//   for (int i = 0; i < arraySize(board[row][col].availableMoves)){
//     sum += calculateoneLED(board[row][col].availableMoves[i]);
//   }

//   return sum;
// }

int findDrainNum(int row, int col){
 //for columns 2 and 0
  if (col % 2 == 0){
    return(row + 2);

  //for columns 3 and 1
  } else if (col % 2 == 1){
    if (row == 0){
      return 1;  
    } else if (row == 1){
      return 0;
    } else if (row == 2){
      return 7;
    } else if (row == 3){
      return 6;
    }
  }  
}

int findRegNum (int row, int col) {
    //LEFT is 1, RIGHT is 0
  if (col == 0 || col == 1){
    return 1;
  } else if (col == 2 || col == 3){
    return 0;
  }
}

int calculateoneLED(int availableMove){
  int row = availableMove / 10;
  int col = availableMove % 10;
  int drain_num = -1;
  int reg_num = -1;

  //based on row and column from availableMove, how do we access that square based on register?
  int numRegisters = (BOARDSIZE * BOARDSIZE) / 8;

  // THIS IS TO FIND THE DRAIN # ON THE REG
  drain_num = findDrainNum(row, col);

  //THIS IS TO FIND THE REG # (so far only for 4x4)
  reg_num = findRegNum(row, col);
  
  int addend = 1;
  //in these tpic registers w/ shiftout MSBFIRST, Drain0 fills first for original AND daisy-chained registers
  addend >> (reg_num * 8) + drain_num;

  return addend;
  

}

int checkMux(int mux_number){

}


void setup()
{
  //boardStates setup
  for (int i = 0; i < BOARDSIZE; i++){
    for (int j = 0; j < BOARDSIZE; j++){
      boardStates[i][j] = OFF;
    }
  }
  // boardStates[0][3] = ON;
  
  Serial.begin(115200);    
  pinMode(SER_IN, OUTPUT);
  pinMode(SRCK, OUTPUT);
  pinMode(RCK, OUTPUT);

  Serial.println("LED driver initialized");

  // Set control pins as outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  // Set the common signal pin as an input once
  pinMode(a_common_pin, INPUT);
  pinMode(b_common_pin, INPUT);

  Serial.println("Multiplexer code initialized");

}

void loop()
{    
  for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++){
    int muxA_state;
    int muxB_state;

    //this if statement if temporary because we aren't using all mux spots right now
    if (i <= 3 || i >= 12){

      if (i == 0 || i == 1|| i == 14 || i == 15){
        mux_A.channel(i);

        analogRead(a_common_pin);
        
        muxA_state = analogRead(a_common_pin);
        if (muxA_state > THRESHOLD){
          muxA_state = OFF;
        } else if (muxA_state <= THRESHOLD){
          muxA_state = ON;
        }

        if (muxA_state != boardStates[findBoardRow(A, i)][findBoardCol(A, i)]){
          //somewhere here, copy the board so we return it to normal later

          changedSquare(A, i, muxA_state);
          boardStates[findBoardRow(A, i)][findBoardCol(A, i)] = muxA_state;
        }

      }
      
      if (i == 2 || i == 3 || i == 12 || i == 13){
      mux_B.channel(i);

    
      analogRead(b_common_pin);

      muxB_state = analogRead(b_common_pin);
      if (muxB_state > THRESHOLD){
        muxB_state = OFF;
      } else if (muxB_state <= THRESHOLD){
        muxB_state = ON;
      }

      if (muxB_state != boardStates[findBoardRow(B, i)][findBoardCol(B, i)]){
        changedSquare(B, i, muxB_state);
        boardStates[findBoardRow(B, i)][findBoardCol(B, i)] = muxB_state;
      } 
    }
    }
  }
}