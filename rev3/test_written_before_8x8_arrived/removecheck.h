#ifndef REMOVECHECK
#define REMOVECHECK

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "constantsandstructs.h"

int removeCheck(Piece piece, int availableMoveIndex, int testedMove);
void deepCopyPiece(Piece *dest, Piece *src);

#ifdef __cplusplus
}
#endif

#endif