#include "stack.h"    

void StackInit(stackT *stackP, int maxSize)
{
  stackElementT *newContents;
  newContents = (stackElementT *)malloc(sizeof(stackElementT) * maxSize);
  assert(newContents);
  stackP->contents = newContents;
  stackP->maxSize = maxSize;
  stackP->top = -1;  /* I.e., empty */
}

void StackDestroy(stackT *stackP)
{
  if(stackP->contents) free(stackP->contents);
  stackP->contents = NULL;
  stackP->maxSize = 0;
  stackP->top = -1;  /* I.e., empty */
}

void StackPush(stackT *stackP, stackElementT element)
{
  if (StackIsFull(stackP)) {
    stackP->contents = realloc(stackP->contents, stackP->maxSize * 2 * sizeof(stackElementT));
    assert(stackP->contents);
    stackP->maxSize *= 2;
  }
  stackP->contents[++stackP->top] = element;
}

stackElementT StackPop(stackT *stackP)
{
  assert(!StackIsEmpty(stackP));
  return stackP->contents[stackP->top--];
}

int StackIsEmpty(stackT *stackP)
{
  return stackP->top < 0;
}

int StackIsFull(stackT *stackP)
{
  return stackP->top >= stackP->maxSize - 1;
}