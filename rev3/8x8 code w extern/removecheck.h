#ifndef REMOVECHECK
#define REMOVECHECK

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "constantsandstructs.h"

bool removeCheck(Piece piece, int availableMoveIndex, int testedMove);

#ifdef __cplusplus
}
#endif

#endif