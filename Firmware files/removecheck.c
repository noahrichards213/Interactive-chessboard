#include "removecheck.h"

#include "arraysize.h"
#include "findkingsquare.h"
#include "legalmoveandcapture.h"
#include "printing.h"

extern Piece board[8][8];

// finding the kingSquare

bool removeCheck(Piece piece, int availableMoveIndex, int testedMove) {
  int kingSquare;

  Piece prevBoard[8][8];
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      prevBoard[i][j] = board[i][j];
    }
  }

  int pieceColour = piece.colour;
  int oppositeColour = piece.colour == WHITE ? BLACK : WHITE;

  // here we make the move
  int rankNew = piece.availableMoves[availableMoveIndex] / 10;
  int fileNew = piece.availableMoves[availableMoveIndex] % 10;

  board[rankNew][fileNew] = board[(8 - piece.rank)][(piece.file - 97)];
  board[(8 - piece.rank)][(piece.file - 97)] = empty;

  kingSquare = findKingSquare(piece.colour);
  printf("This is the hypothetical board\n");

  printBoard(board);

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (board[i][j].type != '_') {
        if (board[i][j].colour == oppositeColour) {
          // pieceColour is the turn that it is not, because we are moving the
          // other colour for hypothetical
          changeAvailableMoves(&board[i][j], pieceColour);
          int size = arraySize(board[i][j].availableMoves);
          for (int k = 0; k < size; k++) {
            if (board[i][j].availableMoves[k] == kingSquare) {
              printf("These are all the legal moves:");
              printf("We're returning true for %c%d\n", piece.file, piece.rank);
              printf("The move was %d\n", testedMove);
              printf("The enemy piece was %c\n", board[i][j].type);

              for (int l = 0; l < 8; l++) {
                for (int m = 0; m < 8; m++) {
                  board[l][m] = prevBoard[l][m];
                }
              }
              printBoard(board);
              return true;
            }
          }
        }
      }
    }
  }

  for (int l = 0; l < 8; l++) {
    for (int m = 0; m < 8; m++) {
      board[l][m] = prevBoard[l][m];
    }
  }
  printBoard(board);
  return false;
}
