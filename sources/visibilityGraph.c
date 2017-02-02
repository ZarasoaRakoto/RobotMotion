#include "datatypes.h"
#include "visibilityGraph.h"
#include "stack.h"

/* 
	visibility graph algorithm
	using rotation sweep trees from Welzl & Overmars [March88] paper
	complexity worst case O(n^2)
*/

void * computeVgraph(VGnode * pointlist, int size){
	sortNodes(pointlist, size);
	VGnode pInf = {pointlist[0].x, DBL_MAX, NULL, NULL, NULL, NULL};
	VGnode mpInf = {pointlist[0].x, DBL_MIN, NULL, NULL, NULL, NULL};
	addRightmost(&mpInf, &pInf);
	int i;
	for(i=0; i<size; i++) addRightmost(&pointlist[i], &mpInf);
	stackT nodeStack;
	StackInit(&nodeStack, 20); StackPush(&nodeStack, &pointlist[0]);
	VGnode *p = NULL, *pr = NULL, *q = NULL, *z = NULL;
	while(!StackIsEmpty(&nodeStack)){
		p = StackPop(&nodeStack);
		pr = p->right; q = p->father;
		if (q != &mpInf) process(p,q); // crutial
		z = q->left;
		removeNode(p);
		if(!z || leftTurn(p, z, z->father)==false) addLeftOf(p,q);
		else {
			while(z->rightMostSon && leftTurn(p, z->rightMostSon, z)==true) z = z->rightMostSon;
			addRightmost(p,z);
			if (z == StackTop(&nodeStack)) z = StackPop(&nodeStack); 
		}
		if (!p->left && p->father != &pInf) StackPush(&nodeStack, p);
		if (!pr)  StackPush(&nodeStack, pr);
	}
	return NULL;
}

int process(VGnode *p, VGnode *q){
	if (q == otherNode(p)) gatherEdge(p,q);
	else if (Edge(q) == VISIBLE(p)) {VISIBLE(p) = VISIBLE(q); gatherEdge(p,q);}
	else if (before(p, q, VISIBLE(p))) {VISIBLE(p) = Edge(q); gatherEdge(p,q);}
	return 0;
}

VGnode* otherNode(VGnode *p) {
	return
}
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