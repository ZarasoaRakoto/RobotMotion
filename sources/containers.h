#ifndef _CONTAINERS_
#define _CONTAINERS_
/*
   FIchiers header de déclaration des structures de données nécessaires
*/
#include "stdclibs.h"
#include "datatypes.h"

/* Interface de la file de priorité */
typedef struct {
    int maxElems; // nombre d'éléments maximum prédit
    int nElems;  // nombre d'éléments dans la file
    PQItem *items;
} PQ;

PQ* PQinit(int maxElems, bool inplace);
PQ* PQheapify(PQItem *items, int nElems); // inplace heapifying
bool PQinstert(PQ *pq, PQItem v);
PQItem* PQdelmax(PQ *pq);
PQItem* PQfindmax(PQ *pq);
bool PQchange(PQ *pq, PQItem *item, double key);  // Increase or decrease node key
bool PQRemove(PQ *pq, PQItem *item);  // Delete a node
bool PQEmpty(PQ *pq);
bool PQDelete(PQ *pq);
void fixUP(PQItem *items, int k, int nElems); // Restore heap conformity upward
void fixDown(PQItem *items, int k, int nElems);  // Restore heap conformity downward


#endif