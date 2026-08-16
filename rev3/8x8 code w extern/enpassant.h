#ifndef ENPASSANT
#define ENPASSANT

#ifdef __cplusplus
extern "C" {
#endif

#include "constantsandstructs.h"

// en passant functions
void removeEnPassant();

void addEnPassant(Piece piece, int rank, char file);

#ifdef __cplusplus
}
#endif

#endif