#ifndef _DATATYPES_
#define _DATATYPES_
#include "stdclibs.h"
/* définitions des types de données nécessaies */

bool item_less(PQItem *i1, PQItem *i2);

typedef struct pt {
    double x;
    double y;
    double key;       // used for dijkstra algorithm
    struct pt *next;  // used as leftmost son for de dijkstra algorithm 
    struct pt *right; // right brother, useful for Dijkstra algorithm graph representation
    struct pt *prev;  // useful for reconstructing the path from the goal node
} Raw_point;

typedef Raw_point* PQItem; // too many memory copies for fixup and fixdown heap operations !!

typedef struct VGpt{
    Raw_point *point;  // pointer on polygons points and begin-end points
    struct VGpt *father;
    struct VGpt *left;
    struct VGpt *right;
    struct VGpt *rightMostSon;
} VGnode;

typedef struct{
    Raw_point *base, *next;
} VGedge;


typedef struct {
    Raw_point *points;
    int size; int maxSize;
} Polygon;

typedef struct {
    Polygon *polygons;
    int size; int maxSize;
} Obstacles;


#define nb_obs_points 3

#endif