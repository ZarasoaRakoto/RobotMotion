/* 
   Implementation des strcutures de données nécessaisres pour l'exécution des algotithmes
   de motion planning
*/
#include "containers.h"
bool item_less(PQItem *i1, PQItem *i2){
    return (i1->key < i2->key ? true : false);
}

PQ* PQinit(int maxElems, bool inplace){
	PQ* pq = malloc(sizeof(*pq));
	if (!pq) return NULL;
	pq->maxElems = MAX(1, maxElems);
	pq->nElems = 0;
	if (!inplace) pq->items = malloc((pq->maxElems) * sizeof(PQItem));
	if (!inplace && !pq->items) { free(pq); return NULL; }
	return pq;
}

PQ* PQheapify(PQItem *items, int nElems){
	assert (nElems > 0);
	PQ* pq = PQinit(nElems, true);
	if (!pq) return NULL;
	pq->nElems = nElems;
	pq->items = items;
	for(int k = nElems/2; k>=0; k--){
		fixDown(pq->items, k, nElems);
	}
	return pq;
}

void fixDown(PQItem *items, int k, int nElems){
	int j; PQItem temp;
	while(2*k < nElems){
		j = 2*k;
		temp = items[k];
		if ((j < nElems-1) && item_less(&items[j+1], &items[j])) j++;
		if (item_less(&temp, &items[j])) break;
		items[k] = items[j]; // swapping when necessary
		items[j] = temp;
		k = j;
	}
}

void fixUP(PQItem *items, int k, int nElems){
	assert(k<nElems);
	PQItem temp;
	while (k>0 && item_less(&items[k], &items[k/2])){
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