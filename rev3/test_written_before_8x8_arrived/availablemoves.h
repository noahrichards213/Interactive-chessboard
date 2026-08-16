#ifndef AVAILABLEMOVES
#define AVAILABLEMOVES

#ifdef __cplusplus
extern "C" {
#endif

#include "arraysize.h"
#include "constantsandstructs.h"

// returns array of legal moves
int* rookMoves(Piece piece);
int* bishopMoves(Piece piece);
int* queenMoves(Piece piece);
int* knightMoves(Piece piece);
int* pawnMoves(Piece piece);
int* kingMoves(Piece piece);

#ifdef __cplusplus
}
#endif

#endif