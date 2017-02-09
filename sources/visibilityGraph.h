#ifndef _VGRAPH_ 
#define _VGRAPH_

#include "stdclibs.h"
#include "datatypes.h"

void computeVgraph(VGnode *pointlist, int size);
bool process(Raw_point*, Raw_point*);
void addRightmost(VGnode *p, VGnode *q);
void addLeftOf(VGnode *p, VGnode *q);
void removeNode(VGnode *p);
static int compare_points (void const *a, void const *b);
void sortNodes(VGnode * pointlist, int size);
bool leftTurn(VGnode *p, VGnode *q, VGnode *r);
Raw_point* otherNode(Raw_point *p);
bool gatherEdge(Raw_point*, Raw_point*);

VGedge Edge(VGnode *);
VGedge VISIBLE(VGnode *);
bool before(VGnode*, VGnode*, VGedge);

#endif