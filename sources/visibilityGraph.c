#include "containers.h"
#include "visibilityGraph.h"


void addRightmost(VGnode *p, VGnode *q){
    assert(q); assert(p);
    p->father = q;
    p->left = q->rightMostSon;
    p->right = NULL;
    p->rightMostSon = NULL;
    q->rightMostSon = p;
}

void addLeftOf(VGnode *p, VGnode *q){
    assert(q); assert(p);
    p->father = q->father;
    p->right = q;
    p->left = q->left;
    p->rightMostSon = NULL;
    q->left = p;
}

// Be sure that all p sons' fathers have been updated
void removeNode(VGnode *p){
    assert(p);
    if(p->left) p->left->right = p->right;
    if(p->right) p->right->left = p->left;
    free(p);
    p = NULL;
}

VGnode* rightBrother(VGnode *p){
    return p->right;
}

VGnode* leftBrother(VGnode *p){
    return p->left;
}

VGnode* father(VGnode *p){
    return p->father;
}

VGnode*rightMostson(VGnode *p){
    return p->rightMostSon;
}

static int compare_points (void const *a, void const *b)
{
   VGnode const *pa = a;
   VGnode const *pb = b;
   return ((pa->x < pb->x) || ((pa->x == pb->x) && (pa->y < pb->y)));
}

void sortNodes(VGnode * pointlist, int size){
    qsort (pointlist, size, sizeof *pointlist, compare_points);
}

// Manque une assertion validant que ni pa, ni q, ni r ne peuvent etre -pInf
bool leftTurn(VGnode *p, VGnode *q, VGnode *r){
    assert(p->father && q->father);
    if (!r->father) return (compare_points(p , q) ? true : false); // handling pInf case
    return ((q->x - p->x) * (r->y - p->y) - (r->x - p->x) * (q->y - p->y) > 0) ? true : false;
}