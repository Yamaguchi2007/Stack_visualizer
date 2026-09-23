#include "vis2_stack_elements.h"

#include <stdio.h>
#include <string.h>

static char bufferA[BUFFER_HEIGHT][BUFFER_WIDTH];
static char bufferB[BUFFER_HEIGHT][BUFFER_WIDTH];

static char (*frontBuffer)[BUFFER_WIDTH] = bufferA;
static char (*backBuffer)[BUFFER_WIDTH] = bufferB;

/* ---------- Visual stack state ---------- */

static VisualizerStack mainVisualStack = {1, 37, 7, 10, 7};
static VisualizerStack tempVisualStack = {1, 60, 7, 10, 7};

/* ---------- Visual stack elements ---------- */

#define MAX_VISUAL_ELEMENTS 5
#define MAX_VISIBLE_LOGS 12


static int peekVisible = 0;
static int peekIndex = -1;


/* ---------- Buffer ---------- */

void clearBuffer(char buffer[BUFFER_HEIGHT][BUFFER_WIDTH])
{
    for (int y = 0; y < BUFFER_HEIGHT; y++)
        for (int x = 0; x < BUFFER_WIDTH; x++)
            buffer[y][x] = ' ';
}

void swapBuffers(void)
{
    char (*temp)[BUFFER_WIDTH] = frontBuffer;
    frontBuffer = backBuffer;
    backBuffer = temp;
}

char (*getBackBuffer(void))[BUFFER_WIDTH]
{
    return backBuffer;
}


/* ---------- Drawing primitives ---------- */

void drawText(char buffer[BUFFER_HEIGHT][BUFFER_WIDTH],
              int x, int y, const char *text)
{
    if (text == NULL || y < 0 || y >= BUFFER_HEIGHT)
        return;

    for (int i = 0; text[i] != '\0'; i++)
    {
        int drawX = x + i;

        if (drawX >= BUFFER_WIDTH)
            break;

        if (drawX >= 0)
            buffer[y][drawX] = text[i];
    }
}

void drawRectangle(char buffer[BUFFER_HEIGHT][BUFFER_WIDTH],
                   Rectangle rect)
{
    for (int i = 0; i < rect.height; i++)
    {
        int y = rect.y + i;

        if (y < 0 || y >= BUFFER_HEIGHT)
            continue;

        for (int j = 0; j < rect.width; j++)
        {
            int x = rect.x + j;

            if (x < 0 || x >= BUFFER_WIDTH)
                continue;

            if (i == 0 || i == rect.height - 1)
                buffer[y][x] = '-';
            else if (j == 0 || j == rect.width - 1)
                buffer[y][x] = '|';
            else
                buffer[y][x] = ' ';
        }
    }
}

void drawBoxWithText(char buffer[BUFFER_HEIGHT][BUFFER_WIDTH],
                     Rectangle rect, const char *text)
{
    drawRectangle(buffer, rect);

    if (text == NULL)
        return;

    int textLength = (int)strlen(text);
    int textX = rect.x + (rect.width - textLength) / 2;
    int textY = rect.y + rect.height / 2;

    drawText(buffer, textX, textY, text);
}


/* ---------- UI ---------- */

void drawTitle(char buffer[BUFFER_HEIGHT][BUFFER_WIDTH])
{
    Rectangle box = {0, 0, 3, BUFFER_WIDTH};

    drawRectangle(buffer, box);
    drawText(buffer, 32, 1, "STACK VISUALIZER");
}

void drawLogs(
    char buffer[BUFFER_HEIGHT][BUFFER_WIDTH],
    TraceList *trace
)
{
    Rectangle box = {0, 3, 16, 28};

    drawRectangle(buffer, box);
    drawText(buffer, 2, 4, "LOGS");

    if (trace == NULL)
        return;

    

    int start = 0;

    if (trace->traceCount > MAX_VISIBLE_LOGS)
    {
        start = trace->traceCount - MAX_VISIBLE_LOGS;
    }

    for (int i = start; i < trace->traceCount; i++)
    {
        int displayRow = 6 + (i - start);

        TraceEvent event = trace->events[i];

        const char *operation;
        const char *stack;
        const char *status;

        switch (event.operation)
        {
            case OP_PUSH:
                operation = "PUSH";
                break;

            case OP_POP:
                operation = "POP";
                break;

            case OP_PEEK:
                operation = "PEEK";
                break;

            default:
                operation = "UNKNOWN";
                break;
        }

        switch (event.stack)
        {
            case STACK_MAIN:
                stack = "MAIN";
                break;

            case STACK_TEMP:
                stack = "TEMP";
                break;

            default:
                stack = "UNKNOWN";
                break;
        }

        switch (event.status)
        {
            case TRACE_SUCCESS:
                status = "SUCCESS";
                break;

            case TRACE_STACK_FULL:
                status = "FULL";
                break;

            case TRACE_STACK_EMPTY:
                status = "EMPTY";
                break;

            default:
                status = "UNKNOWN";
                break;
        }

        char logLine[128];

        snprintf(
            logLine,
            sizeof(logLine),
            "%s-%s-%d-%s",
            operation,
            status,
            event.value,
            stack
        );

        drawText(
            buffer,
            2,
            displayRow,
            logLine
        );
    }


}


void showPeekIndicator(int index)
{
    peekVisible = 1;
    peekIndex = index;
}

void hidePeekIndicator(void)
{
    peekVisible = 0;
    peekIndex = -1;
}

void drawStacks(
    char buffer[BUFFER_HEIGHT][BUFFER_WIDTH],
    Stack *mainStack,
    Stack *tempStack
)
{
    Rectangle stackArea = {28, 3, 16, 52};

    drawRectangle(buffer, stackArea);
    drawText(buffer, 49, 4, "STACKS");

    drawText(buffer, 38, 6, "MAIN");
    drawText(buffer, 61, 6, "TEMP");

    /*
     * Draw MAIN stack
     */
    if (mainVisualStack.visible && mainStack != NULL)
    {
        for (int i = 0; i <= mainStack->top; i++)
        {
            Rectangle element = {
                mainVisualStack.x,
                16 - (i * 2),
                2,
                mainVisualStack.width
            };

            char value[16];

            snprintf(
                value,
                sizeof(value),
                "%d",
                mainStack->data[i]
            );

            drawBoxWithText(buffer, element, value);
        }
    }



    /*
     * Draw TEMP stack
     */
    if (tempVisualStack.visible && tempStack != NULL)
    {
        for (int i = 0; i <= tempStack->top; i++)
        {
            Rectangle element = {
                tempVisualStack.x,
                16 - (i * 2),
                2,
                tempVisualStack.width
            };

            char value[16];

            snprintf(
                value,
                sizeof(value),
                "%d",
                tempStack->data[i]
            );

            drawBoxWithText(buffer, element, value);


        }
    }

     /*
     * Draw PEEK indicator
     */
    if (peekVisible &&
        mainStack != NULL &&
        peekIndex >= 0 &&
        peekIndex <= mainStack->top)
    {
        int y = 16 - (peekIndex * 2);

        drawText(buffer, 29, y + 1, "PEEK-->");
    }
}



void drawOperations(char buffer[BUFFER_HEIGHT][BUFFER_WIDTH])
{
    Rectangle box = {0, 19, 5, BUFFER_WIDTH};

    drawRectangle(buffer, box);
    drawText(buffer, 2, 20, "OPERATIONS");

    drawText(buffer, 16, 20, "[ PUSH ]");
    drawText(buffer, 27, 20, "[ POP ]");
    drawText(buffer, 37, 20, "[ PEEK ]");

    drawText(buffer, 16, 22, "[ INSERT AT INDEX ]");
    drawText(buffer, 38, 22, "[ INSERT AT BOTTOM ]");

    drawText(buffer, 60, 22, "[ REVERSE ]");
}

void drawInput(char buffer[BUFFER_HEIGHT][BUFFER_WIDTH])
{
    /*
     * Dedicated input field.
     * The inside of this box is left open so the actual user input
     * can be rendered here in real time later.
     */
    Rectangle box = {0, 26, 6, BUFFER_WIDTH};

    drawRectangle(buffer, box);
    drawText(buffer, 2, 27, "INPUT >");

    /* Temporary cursor position for the input field. */
    drawText(buffer, 11, 27, "|");
}

void drawUI(char buffer[BUFFER_HEIGHT][BUFFER_WIDTH], Stack *mainStack, Stack *tempStack, TraceList *trace)
{

    drawTitle(buffer);
    drawLogs(buffer, trace);
    drawStacks(buffer, mainStack, tempStack);
    drawOperations(buffer);
    drawInput(buffer);
}


/* ---------- Rendering ---------- */

void renderBuffer(char buffer[BUFFER_HEIGHT][BUFFER_WIDTH])
{
    printf("\033[H");

    for (int y = 0; y < BUFFER_HEIGHT; y++)
    {
        fwrite(buffer[y], sizeof(char), BUFFER_WIDTH, stdout);

        if (y < BUFFER_HEIGHT - 1)
            putchar('\n');
    }

    fflush(stdout);
}


/* ---------- Terminal lifecycle ---------- */

void visualizerInit(void)
{
    printf("\033[?25l");
    printf("\033[2J");
    printf("\033[H");

    clearBuffer(frontBuffer);
    clearBuffer(backBuffer);
}

void visualizerShutdown(void)
{
    printf("\033[?25h");
    fflush(stdout);
}
