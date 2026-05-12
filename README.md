# Simple Shell Project - Holberton School

## Description
This is a simple UNIX command interpreter (shell) written in C. It provides a command-line interface for users to interact with the operating system.

## Usage
The shell can be used in both interactive and non-interactive modes.

### Compilation
To compile the shell, use the following command:
```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

### Interactive Mode
Run the shell:
```bash
./hsh
```
Type a command and press Enter:
```bash
($) /bin/ls
README.md shell.c shell.h
($)
```

### Non-Interactive Mode
```bash
echo "/bin/ls" | ./hsh
```

## Features
- Displays a prompt and waits for user input.
- Executes simple commands (one word or with arguments).
- Handles the "end of file" condition (Ctrl+D).
- Handles errors.

## Authors
See the AUTHORS file for details on contributors.
