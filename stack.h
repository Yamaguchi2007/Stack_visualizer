#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    int top;
    int capacity;
    int *data;
} Stack;

Stack *stack_init(size_t req_size);
void destroy_stack(Stack *s);

bool push(Stack *s, int num);
bool pop(Stack *s, int *value);
bool peek(Stack *s, int *value);

bool isEmpty(Stack *s);
bool isFull(Stack *s);
int size(Stack *s);

int stackSize(Stack *s);
#endif
