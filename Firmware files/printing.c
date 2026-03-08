#include "printing.h"

#include <math.h>
#include <stdio.h>

extern Piece board[8][8];

void printBoard() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      printf("%c", board[i][j].type);
    }
    printf("\n");
  }
}

void printLegalMoves() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (board[i][j].type != '_') {
        printf("%c on %c%d can move to: ", board[i][j].type, board[i][j].file,
               board[i][j].rank);
        int k = 0;
        if (board[i][j].availableMoves != NULL) {
          while (board[i][j].availableMoves[k] != -1) {
            int rankNew;
            int fileNew;
            if (board[i][j].availableMoves[k] < 100) {
              rankNew = (7 - (board[i][j].availableMoves[k] / 10));
              fileNew = board[i][j].availableMoves[k] % 10;
              printf("%c%d", 'a' + fileNew, (rankNew + 1));
            } else if (board[i][j].availableMoves[k] > 100 &&
                       board[i][j].availableMoves[k] < 10000) {
              // castle move
              rankNew = ((7 - (board[i][j].availableMoves[k] / 100) / 10));
              fileNew = ((board[i][j].availableMoves[k]) / 100) % 10;

              printf("%c%d", 'a' + fileNew, (rankNew + 1));
            } else if (board[i][j].availableMoves[k] > 10000) {
              rankNew = ((7 - (board[i][j].availableMoves[k] / 10000) / 10));
              fileNew = ((board[i][j].availableMoves[k]) / 10000) % 10;
              printf("%c%d", 'a' + fileNew, (rankNew + 1));
            }
            if (board[i][j].availableMoves[k + 1] != -1) {
              printf(", ");
            }
            k++;
          }
        }
        printf("\n");
      }
    }
  }
}

void printPieceInfo(Piece piece) {
  if (piece.type == '_') {
    printf("Empty square");
    return;
  }
  printf("Piece info:");
  printf("Type: %c\n", piece.type);
  printf("Colour: %d\n", piece.colour);
  printf("Rank: %d\n", piece.rank);
  printf("File: %c\n", piece.file);
  printf("Available moves:");
  int i = 0;
  while (piece.availableMoves[i] != -1) {
    printf("%d", piece.availableMoves[i]);
    printf(", ");
    i++;
  }
}
