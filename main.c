#include "vis2_stack_elements.h"
#include "input.h"
#include "parser.h"
#include "stack.h"
#include "trace.h"
#include "algos.h"

#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

static struct termios originalTerminal;

static char lastSubmitted[INPUT_MAX_LENGTH] = "";

static void enableRawMode(void)
{
    tcgetattr(STDIN_FILENO, &originalTerminal);

    struct termios raw = originalTerminal;

    raw.c_lflag &= ~(ICANON | ECHO);

    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

static void disableRawMode(void)
{
    tcsetattr(
        STDIN_FILENO,
        TCSAFLUSH,
        &originalTerminal
    );
}

int main(void)
{
    inputInit();

    enableRawMode();

    visualizerInit();


    Stack *mainStack = stack_init(7);
    Stack *tempStack = stack_init(7);

    TraceList trace;
    initTrace(&trace);

    while (1)
    {
        char (*buffer)[BUFFER_WIDTH] = getBackBuffer();

        clearBuffer(buffer);

        drawUI(buffer, mainStack, tempStack, &trace);

        const char *currentInput = inputGetBuffer();

        drawText(
            buffer,
            11,
            27,
            currentInput
        );

        int inputLength = (int)strlen(currentInput);

        if (11 + inputLength < BUFFER_WIDTH)
        {
            drawText(
                buffer,
                11 + inputLength,
                27,
                "|"
            );
        }

        if (lastSubmitted[0] != '\0')
        {
            drawText(
                buffer,
                2,
                29,
                "LAST COMMAND:"
            );

            drawText(
                buffer,
                16,
                29,
                lastSubmitted
            );
        }

        renderBuffer(buffer);

        swapBuffers();

        char c;

        if (read(STDIN_FILENO, &c, 1) <= 0)
        {
            break;
        }

        if (c == 27)
        {
            break;
        }
        else
        {
            inputHandleKey(c);

            if (inputHasSubmittedCommand())
            {
                const char *command =
                    inputGetSubmittedCommand();

                ParsedCommand parsedCommand;

                if (parseCommand(
                        command,
                        &parsedCommand))
                {
                    hidePeekIndicator();

                    if (parsedCommand.type == COMMAND_PUSH)
                    {
                        if (algoPush(
                            mainStack,
                            parsedCommand.value,
                            &trace,
                            STACK_MAIN))
                        {
                            snprintf(
                                lastSubmitted,
                                INPUT_MAX_LENGTH,
                                "TOP: %d, VALUE: %d",
                                mainStack->top,
                                mainStack->data[mainStack->top]
                            );
                        }
                        else
                        {
                            snprintf(
                                lastSubmitted,
                                INPUT_MAX_LENGTH,
                                "STACK FULL"
                            );
                        }
                    }

                    else if (parsedCommand.type == COMMAND_POP)
                    {
                        int poppedValue;

                        if (algoPop(
                                mainStack,
                                &poppedValue,
                                &trace,
                                STACK_MAIN))
                        {
                            hidePeekIndicator();

                            snprintf(
                                lastSubmitted,
                                INPUT_MAX_LENGTH,
                                "POP: %d",
                                poppedValue
                            );
                        }
                        else
                        {
                            hidePeekIndicator();

                            snprintf(
                                lastSubmitted,
                                INPUT_MAX_LENGTH,
                                "STACK EMPTY"
                            );
                        }
                    }

                    else if (parsedCommand.type == COMMAND_PEEK)
                    {
                        int peekedValue;

                        if (algoPeek(
                                mainStack,
                                &peekedValue,
                                &trace,
                                STACK_MAIN))
                        {
                            showPeekIndicator(mainStack->top);

                            snprintf(
                                lastSubmitted,
                                INPUT_MAX_LENGTH,
                                "PEEK: %d",
                                peekedValue
                            );
                        }
                        else
                        {
                            hidePeekIndicator();

                            snprintf(
                                lastSubmitted,
                                INPUT_MAX_LENGTH,
                                "STACK EMPTY"
                            );
                        }
                    }


                    // IAB
                    else if (parsedCommand.type == COMMAND_IAB)
                    {
                        int value = parsedCommand.value;

                        if (insertAtBottom(mainStack, tempStack, value, &trace))
                        {
                            snprintf(
                                lastSubmitted,
                                INPUT_MAX_LENGTH,
                                "IAB: %d",
                                value
                            );
                        }
                        else
                        {
                            snprintf(
                                lastSubmitted,
                                INPUT_MAX_LENGTH,
                                "IAB FAILED"
                            );
                        }
                    }

                    // IAI
                    else if (parsedCommand.type == COMMAND_IAI)
                    {
                        if (insertAtIndex(
                                mainStack,
                                tempStack,
                                parsedCommand.index,
                                parsedCommand.value,
                                &trace))
                        {
                            snprintf(
                                lastSubmitted,
                                INPUT_MAX_LENGTH,
                                "IAI: INDEX %d, VALUE %d",
                                parsedCommand.index,
                                parsedCommand.value
                            );
                        }
                        else
                        {
                            snprintf(
                                lastSubmitted,
                                INPUT_MAX_LENGTH,
                                "IAI FAILED"
                            );
                        }
                    }


                    // REVERSE
					else if (parsedCommand.type == COMMAND_REVERSE)
					{
						if (reverseStack(mainStack, tempStack, &trace))
						{
							snprintf(
								lastSubmitted,
								INPUT_MAX_LENGTH,
								"REVERSE"
							);
						}
						else
						{
							snprintf(
								lastSubmitted,
								INPUT_MAX_LENGTH,
								"REVERSE FAILED"
							);
						}
					}

                    else
                    {
                        strncpy(
                            lastSubmitted,
                            "INVALID COMMAND",
                            INPUT_MAX_LENGTH - 1
                        );

                        lastSubmitted[
                            INPUT_MAX_LENGTH - 1
                        ] = '\0';
                    }

                    inputClearSubmittedCommand();
                }
            }
        }
    }

    disableRawMode();

    visualizerShutdown();

    return 0;

}
