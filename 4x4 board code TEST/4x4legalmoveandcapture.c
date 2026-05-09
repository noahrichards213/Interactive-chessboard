#include "4x4legalmoveandcapture.h"

#include <stdio.h>
#include <stdlib.h>

#include "4x4arraysize.h"
#include "4x4availablemoves.h"

extern Piece board[8][8];

// int colour is the colour whose turn it is not
void changeAvailableMoves(Piece* piece, int colour) {
  char type = piece->type;


  if (type == 'P' || type == 'p') {
    piece->availableMoves = pawnMoves(*piece);
  } else if (type == 'R' || type == 'r') {
    piece->availableMoves = rookMoves(*piece);
  } else if (type == 'N' || type == 'n') {
    piece->availableMoves = knightMoves(*piece);
  } else if (type == 'B' || type == 'b') {
    piece->availableMoves = bishopMoves(*piece);
  } else if (type == 'Q' || type == 'q') {
    piece->availableMoves = queenMoves(*piece);
  } else if (type == 'K' || type == 'k') {
    piece->availableMoves = kingMoves(*piece);
  }
}

// checks to see if there is a legal move
bool checkLegalMoves(int turn) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (board[i][j].colour == turn) {
        int size = arraySize(board[i][j].availableMoves);
        for (int k = 0; k < size; k++) {
          if (board[i][j].availableMoves[k] > 0) {
            printf("This was the was piece: %c\n", board[i][j].type);
            printf("This was the move: %d\n", board[i][j].availableMoves[k]);
            return true;
          }
        }
      }
    }
  }
  return false;
}
