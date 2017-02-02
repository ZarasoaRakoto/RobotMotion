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


typedef struct VGpt{
    double x;
    double y;
    struct VGpt *father;
    struct VGpt *left;
    struct VGpt *right;
    struct VGpt *rightMostSon;
} VGnode;

typedef struct pt {
    double x;
    double y;
    pt *next;
} Raw_point;

typedef struct {
    Raw_points *points;
    int size; int maxSize;
} Polygon;

typedef struct {
    Polygon *obstacles;
    int size; int maxSize;
} Obstacles;

#endif