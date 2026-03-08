#include "findkingsquare.h"

#include "constantsandstructs.h"

extern Piece board[8][8];

int findKingSquare(int colour) {
  // finding kingSquare for white and black
  int kingSquare = -1;
  char kingCharacter = colour ? 'K' : 'k';
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8 && kingSquare == -1; j++) {
      if (board[i][j].type == kingCharacter) {
        kingSquare = (i * 10) + j;
      }
    }
  }
  return kingSquare;
}
