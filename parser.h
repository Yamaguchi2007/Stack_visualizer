#ifndef PARSER_H
#define PARSER_H

typedef enum
{
    COMMAND_INVALID,
    COMMAND_PUSH,
    COMMAND_POP,
    COMMAND_PEEK,
    COMMAND_IAB,
    COMMAND_IAI,
    COMMAND_REVERSE
} CommandType;

typedef struct
{
    CommandType type;

    int hasValue;
    int value;
    int index;

} ParsedCommand;


/*
 * Parse a submitted command.
 *
 * Returns 1 if the command is valid.
 * Returns 0 if the command is invalid.
 */
int parseCommand(const char *input, ParsedCommand *command);

#endif
