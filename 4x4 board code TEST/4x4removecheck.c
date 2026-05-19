#include "4x4removecheck.h"

#include <stdbool.h>
#include <stdlib.h>

#include "4x4arraysize.h"
#include "4x4findkingsquare.h"
#include "4x4legalmoveandcapture.h"
#include "4x4printing.h"

extern Piece board[BOARDSIZE][BOARDSIZE];

bool removeCheck(Piece piece, int availableMoveIndex, int testedMove) {

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

  int kingSquare = -1;
  char kingCharacter = pieceColour == WHITE ? 'K' : 'k';

  for (int i = 0; i < BOARDSIZE; i++) {
    for (int j = 0; j < BOARDSIZE && kingSquare == -1; j++) {
      if (prevBoard[i][j].type == kingCharacter) {
        kingSquare = (i * 10) + j;
      }
    }
  }


  for (int i = 0; i < BOARDSIZE; i++) {
    for (int j = 0; j < BOARDSIZE; j++) {
      if (prevBoard[i][j].type != '_') {
        if (prevBoard[i][j].colour == oppositeColour) {
          changeAvailableMoves(&prevBoard[i][j]);

          for (int k = 0; k < arraySize(prevBoard[i][j].availableMoves); k++) {
            if (prevBoard[i][j].availableMoves[k] == kingSquare) {
              for (int m = 0; m < BOARDSIZE; m++) {
                for (int l = 0; l < BOARDSIZE; l++) {
                  if (prevBoard[m][l].type != '_') {
                    free(prevBoard[m][l].availableMoves);
                    prevBoard[m][l].availableMoves = NULL;
                  }
                }
              }
              return true;
            }
          }
        }
      }
    }
  }

  for (int o = 0; o < BOARDSIZE; o++) {
    for (int p = 0; p < BOARDSIZE; p++) {
      if (prevBoard[o][p].type != '_') {
        free(prevBoard[o][p].availableMoves);
        prevBoard[o][p].availableMoves = NULL;
      }
    }
  }
  return false;
}
