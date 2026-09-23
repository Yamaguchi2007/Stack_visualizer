#ifndef INPUT_H
#define INPUT_H

#define INPUT_MAX_LENGTH 64

void inputInit(void);

void inputHandleKey(int c);

const char *inputGetBuffer(void);

const char *inputGetSubmittedCommand(void);

int inputHasSubmittedCommand(void);

void inputClearSubmittedCommand(void);

#endif
