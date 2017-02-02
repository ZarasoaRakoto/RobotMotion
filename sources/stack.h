#ifndef _STACK_H_
#define _STACK_H
#include "datatypes.h"

/* stack implementation */
typedef VGnode* stackElementT;

typedef struct {
  stackElementT *contents;
  int maxSize;
  int top;
} stackT;

void StackInit(stackT *stackP, int maxSize);
void StackDestroy(stackT *stackP);
void StackPush(stackT *stackP, stackElementT element);
stackElementT StackPop(stackT *stackP);
int StackIsEmpty(stackT *stackP);
int StackIsFull(stackT *stackP);

#endif