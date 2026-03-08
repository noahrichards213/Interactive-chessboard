#ifndef AVAILABLEMOVES
#define AVAILABLEMOVES

#include "arraysize.h"
#include "constantsandstructs.h"

// returns array of legal moves
int* rookMoves(Piece piece);
int* bishopMoves(Piece piece);
int* queenMoves(Piece piece);
int* knightMoves(Piece piece);
int* pawnMoves(Piece piece);
int* kingMoves(Piece piece);

#endif