#include "4x4findkingsquare.h"

#include "4x4constantsandstructs.h"

extern Piece board[BOARDSIZE][BOARDSIZE];

int findKingSquare(int colour) {
  // finding kingSquare for white and black
  int kingSquare = -1;
  char kingCharacter = colour == WHITE ? 'K' : 'k';
  for (int i = 0; i < BOARDSIZE; i++) {
    for (int j = 0; j < BOARDSIZE && kingSquare == -1; j++) {
      if (board[i][j].type == kingCharacter) {
        kingSquare = (i * 10) + j;
      }
    }
  }
  return kingSquare;
}
