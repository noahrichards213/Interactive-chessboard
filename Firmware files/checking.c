#include "checking.h"

#include <stdio.h>

#include "arraysize.h"
#include "constantsandstructs.h"
#include "findkingsquare.h"
#include "legalmoveandcapture.h"

// just for the constants
#include "availablemoves.h"

extern Piece board[8][8];

bool inCheck(int colour) {
  int kingSquare = findKingSquare(colour);

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      // attacking white
      if (board[i][j].colour != colour && board[i][j].type != '_') {
        int k = 0;
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
  // white kingside
  if (castle == WHITEKINGCASTLE) {
    between = 75;
    // black kingside
  } else if (castle == BLACKKINGCASTLE) {
    between = 5;
    // white queenside
  } else if (castle == WHITEQUEENCASTLE) {
    between = 73;
    // black queenside
  } else if (castle == BLACKQUEENCASTLE) {
    between = 3;
  }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; i < 8; j++) {
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
