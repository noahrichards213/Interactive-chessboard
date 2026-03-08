#include "legalinput.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "arraysize.h"
#include "constantsandstructs.h"

extern Piece board[8][8];

Piece legalSource(char file, int rank, int colour) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      // if rank, file, and colour match
      if (board[i][j].rank == rank && board[i][j].file == file &&
          board[i][j].colour == colour) {
        return board[i][j];
      }
    }
  }

  return empty;
}

int legalDest(char fileDest, int rankDest, char fileSource, int rankSource,
              int colour) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      // if rank, file, and colour match
      if (board[i][j].rank == rankSource && board[i][j].file == fileSource &&
          board[i][j].colour == colour) {
        int size = arraySize(board[i][j].availableMoves);

        // matching character input to actual data in int *availableMoves

        int matchingRank = 8 - rankDest;
        int matchingFile = fileDest - 97;

        for (int k = 0; k < size; k++) {
          if (board[i][j].availableMoves[k] ==
                  ((matchingRank * 10) + matchingFile) ||
              // to include castilng moves
              board[i][j].availableMoves[k] ==
                  ((matchingRank * 10) + matchingFile) * (100)) {
            return board[i][j].availableMoves[k];
          }
        }
      }
    }
  }

  return -1;
}
