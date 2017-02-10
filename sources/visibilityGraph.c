#include "containers.h"
#include "visibilityGraph.h"
#include "stack.h"

/* 
	visibility graph algorithm
	using rotation sweep trees from Welzl & Overmars [March88] paper
	complexity worst case O(n^2)
*/

void computeVgraph(VGnode *pointlist, int size){
	sortNodes(pointlist, size);
	Raw_point pinf = {pointlist[0].point->x, DBL_MAX, DBL_MAX, NULL, NULL};
	Raw_point mpinf = {pointlist[0].point->x, DBL_MIN, DBL_MAX, NULL, NULL};
	VGnode pInf = {&pinf, NULL, NULL, NULL, NULL};
	VGnode mpInf = {&mpinf, NULL, NULL, NULL, NULL};
	addRightmost(&mpInf, &pInf);
	int i;
	for(i=0; i<size; i++) addRightmost(&pointlist[i], &mpInf);
	stackT nodeStack;
	StackInit(&nodeStack, 20); StackPush(&nodeStack, &pointlist[0]);
	VGnode *p = NULL, *pr = NULL, *q = NULL, *z = NULL;
	while(!StackIsEmpty(&nodeStack)){
		p = StackPop(&nodeStack);
		pr = p->right; q = p->father;
		if (q != &mpInf) assert(process(p->point, q->point)); // crutial
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
}

bool process(Raw_point *p, Raw_point *q){
	if (q == otherNode(p)) return gatherEdge(p,q);
	//else if (Edge(q) == VISIBLE(p)) {VISIBLE(p) = VISIBLE(q); return gatherEdge(p,q);}
	//else if (before(p, q, VISIBLE(p))) {VISIBLE(p) = Edge(q); return gatherEdge(p,q);}
	return true;
}

Raw_point* otherNode(Raw_point *p) {
	return (p->adjlist->head ? p->adjlist->head->item : NULL);
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

int compare_points (void const *a, void const *b)
{
   Raw_point const *pa = ((VGnode*)a)->point;
   Raw_point const *pb = ((VGnode*)b)->point;
   return ((pa->x < pb->x) || ((pa->x == pb->x) && (pa->y < pb->y)));
}

void sortNodes(VGnode * pointlist, int size){
	qsort (pointlist, size, sizeof *pointlist, compare_points);
}

// Manque une assertion validant que ni pa, ni q, ni r ne peuvent etre -pInf
bool leftTurn(VGnode *p, VGnode *q, VGnode *r){
	assert(p->father && q->father);
	if (!r->father) return (compare_points(p , q) ? true : false); // handling pInf case
	Raw_point *pp = p->point, *qp = q->point, *rp = r->point;
	return ((((qp->x - pp->x) * (rp->y - pp->y) - (rp->x - pp->x) * (qp->y - pp->y)) > 0) ? true : false);
}

bool gatherEdge(Raw_point *p1, Raw_point *p2){
	// populating the motion visibility graph
	if (!(p1->adjlist->head) || (p1->adjlist->head->item != p2)) list_insert(p1->adjlist, p2);
	return true;
}

VGedge Edge(VGnode *n){
	VGedge e =  {n->point, n->point->adjlist->head->item};
	return e;
}
VGedge VISIBLE(VGnode *n){
	VGedge e =  {NULL, NULL};
	return e;
}
bool before(VGnode *p, VGnode *q, VGedge e){
	return true;
}
