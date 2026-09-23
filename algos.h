#ifndef ALGOS_H
#define ALGOS_H

#include <stdbool.h>

#include "stack.h"
#include "trace.h"

/* ===========================
 * Wrapper Functions
 * =========================== */

bool algoPush(Stack *s,
              int value,
              TraceList *trace,
              StackID stackID);

bool algoPop(Stack *s,
             int *location,
             TraceList *trace,
             StackID stackID);

bool algoPeek(Stack *s,
              int *location,
              TraceList *trace,
              StackID stackID);

/* ===========================
 * Primitive Algorithms
 * =========================== */

bool insertAtIndex(Stack *s,
                   Stack *tempStack,
                   int ins_index,
                   int ins_value,
                   TraceList *trace);


bool insertAtBottom(Stack *s,
                   Stack *tempStack,
                   int value,
                   TraceList *trace);

bool reverseStack(Stack *s,
                  Stack *tempStack,
                  TraceList *trace);

#endif /* ALGOS_H */
