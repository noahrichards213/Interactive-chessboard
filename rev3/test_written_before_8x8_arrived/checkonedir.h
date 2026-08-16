#ifndef CHECKONEDIR
#define CHECKONEDIR

#ifdef __cplusplus
extern "C" {
#endif

#include "constantsandstructs.h"

// checks one direction, allows to check for rook, bishop and queen moves
int* checkOneDir(Piece piece, int hort, int vert);

#ifdef __cplusplus
}
#endif

#endif
