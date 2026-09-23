//trace.h
#ifndef TRACE_H
#define TRACE_H

#define MAX_TRACE_EVENTS 200

typedef enum
{
    OP_INVALID = 0,
    OP_PUSH,
    OP_POP,
    OP_PEEK
} OperationType;

typedef enum
{
    STACK_MAIN,
    STACK_TEMP
} StackID;

typedef enum 
{
    TRACE_SUCCESS,
    TRACE_STACK_FULL,
    TRACE_STACK_EMPTY
} TraceStatus;

typedef struct
{
    OperationType operation;
    int value;
    StackID stack;
    TraceStatus status;
    const char *message;
} TraceEvent;

typedef struct
{
    TraceEvent events[MAX_TRACE_EVENTS];
    int traceCount;
} TraceList;

//prototypes

void initTrace(TraceList *trace);

void clearTrace(TraceList *trace);

void recordOperation(TraceList *trace, OperationType op, int val, StackID st, TraceStatus status, const char *msg);

TraceEvent getEvent(TraceList *trace, int index);

int getTraceCount(TraceList *trace);

#endif
