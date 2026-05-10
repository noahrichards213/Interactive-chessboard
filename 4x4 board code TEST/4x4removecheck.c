#include "4x4removecheck.h"

#include "4x4arraysize.h"
#include "4x4findkingsquare.h"
#include "4x4legalmoveandcapture.h"
#include "4x4printing.h"

extern Piece board[BOARDSIZE][BOARDSIZE];

// finding the kingSquare

bool removeCheck(Piece piece, int availableMoveIndex, int testedMove) {
  int kingSquare;

  Piece prevBoard[BOARDSIZE][BOARDSIZE];
  for (int i = 0; i < BOARDSIZE; i++) {
    for (int j = 0; j < BOARDSIZE; j++) {
      prevBoard[i][j] = board[i][j];
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

  //still applies for 4x4 board, pawns will start on first rank
  if (move > 10000) {
    move /= 1000;
  }
  // here we make the move
  rankNew = move / 10;
  fileNew = move % 10;

  board[rankNew][fileNew] = board[(BOARDSIZE - piece.rank)][(piece.file - 97)];
  board[(BOARDSIZE - piece.rank)][(piece.file - 97)] = empty;

  
  kingSquare = findKingSquare(piece.colour);

  for (int i = 0; i < BOARDSIZE; i++) {
    for (int j = 0; j < BOARDSIZE; j++) {
      if (board[i][j].type != '_') {
        if (board[i][j].colour == oppositeColour) {
          // pieceColour is the turn that it is not, because we are moving the
          // other colour for hypothetical
          changeAvailableMoves(&board[i][j], pieceColour);
          int size = arraySize(board[i][j].availableMoves);

          for (int k = 0; k < size; k++) {
            if (board[i][j].availableMoves[k] == kingSquare) {
              for (int l = 0; l < BOARDSIZE; l++) {
                for (int m = 0; m < BOARDSIZE; m++) {
                  board[l][m] = prevBoard[l][m];
                }
              }

              return true;
            } 
          }
        }
      }
    }
  }

  for (int l = 0; l < BOARDSIZE; l++) {
    for (int m = 0; m < BOARDSIZE; m++) {
      board[l][m] = prevBoard[l][m];
    }
  }
  return false;
}
