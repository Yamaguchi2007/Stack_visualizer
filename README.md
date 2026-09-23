# Stack Visualizer

A terminal-based Stack Visualizer written in C using only the standard C library.

The project demonstrates stack operations and several stack algorithms through a simple terminal interface. It uses double buffering to render the interface without relying on external terminal UI libraries such as ncurses.

## Platform: Linux / Unix-like systems
## Language: C
## Terminal APIs: POSIX (termios, unistd)

## Features

- PUSH, POP, and PEEK operations
- Insert a value at the bottom of the stack
- Insert a value at a specified index
- Reverse the stack
- Main stack and TEMP stack visualization
- Operation logs and trace recording
- Stack-full and stack-empty handling
- Double-buffered terminal rendering
- No ncurses or other external UI libraries
- Stack capacity of 7 elements

## Commands

### PUSH
Adds a value to the top of the main stack.

```text
push 10
```

### POP
Removes the value from the top of the main stack.

```text
pop
```

If the stack is empty, the operation reports `STACK EMPTY`.

### PEEK
Displays the value at the top of the main stack without removing it.

```text
peek
```

### Insert At Bottom
Inserts a value at the bottom of the main stack.

```text
iab 99
```

`iab` stands for *Insert At Bottom*.

### Insert At Index
Inserts a value at a specified index.

```text
iai 1 99
```

The index is counted from the top of the stack, starting at `0`.

### Reverse
Reverses the order of the main stack.

```text
reverse
```

## Project Structure

### `main.c`
The main program and application controller. It initializes the program, creates the stacks, handles the input loop, passes commands to algorithms, updates the visualizer, handles terminal raw mode, and performs cleanup.

### `stack.c`
Contains the basic stack implementation: `stack_init()`, `destroy_stack()`, `push()`, `pop()`, `peek()`, `isEmpty()`, `isFull()`, and `size()`.

### `stack.h`
Defines the `Stack` structure and declarations for stack functions.

### `algos.c`
Contains higher-level stack algorithms: `insertAtBottom()`, `insertAtIndex()`, and `reverseStack()`.

### `algos.h`
Contains declarations for the stack algorithms.

### `input.c`
Handles terminal input, input buffering, command normalization, special keys, and submitted commands.

### `input.h`
Header file for the input system.

### `parser.c`
Converts submitted commands into structured commands containing command type, value, index, and value-presence information.

### `parser.h`
Contains parser structures and declarations.

### `trace.c`
Records stack operations such as PUSH, POP, and PEEK, along with the stack involved and operation status.

### `trace.h`
Contains trace structures, enums, and declarations.

### `vis2_stack_elements.c`
Contains the terminal visualizer, drawing functions, screen buffers, stack display, logs, operations, input area, rendering, and visualizer initialization/shutdown.

### `vis2_stack_elements.h`
Contains visualizer constants, structures, and function declarations.

## How The Program Works

The general flow is:

```text
User Input
    ↓
input.c
    ↓
parser.c
    ↓
main.c
    ↓
algos.c / stack.c
    ↓
trace.c
    ↓
vis2_stack_elements.c
    ↓
Terminal Output
```

Basic stack operations are handled by `stack.c`. Higher-level operations such as *Insert At Bottom*, *Insert At Index*, and *Reverse* are implemented in `algos.c`.

The parser separates command interpretation from stack logic. The visualizer is responsible for displaying the current state.

## Main Stack and TEMP Stack

The program maintains two stacks:

- **MAIN** — the primary stack operated on by the user.
- **TEMP** — a temporary working stack used by some algorithms.

The TEMP stack is visible in the interface so that intermediate stack operations can be observed.

## Trace and Logging

The trace system provides a record of stack operations. It records operations such as:

- PUSH
- POP
- PEEK

It also records the stack involved and the operation status.

Possible statuses include:

- `SUCCESS`
- `STACK FULL`
- `STACK EMPTY`

## Double Buffering

The visualizer uses two character buffers:

- Front buffer
- Back buffer

Drawing operations are performed on the back buffer. Once the frame is ready, the buffers are swapped and the rendered buffer is displayed.

This helps reduce terminal flickering.

## Compilation

```bash
gcc -Wall -Wextra -std=c11 main.c vis2_stack_elements.c input.c parser.c stack.c algos.c trace.c
```

This produces the default executable:

```text
a.out
```

## Running

```bash
./a.out
```

## Example Session

```text
push 10
push 20
push 30
peek
pop
iab 99
iai 1 50
reverse
```

## Notes

- The stack capacity is currently **7 elements**.
- Indexes for `iai` are counted from the top of the stack, starting at `0`.
- The project uses only the standard C library and terminal functionality available through the source files.
- Animation is not required for the completed implementation.

## Purpose

This project demonstrates how stack data structures and stack-based algorithms can be implemented in C while providing a visual representation of their behavior.

The project is separated into modules for stack management, command parsing, input handling, algorithms, tracing, and visualization.
