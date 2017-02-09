/* 
   Implementation des strcutures de données nécessaisres pour l'exécution des algotithmes
   de motion planning
*/
#include "containers.h"
bool item_less(PQItem i1, PQItem i2){
    return (i1->key < i2->key ? true : false);
}

PQ* PQinit(uint maxElems, bool inplace){
	PQ* pq = malloc(sizeof(*pq));
	if (!pq) return NULL;
	pq->maxElems = MAX(1, maxElems);
	pq->nElems = 0;
	if (!inplace) pq->items = malloc((pq->maxElems) * sizeof(PQItem));
	if (!inplace && !pq->items) { free(pq); return NULL; }
	return pq;
}

PQ* PQheapify(PQItem *items, uint nElems){
	assert (nElems > 0);
	PQ* pq = PQinit(nElems, true);
	if (!pq) return NULL;
	pq->nElems = nElems;
	pq->items = items;
	for(uint k = nElems/2; k>=0; k--){
		fixDown(pq->items, k, nElems);
	}
	return pq;
}

bool PQinstert(PQ *pq, PQItem v){
	uint nElems = pq->nElems, maxElems = pq->maxElems;
	if (nElems >= maxElems) pq = PQrealloc(pq, maxElems*2);
	if (!pq) return false;
	pq->items[nElems] = v;
	pq->nElems++;
	fixUP(pq->items, nElems, nElems+1);
	return true;
}

PQ* PQrealloc(PQ *pq, uint newMaxElems){
	if(!pq) return NULL;
	newMaxElems = MAX(newMaxElems,1); 
	pq->items = realloc (pq->items, newMaxElems * sizeof(PQItem));
	if (!pq->items) { free(pq); return NULL; }
	pq->maxElems = newMaxElems;
	return pq;
}

void fixDown(PQItem *items, uint k, uint nElems){
	uint j; PQItem temp;
	while(2*k < nElems){
		j = 2*k;
		temp = items[k];
		if ((j < nElems-1) && item_less(items[j+1], items[j])) j++;
		if (item_less(temp, items[j])) break;
		items[k] = items[j]; // swapping when necessary
		items[j] = temp;
		k = j;
	}
}

void fixUP(PQItem *items, uint k, uint nElems){
	assert(k<nElems);
	PQItem temp;
	while (k>0 && item_less(items[k], items[k/2])){
		// swapping when necessary
		temp = items[k];
		items[k] = items[k/2];
		items[k/2] = temp;
		k /= 2;
	}
}

bool PQDelete(PQ *pq){
	if(pq) {
		if (pq->items) free(pq->items);
		free(pq);
	}
	return true;
}

PQItem PQdelmax(PQ *pq){
	assert (pq->nElems);
	PQItem maxit = pq->items[0];
	pq->items[0] = pq->items[--pq->nElems];
	fixDown(pq->items, 0, pq->nElems);
	return maxit;
}

PQItem* PQfindmax(PQ *pq){
	if (!pq->nElems) return NULL;
	return &(pq->items[0]);
}

bool PQchange(PQ *pq, uint rank, double key){
	PQItem *items = pq->items;
	double last_key = items[rank]->key;
	if (last_key == key) return true;
	items[rank]->key = key;
	if (last_key > key) fixUP(pq->items, rank, pq->nElems);
	else fixDown(pq->items, rank, pq->nElems);
	return true;
}
PQItem PQRemove(PQ *pq, uint rank){
	assert(rank < pq->nElems);
	PQItem item = pq->items[rank];
	pq->items[rank] = pq->items[--pq->nElems];
	fixDown(pq->items, rank, pq->nElems);
	return item;
}

bool PQEmpty(PQ *pq){
	return (pq->nElems ? false : true);
}

AdjList* init_list(void){
	AdjList *new = malloc(sizeof(*new)); assert(new);
	new->head = NULL;
	return new;
}

void list_insert(AdjList *L, ListItem i){ // always downward : the first adj element should remain the first
	AdjListNode *new = malloc(sizeof(*new)); assert(new);
	new->item = i;
	if(L->head) {
		new->next = L->head->next; L->head->next = new;
	}
	else { L->head = new; new->next = NULL}
}

void list_delete(AdjListNode *L){
	if(L) {
		if (L->next) list_delete(L->next);
		free(L);
	}
}