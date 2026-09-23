//goal is to make a stack which you can see in the terminal
// eg:
//      push 5 will add 5 to the stack
//      pop 5 will remove the 5
//      we can see every changes
//      exit will exit the program
//#include <stdio.h>
#include <stdlib.h>
//#include <stdbool.h>
#include "stack.h"

/*
typedef struct
{
    int top;
    int capacity;
    int *data;
} Stack;
*/

//prototypes
//bool isEmpty(Stack *);
//bool isFull(Stack *);

Stack *stack_init(size_t req_size)
{
    //malloc a stack and return the address.
    if(req_size == 0)
        return NULL;

    Stack *s = malloc(sizeof(Stack));
    if(s == NULL)
        return NULL;
   
    s->data = malloc(req_size * sizeof(int));
    
    if(s->data == NULL)
    {
        free(s);
        return NULL;
    }
    
    s->top = -1;
    s->capacity = req_size;

    return s;
}

void destroy_stack(Stack *s)
{
    if(s == NULL)
        return;

    free(s->data);
    free(s);

}

// Operations
bool push(Stack *s, int num)
{
    if(s == NULL)
        return false;

    if(isFull(s))
        return false;
    
    s->data[++s->top] = num;

    return true;

}

bool pop(Stack *s, int *value)
{
    if(s == NULL || value == NULL)
        return false;

    if(isEmpty(s))
        return false;
    
    *value = s->data[s->top];
    s->top--;
    //s->data[s->top] == NULL no need to remove it even, once top decrements, its unused

    return true;
}

bool peek(Stack *s, int *value)
{

    if(s == NULL || value == NULL)
        return false;

    if(isEmpty(s))
        return false;

    *value= s->data[s->top];
    return true;

}

//Checks:
bool isEmpty(Stack *s)
{

    if(s == NULL)
        return false;

    return s->top < 0;
}

bool isFull(Stack *s)
{
    
    if(s == NULL)
        return false;

    return s->top == s->capacity - 1;
}

int size(Stack *s)
{
    if(s == NULL)
        return -1;

    return s->top + 1;
}


