#include "4x4legalmoveandcapture.h"

#include <stdio.h>
#include <stdlib.h>

#include "4x4arraysize.h"
#include "4x4availablemoves.h"
#include "4x4printing.h"

extern Piece board[BOARDSIZE][BOARDSIZE];

// int colour is the colour whose turn it is not
void changeAvailableMoves(Piece* piece) {
  char type = piece->type;

  // we don't want to remove en passant move
  if (type != 'P' && type != 'p') {
    free(piece->availableMoves);
    piece->availableMoves = NULL;
  }

  if (type == 'P' || type == 'p') {
    piece->availableMoves = pawnMoves(piece);
  } else if (type == 'R' || type == 'r') {
    piece->availableMoves = rookMoves(piece);
  } else if (type == 'N' || type == 'n') {
    piece->availableMoves = knightMoves(piece);
  } else if (type == 'B' || type == 'b') {
    piece->availableMoves = bishopMoves(piece);
  } else if (type == 'Q' || type == 'q') {
    piece->availableMoves = queenMoves(piece);
  } else if (type == 'K' || type == 'k') {
    piece->availableMoves = kingMoves(piece);
  } else {
    printf("WE SHOULD NOT BE HERE\n");
  }

  return;
}

// checks to see if there is a legal move
bool checkLegalMoves(int turn) {
  for (int i = 0; i < BOARDSIZE; i++) {
    for (int j = 0; j < BOARDSIZE; j++) {
      if (board[i][j].colour == turn) {
        int size = arraySize(board[i][j].availableMoves);
        for (int k = 0; k < size; k++) {
          if (board[i][j].availableMoves[k] > 0) {
            return true;
          }
        }
      }
    }
  }
  return false;
}
