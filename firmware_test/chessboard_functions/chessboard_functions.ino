#include <CD74HC4067.h>

//for the hall effects
#define ON 0
#define OFF 1
#define THRESHOLD 500

//recall that all the #'s are are referring to GPIO
#define S0 7
#define S1 8
#define S2 9
#define S3 10

#define A 0
#define B 1

//creating mux object
CD74HC4067 mux_A(S0, S1, S2, S3);
CD74HC4067 mux_B(S0, S1, S2, S3);

const int a_common_pin = 5;
const int b_common_pin = 6;

//let's start with just one piece moving around
int boardStates[4][4];

// void muxSetup(){


// }

// void LEDDriverSetup(){



// }

//helper function to map boardState array to loopThroughSensors()
int findBoardRow(int mux_number, int loop_number){
  if (mux_number == A){
    if (i >= 12 && i <= 16){
      return 0;
    } else if (i >= 0 && i <= 3){
      return 1;
    }
  } else if (mux_number == B){
    if (i >= 12 && i <= 16){
      return 3;
    } else if (i >= 0 && i <= 3){
      return 2;
    }
  }
}

int findBoardCol(int mux_number, int loop_number){  
  if (mux_number == A){
    if (i == 12 || i == 3){
      return 0;
    } else if (i == 13 || i == 2){
      return 1;
    } else if (i == 14 || i == 1){
      return 2;
    } else if (i == 15 || i == 0){
      return 3;
    }
  } else if (mux_number == B){
    if (i == 12 || i == 3){
      return 3;
    } else if (i == 13 || i == 2){
      return 2;
    } else if (i == 14 || i == 1){
      return 1;
    } else if (i == 15 || i == 0){
      return 0;
    }
  }
}

void changedSquare(int state){
  if (state > THRESHOLD){
    Serial.println("You took a piece away");
  } else if (state <= THRESHOLD){
    Serial.println("You put a piece on");
  }
}


void setup()
{


}

void loop() {
    for (int i = 0; i < 16; i++){
    int muxA_state;
    int muxB_state;

    //this if statement if temporary because we aren't using all mux spots
    if (i <= 3 && i >= 12){
      mux_A.channel(i);
      mux_B.channel(i);

      muxA_state = analogRead(a_common_pin);
      if (muxA_state != boardStates[findBoardRow(A, i)][findBoardCol(A, i)]){
        void changedSqare(muxA_state);
        boardStates[findBoardRow(A, i)][findBoardCol(A, i)] = muxA_state;
      }

      muxB_state = analogRead(b_common_pin);
      if (muxB_state != boardStates[findBoardRow(B, i), findBoardCol(B, i)]){
        void changedSquare(muxB_state);
        boardStates[findBoardRow(B, i)][findBoardCol(B, i)] = muxB_state;
      }
    }

    


}

}



