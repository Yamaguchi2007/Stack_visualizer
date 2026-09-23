#include "parser.h"

#include <string.h>
#include <stdlib.h>

int parseCommand(const char *input, ParsedCommand *command)
{
    if (input == NULL || command == NULL)
        return 0;


    /*
     * Default state.
     */
    command->type = COMMAND_INVALID;
    command->hasValue = 0;
    command->value = 0;


    /*
     * PUSH
     *
     * Valid:
     *
     *     push
     *     push 50
     *     push -10
     *
     * "push" without a value defaults to 0.
     */
    if (strncmp(input, "push", 4) == 0)
    {
        /*
         * Exactly "push".
         *
         * Default value = 0.
         */
        if (input[4] == '\0')
        {
            command->type = COMMAND_PUSH;
            command->hasValue = 1;
            command->value = 0;

            return 1;
        }


        /*
         * "push" must be followed by whitespace
         * before an argument.
         */
        if (input[4] != ' ' && input[4] != '\t')
            return 0;


        /*
         * Move past "push".
         */
        const char *valueString = input + 4;


        /*
         * Skip whitespace.
         */
        while (*valueString == ' ' || *valueString == '\t')
            valueString++;


        /*
         * If there is nothing after "push",
         * default to 0.
         */
        if (*valueString == '\0')
        {
            command->type = COMMAND_PUSH;
            command->hasValue = 1;
            command->value = 0;

            return 1;
        }


        /*
         * Convert the argument to an integer.
         */
        char *end;

        long value = strtol(
            valueString,
            &end,
            10
        );


        /*
         * The entire argument must be a valid
         * integer.
         */
        if (*end != '\0')
            return 0;


        command->type = COMMAND_PUSH;
        command->hasValue = 1;
        command->value = (int)value;

        return 1;
    }


    /*
     * POP
     */
    if (strcmp(input, "pop") == 0)
    {
        command->type = COMMAND_POP;
        return 1;
    }


    /*
     * PEEK
     */
    if (strcmp(input, "peek") == 0)
    {
        command->type = COMMAND_PEEK;
        return 1;
    }

    // IAB
    if (strncmp(input, "iab", 3) == 0 ||
        strncmp(input, "insert at bottom", 16) == 0)
    {
        const char *valueString;

        if (strncmp(input, "iab", 3) == 0)
            valueString = input + 3;
        else
            valueString = input + 16;

        while (*valueString == ' ' || *valueString == '\t')
            valueString++;

        if (*valueString == '\0')
            return 0;

        char *end;
        long value = strtol(valueString, &end, 10);

        if (*end != '\0')
            return 0;

        command->type = COMMAND_IAB;
        command->hasValue = 1;
        command->value = (int)value;

        return 1;
    }

    // IAI
    if (strncmp(input, "iai", 3) == 0 ||
        strncmp(input, "insert at index", 15) == 0)
    {
        const char *valueString;

        if (strncmp(input, "iai", 3) == 0)
            valueString = input + 3;
        else
            valueString = input + 15;

        while (*valueString == ' ' || *valueString == '\t')
            valueString++;

        char *end;

        long index = strtol(valueString, &end, 10);

        if (*end != ' ' && *end != '\t')
            return 0;

        while (*end == ' ' || *end == '\t')
            end++;

        if (*end == '\0')
            return 0;

        long value = strtol(end, &end, 10);

        if (*end != '\0')
            return 0;

        command->type = COMMAND_IAI;
        command->hasValue = 1;
        command->index = (int)index;
        command->value = (int)value;

        return 1;
    }

    // REVERSE
    if (strcmp(input, "rev") == 0 ||
        strcmp(input, "reverse") == 0)
    {
        command->type = COMMAND_REVERSE;
        return 1;
    }

    /*
     * Anything else is invalid.
     */
    return 0;
}
