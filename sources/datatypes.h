#ifndef _DATATYPES_
#define _DATATYPES_
#include "stdclibs.h"
/* définitions des types de données nécessaies */
typedef struct {
    double x; // X coordinate
    double y; // Y coordinate
    double key; // cost key
} Point;

typedef Point PQItem;

bool item_less(PQItem *i1, PQItem *i2);

#endif