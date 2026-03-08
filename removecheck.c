#include "removecheck.h"

#include "arraysize.h"
#include "legalmoveandcapture.h"

extern Piece board[8][8];

bool removeCheck(Piece piece, int availableMoveIndex, int testedMove) {
  Piece checkBoard[8][8];
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      checkBoard[i][j] = board[i][j];
    }
  }

  int colour = piece.colour;

  // here we make the move

  int rankNew = piece.availableMoves[availableMoveIndex] / 10;
  int fileNew = piece.availableMoves[availableMoveIndex] % 10;

  checkBoard[rankNew][fileNew] = board[(8 - piece.rank)][(piece.file - 97)];
  checkBoard[(8 - piece.rank)][(piece.file - 97)] = empty;

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (checkBoard[i][j].type != '_') {
        if (checkBoard[i][j].colour != colour) {
          changeAvailableMoves(&checkBoard[i][j], colour);
          int size = arraySize(checkBoard[i][j].availableMoves);
          for (int k = 0; k < size; k++) {
            if (checkBoard[i][j].availableMoves[k] == testedMove) {
              return true;
            }
          }
        }
      }
    }
  }
  return false;
}
