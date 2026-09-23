#include "input.h"

#include <string.h>
#include <ctype.h>

static char inputBuffer[INPUT_MAX_LENGTH];
static char submittedCommand[INPUT_MAX_LENGTH];

static int inputLength = 0;
static int commandSubmitted = 0;


static void normalizeCommand(char *command)
{
    for (int i = 0; command[i] != '\0'; i++)
    {
        command[i] = (char)tolower((unsigned char)command[i]);
    }
}


void inputInit(void)
{
    inputBuffer[0] = '\0';
    submittedCommand[0] = '\0';

    inputLength = 0;
    commandSubmitted = 0;
}


void inputHandleKey(int c)
{
    /* ENTER */
    if (c == '\n' || c == '\r')
    {
        if (inputLength > 0)
        {
            /* Copy what the user typed. */
            strcpy(submittedCommand, inputBuffer);

            /* Convert PUSH / Push / pUsH etc. to lowercase. */
            normalizeCommand(submittedCommand);

            commandSubmitted = 1;

            /* Clear the typing buffer. */
            inputLength = 0;
            inputBuffer[0] = '\0';
        }

        return;
    }

    /* BACKSPACE */
    if (c == 127 || c == '\b')
    {
        if (inputLength > 0)
        {
            inputLength--;
            inputBuffer[inputLength] = '\0';
        }

        return;
    }

    /* NORMAL PRINTABLE CHARACTER */
    if (c >= 32 && c <= 126)
    {
        if (inputLength < INPUT_MAX_LENGTH - 1)
        {
            inputBuffer[inputLength] = (char)c;
            inputLength++;

            inputBuffer[inputLength] = '\0';
        }
    }
}


const char *inputGetBuffer(void)
{
    return inputBuffer;
}


const char *inputGetSubmittedCommand(void)
{
    return submittedCommand;
}


int inputHasSubmittedCommand(void)
{
    return commandSubmitted;
}


void inputClearSubmittedCommand(void)
{
    submittedCommand[0] = '\0';
    commandSubmitted = 0;
}
