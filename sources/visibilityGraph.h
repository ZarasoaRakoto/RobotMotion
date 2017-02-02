#ifndef _VGRAPH_ 
#define _VGRAPH_

#include "stdclibs.h"

void* computeVgraph(VGnode *pointlist, int size);
void* process(VGnode *p, VGnode *q);
void addRightmost(VGnode *p, VGnode *q);
void addLeftOf(VGnode *p, VGnode *q);
void removeNode(VGnode *p);
VGnode* rightBrother(VGnode *p);
VGnode* leftBrother(VGnode *p);
VGnode* father(VGnode *p);
VGnode*rightMostson(VGnode *p);
static int compare_points (void const *a, void const *b);
void sortNodes(VGnode * pointlist, int size);
bool leftTurn(VGnode *p, VGnode *q, VGnode *r);

#endif