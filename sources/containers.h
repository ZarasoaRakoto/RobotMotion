#ifndef _CONTAINERS_
#define _CONTAINERS_
/*
   FIchiers header de déclaration des structures de données nécessaires
*/
#include "stdclibs.h"
#include "datatypes.h"

bool item_less(PQItem i1, PQItem i2);

/* Interface de la file de priorité */
typedef struct {
    uint maxElems; // nombre d'éléments maximum prédit
    uint nElems;  // nombre d'éléments dans la file
    PQItem *items;
} PQ;

PQ* PQinit(uint maxElems, bool inplace);
PQ* PQheapify(PQItem *items, uint nElems); // inplace heapifying
bool PQinstert(PQ *pq, PQItem v);
PQ* PQrealloc(PQ *pq, uint newMaxElems);
PQItem PQdelmax(PQ *pq);
PQItem* PQfindmax(PQ *pq);
bool PQchange(PQ *pq, uint rank, double key);  // Increase or decrease node key
PQItem PQRemove(PQ *pq, uint rank);  // Delete a node
bool PQEmpty(PQ *pq);
bool PQDelete(PQ *pq);
void fixUP(PQItem *items, uint k, uint nElems); // Restore heap conformity upward
void fixDown(PQItem *items, uint k, uint nElems);  // Restore heap conformity downward

void list_delete(struct AdjListNode *L);
void list_insert(AdjList *L, ListItem i);



/***************************************************************************************/

#endif