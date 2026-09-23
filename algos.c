// algos.c is split up in three parts
/*
    
    1. Wrapper.
        We will need to record every push,
        pop and peek operations
    
    2. Primitive algorithms.
        These algorithms will be better than the basic-most
        push,pop and peek. But will be reused in other
        algorithms
            1) insertAtIndex
            2) removeAtIndex
            3) insertAtBottom

    3. Advanced algorithms
        These Algorithms will be taking use of the 
        primitive algorithms to modify the stack.
            1) Reverse.
            (and more)
*/

#include<stdlib.h>

//#include "stack.h"
//#include "trace.h"
#include "algos.h"

// Wrappers
bool algoPush(Stack *s,
            int value,
            TraceList *trace,
            StackID stackID)
{
    
    if(s == NULL || trace == NULL)
        return false;               // send fools BACK

    if(push(s, value))
    {
        recordOperation(trace, OP_PUSH, value, stackID, TRACE_SUCCESS,"Push Successful");
        return true;
    }
    
    recordOperation(trace, OP_PUSH, value, stackID, TRACE_STACK_FULL, "Stack Full");
    return false;

}

bool algoPop(Stack *s,
            int *location,
            TraceList *trace,
            StackID stackID)
{
    if(s == NULL || trace == NULL)
        return false;

    if(pop(s, location))
    {
        recordOperation(trace, OP_POP, *location, stackID, TRACE_SUCCESS,"Pop Successful");
        return true;
    }

    recordOperation(trace, OP_POP, -1, stackID, TRACE_STACK_EMPTY, "Stack Empty");
    return false;

}


bool algoPeek(Stack *s,
            int *location,
            TraceList *trace,
            StackID stackID)
{
    if(s == NULL || trace == NULL)
        return false;

    if(peek(s, location))
    {
        recordOperation(trace, OP_PEEK, *location,stackID,TRACE_SUCCESS,"Peek Successful");
        return true;
    }

    recordOperation(trace, OP_PEEK, -1, stackID, TRACE_STACK_EMPTY, "Stack Empty");
    return false;

}

// Primitive Algorithms


bool insertAtIndex(Stack *s, Stack *tempStack, int ins_index, int ins_value, TraceList *trace)
{
    if (s == NULL || tempStack == NULL || trace == NULL)
        return false;

    if (ins_index < 0 || ins_index > size(s))
        return false;

    if (isFull(s))
        return false;

    while (!isEmpty(tempStack))
    {
        int box;
        pop(tempStack, &box);
    }

    if (ins_index == 0)
        return algoPush(s, ins_value, trace, STACK_MAIN);

    int box;

    int data_to_move = ins_index;

    while (data_to_move--)
    {
        if (!algoPop(s, &box, trace, STACK_MAIN))
            return false;

        if (!algoPush(tempStack, box, trace, STACK_TEMP))
            return false;
    }

    if (!algoPush(s, ins_value, trace, STACK_MAIN))
        return false;

    while (!isEmpty(tempStack))
    {
        if (!algoPop(tempStack, &box, trace, STACK_TEMP))
            return false;

        if (!algoPush(s, box, trace, STACK_MAIN))
            return false;
    }

    return true;
}



bool insertAtBottom(Stack *s, Stack *tempStack, int value, TraceList *trace)
{
    if(s == NULL || tempStack == NULL || trace == NULL)
        return false;

    if(isFull(s))
        return false;

    // Clear TEMP before using it.
    while(!isEmpty(tempStack))
    {
        int box;
        pop(tempStack, &box);
    }

    // Save the current MAIN size.
    int originalSize = size(s);

    if(originalSize == 0)
    {
        if(!algoPush(s, value, trace, STACK_MAIN))
            return false;

        return true;
    }

    // Save the current MAIN values.
    int *data = malloc(originalSize * sizeof(int));

    if(data == NULL)
        return false;

    for(int i = 0; i < originalSize; i++)
        data[i] = s->data[i];

    // Insert the new value at the bottom.
    s->top = -1;

    if(!algoPush(s, value, trace, STACK_MAIN))
    {
        free(data);
        return false;
    }

    for(int i = 0; i < originalSize; i++)
    {
        if(!algoPush(s, data[i], trace, STACK_MAIN))
        {
            free(data);
            return false;
        }
    }

    // Show the previous MAIN contents in TEMP.
    for(int i = originalSize - 1; i >= 0; i--)
    {
        if(!push(tempStack, data[i]))
        {
            free(data);
            return false;
        }
    }

    free(data);

    return true;
}


bool reverseStack(Stack *s, Stack *tempStack, TraceList *trace)
{
    if(s == NULL || trace == NULL || tempStack == NULL)
        return false;

    if(isEmpty(s))
        return true; // base case for the recursion

    int box; // ANOTHER ONE

    if(!algoPop(s, &box, trace, STACK_MAIN))
        return false;

    if(!reverseStack(s, tempStack, trace))
        return false;

    if(!insertAtBottom(s, tempStack, box, trace))
        return false;

    return true;
}





