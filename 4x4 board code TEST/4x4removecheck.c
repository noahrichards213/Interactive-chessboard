#include "4x4removecheck.h"

#include <stdbool.h>
#include <stdlib.h>

#include "4x4arraysize.h"
#include "4x4findkingsquare.h"
#include "4x4legalmoveandcapture.h"
#include "4x4printing.h"

extern Piece board[BOARDSIZE][BOARDSIZE];

// finding the kingSquare

bool removeCheck(Piece piece, int availableMoveIndex, int testedMove) {
  printf("We ar emaking it to start\n");
  int kingSquare;

  Piece prevBoard[BOARDSIZE][BOARDSIZE];

  // need to create deep copy
  for (int i = 0; i < BOARDSIZE; i++) {
    for (int j = 0; j < BOARDSIZE; j++) {
      if (board[i][j].type != '_') {
        int* deepMoves = malloc(sizeof(int) * 30);
        if (deepMoves == NULL) {
          printf("failed to allocate");
        }
        for (int k = 0; k <= arraySize(board[i][j].availableMoves); k++) {
          deepMoves[k] = board[i][j].availableMoves[k];
        }
        Piece deepCopy = {board[i][j].hasMoved, board[i][j].colour,
                          board[i][j].rank,     board[i][j].file,
                          board[i][j].type,     deepMoves};
        prevBoard[i][j] = deepCopy;
      } else {
        prevBoard[i][j] = board[i][j];
      }
    }
  }

  int pieceColour = piece.colour;
  int oppositeColour = piece.colour == WHITE ? BLACK : WHITE;

  int move = piece.availableMoves[availableMoveIndex];
  int rankNew;
  int fileNew;

  // DENOTES CASTLING, WHEN YOU ADD EN PASSANT, CHANGE THIS
  if (move >= 100 && move < 10000) {
    move /= 100;
  }

  // still applies for 4x4 board, pawns will start on first rank
  if (move > 10000) {
    move /= 1000;
  }
  // here we make the move
  rankNew = move / 10;
  fileNew = move % 10;

  prevBoard[rankNew][fileNew] =
      prevBoard[(BOARDSIZE - piece.rank)][(piece.file - 97)];
  prevBoard[(BOARDSIZE - piece.rank)][(piece.file - 97)] = empty;

  kingSquare = findKingSquare(pieceColour);

  for (int i = 0; i < BOARDSIZE; i++) {
    for (int j = 0; j < BOARDSIZE; j++) {
      if (prevBoard[i][j].type != '_') {
        if (prevBoard[i][j].colour == oppositeColour) {
          printf("We are making it right before\n");
          changeAvailableMoves(&prevBoard[i][j]);
          printf("We are making it right after\n");

          for (int k = 0; k < arraySize(prevBoard[i][j].availableMoves); k++) {
            if (prevBoard[i][j].availableMoves[k] == kingSquare) {
              printf("We are making it here true\n");

              return true;
            } else {
              printf("the move was: %d\n", prevBoard[i][j].availableMoves[k]);
              printf("the piece was: %c\n", prevBoard[i][j].type); 
              printf("king square was: %d\n", kingSquare);
            }
          }
        }
      }
    }
  }

  printf("We are making it here false\n");

  return false;
}
