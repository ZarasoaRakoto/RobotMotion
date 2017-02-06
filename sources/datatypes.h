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
    Polygon *polygons;
    int size; int maxSize;
} Obstacles;
void init_obstacles(Obstacles *obstacles, int maxSize);
void shrink(Polygon* obstacles, int *intervals, int n_actifs, int i1, int i2, int j);
void add_point(Polygon* polygon, Raw_point p);
void insert_polygon(Polygon *p, int maxSize);
void add_polygon(Obstacles* obstacles, int k);
void init_polygon(int maxSize);
void insert_interval(int *intervals, int n_actifs, int *n_max, int i1, int i2, int j, int *k);

static int nb_obs_points = 10;

#endif