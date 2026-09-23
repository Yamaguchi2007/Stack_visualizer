#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "stack.h"
#include "trace.h"

#define BUFFER_WIDTH  80
#define BUFFER_HEIGHT 34

typedef struct
{
    int x;
    int y;
    int height;
    int width;
} Rectangle;

/* Visual-only stack state. Actual stack data will live elsewhere. */
typedef struct
{
    int visible;
    int x;
    int y;
    int height;
    int width;
} VisualizerStack;

void clearBuffer(char buffer[BUFFER_HEIGHT][BUFFER_WIDTH]);
void swapBuffers(void);

void drawText(char buffer[BUFFER_HEIGHT][BUFFER_WIDTH], int x, int y, const char *text);
void drawRectangle(char buffer[BUFFER_HEIGHT][BUFFER_WIDTH], Rectangle rect);
void drawBoxWithText(char buffer[BUFFER_HEIGHT][BUFFER_WIDTH], Rectangle rect, const char *text);

void drawTitle(char buffer[BUFFER_HEIGHT][BUFFER_WIDTH]);
void drawLogs(char buffer[BUFFER_HEIGHT][BUFFER_WIDTH], TraceList *trace);
void drawStacks(char buffer[BUFFER_HEIGHT][BUFFER_WIDTH], Stack *mainStack, Stack *tempStack);


void drawOperations(char buffer[BUFFER_HEIGHT][BUFFER_WIDTH]);
void drawInput(char buffer[BUFFER_HEIGHT][BUFFER_WIDTH]);

void drawUI(char buffer[BUFFER_HEIGHT][BUFFER_WIDTH], Stack *mainStack, Stack *tempStack, TraceList *trace);

void renderBuffer(char buffer[BUFFER_HEIGHT][BUFFER_WIDTH]);

char (*getBackBuffer(void))[BUFFER_WIDTH];

void visualizerInit(void);
void visualizerShutdown(void);

void showPeekIndicator(int index);
void hidePeekIndicator(void);

#endif
