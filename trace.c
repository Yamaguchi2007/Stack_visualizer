//trace.c

#include "trace.h"

void initTrace(TraceList *trace)
{
    trace->traceCount = 0;
}
void clearTrace(TraceList *trace)
{
    trace->traceCount = 0;
}

void recordOperation(TraceList *trace, OperationType op, int val, StackID st, TraceStatus rec_status, const char *msg)
{
    if(trace->traceCount < MAX_TRACE_EVENTS)
    {
        trace->events[trace->traceCount].operation = op;
        trace->events[trace->traceCount].value = val;
        trace->events[trace->traceCount].stack = st;
        trace->events[trace->traceCount].status = rec_status;       
        trace->events[trace->traceCount].message = msg;
        trace->traceCount++;
    }
}
TraceEvent getEvent(TraceList *trace, int index)
{
    TraceEvent invalid = {0};
    if(index >= 0 && index < trace->traceCount)
        return trace->events[index];

    else
        return invalid;
}

int getTraceCount(TraceList *trace)
{
    return trace->traceCount;
}
