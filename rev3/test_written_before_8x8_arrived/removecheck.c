#include "removecheck.h"
#include "esp_system.h"

#include "arraysize.h"
#include "findkingsquare.h"
#include "legalmoveandcapture.h"
#include "printing.h"

#include <string.h>
extern Piece board[8][8];

// finding the kingSquare

int removeCheck(Piece piece, int availableMoveIndex, int testedMove) {
  int kingSquare = -1;

  Piece prevBoard[8][8];

  for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
          prevBoard[i][j] = board[i][j];

          // 1. Verify source pointer is non-NULL
          if (board[i][j].availableMoves != NULL) {
              
              // 2. Validate array size
              int count = arraySize(board[i][j].availableMoves);
              if (count < 0 || count > 30) { 
                  // Bounds guard against garbage memory
                  printf("Error: Invalid arraySize (%d) at [%d][%d]\n", count, i, j);
                  prevBoard[i][j].availableMoves = NULL;
                  continue;
              }


              int numMoves = count + 1;
              size_t copyBytes = numMoves * sizeof(int);

              // 3. Allocate memory

              // printf("[%d][%d] count: %d, copyBytes: %u, Free Heap: %u\n", 
              // i, j, count, (unsigned int)copyBytes, (unsigned int)esp_get_free_heap_size());


              prevBoard[i][j].availableMoves = (int*)malloc(copyBytes);


              // 4. STRICT WRITE GUARD: Only write if destination is non-NULL
              if (prevBoard[i][j].availableMoves != NULL) {
                  memcpy(prevBoard[i][j].availableMoves, board[i][j].availableMoves, copyBytes);
              } 

          } else {
              prevBoard[i][j].availableMoves = NULL;
          }
      }
  }


  int pieceColour = piece.colour;
  int oppositeColour = piece.colour == WHITE ? BLACK : WHITE;

  int move = piece.availableMoves[availableMoveIndex];
  int rankNew;
  int fileNew;

  if (move > 200 && move < 10000) {
    move /= 100;
  }

  if (move > 10000) {
    move /= 1000;
  }
  // here we make the move
  rankNew = move / 10;
  fileNew = move % 10;

  if (board[rankNew][fileNew].availableMoves != NULL) {
    free(board[rankNew][fileNew].availableMoves);
    board[rankNew][fileNew].availableMoves = NULL;
  }

  board[rankNew][fileNew] = board[(8 - piece.rank)][(piece.file - 97)];
  board[(8 - piece.rank)][(piece.file - 97)] = empty;
  board[8 - piece.rank][piece.file - 97].availableMoves = NULL; // Prevents double-free!

  board[rankNew][fileNew].availableMoves = NULL; 

  char kingCharacter = piece.colour == WHITE ? 'K' : 'k';


  for (int l = 0; l < 8; l++){
    for(int m = 0; m < 8; m++){
      if (board[l][m].type == kingCharacter){
        kingSquare = (l * 10) + m;
      }
    }
  }




  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (board[i][j].type != '_') {
        if (board[i][j].colour == oppositeColour) {
          // pieceColour is the turn that it is not, because we are moving the
          // other colour for hypothetical
          changeAvailableMoves(&board[i][j], pieceColour);
          int size = arraySize(board[i][j].availableMoves);

          // printBoard(board);
          for (int k = 0; k < size; k++) {
            if (board[i][j].availableMoves[k] == kingSquare) {
              for (int l = 0; l < 8; l++) {
                for (int m = 0; m < 8; m++) { 
                  free(board[l][m].availableMoves);
                  board[l][m].availableMoves = NULL;
                  board[l][m] = prevBoard[l][m];
                }
              }

              int pieceSquare = i * 10 + j;
              return pieceSquare;
            } 
          }
        }
      }
    }
  }

  for (int l = 0; l < 8; l++) {
    for (int m = 0; m < 8; m++) {
      free(board[l][m].availableMoves);
      board[l][m].availableMoves = NULL;
      board[l][m] = prevBoard[l][m];
    }
  }
  return -1;
}