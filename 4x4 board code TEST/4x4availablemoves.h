#ifndef AVAILABLEMOVES
#define AVAILABLEMOVES

#include "4x4arraysize.h"
#include "4x4constantsandstructs.h"

// returns array of legal moves
int* rookMoves(Piece piece);
int* bishopMoves(Piece piece);
int* queenMoves(Piece piece);
int* knightMoves(Piece piece);
int* pawnMoves(Piece piece);
int* kingMoves(Piece piece);

#endif