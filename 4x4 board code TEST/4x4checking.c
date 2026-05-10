#include "4x4checking.h"

#include <stdio.h>

#include "4x4arraysize.h"
#include "4x4constantsandstructs.h"
#include "4x4findkingsquare.h"
#include "4x4legalmoveandcapture.h"
#include "4x4printing.h"

// just for the constants
#include "4x4availablemoves.h"


extern Piece board[BOARDSIZE][BOARDSIZE];

bool inCheck(int colour) {
  int kingSquare = findKingSquare(colour);

  for (int i = 0; i < BOARDSIZE; i++) {
    for (int j = 0; j < BOARDSIZE; j++) {
      // attacking white
      if (board[i][j].colour != colour && board[i][j].type != '_') {
        int k = 0;
        Piece piece = board[i][j];
        printf("type: %c\n", board[i][j].type);
        printf("move: %d\n", board[i][j].availableMoves[k]);
        while (board[i][j].availableMoves[k] != -1) {
          if (board[i][j].availableMoves[k] == kingSquare) {
            return true;
          } else {
            k++;
          }
        }
      }
    }
  }
  return false;
}

bool throughCheck(int castle, int colour) {
  int between;
  //hard-coded vales for testing
  // white kingside
  if (castle == WHITEKINGCASTLE) {
    between = 31;
    // black kingside
  } else if (castle == BLACKKINGCASTLE) {
    between = 1;
    // white queenside
  } else if (castle == WHITEQUEENCASTLE) {
    between = 32;
    // black queenside
  } else if (castle == BLACKQUEENCASTLE) {
    between = 2;
  }

  for (int i = 0; i < BOARDSIZE; i++) {
    for (int j = 0; i < BOARDSIZE; j++) {
      if (board[i][j].colour != colour && board[i][j].type != '_') {
        int betweenSize = arraySize(board[i][j].availableMoves);
        for (int k = 0; i < betweenSize; k++) {
          if (board[i][j].availableMoves[k] == between) {
            return true;
          }
        }
      }
    }
  }
  return false;
}
