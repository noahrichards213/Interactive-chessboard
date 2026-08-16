#ifndef PROMOTION
#define PROMOTION

#ifdef __cplusplus
extern "C" {
#endif

#include "constantsandstructs.h"

Piece executePromotion(int colour, int rankDest, char fileDest);

#ifdef __cplusplus
}
#endif

#endif