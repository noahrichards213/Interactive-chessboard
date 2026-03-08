#include "enpassant.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "arraysize.h"

extern Piece board[8][8];

void removeEnPassant() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      int size = arraySize(board[i][j].availableMoves);
      for (int k = 0; k < size; k++) {
        // has en passant move
        if (board[i][j].availableMoves[k] >= 10000) {
          board[i][j].availableMoves[k] = EN_PASSANT;
        }
      }
    }
  }
}

// colour is the pawn that moved two squares
// rank and file is the square that it moved to (use actual inputted values)
void addEnPassant(Piece piece, int rank, char file) {
  int colour = piece.colour;

  // old rank - new rank
  int difference = abs(piece.rank - rank);

  // if pawn did not move two squares then we don't need this function
  if (difference != 2) {
    printf("We did return\n");
    return;
  }

  printf("We didn't return\n");

  // white is 1, black is 0
  char enemyPawn = colour ? 'p' : 'P';
  int normDir = colour ? WHITE : BLACK;
  // we know that if the pawn successfully moved two
  // squares nothing can be blocking it

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      // left side en passant

      if (board[i][j].type == enemyPawn && board[i][j].rank == rank &&
          board[i][j].file == (file - 1)) {
        int size = arraySize(board[i][j + 1].availableMoves);
        board[i][j + 1].availableMoves[size] =
            ((i + normDir) * 10 + (file - 1) * 10000);
        board[i][j + 1].availableMoves[size + 1] = -1;
      }

      // right side en passant

      if (board[i][j].type == enemyPawn && board[i][j].rank == rank &&
          board[i][j].file == (file + 1)) {
        int size = arraySize(board[i][j - 1].availableMoves);
        board[i][j - 1].availableMoves[size] =
            ((i + normDir) * 10 + (file + 1) * 10000);
        board[i][j - 1].availableMoves[size + 1] = -1;
      }
    }
  }
}