## Minishell
Created by: Sheree Morphett, Jenny Boucher

The Minishell project is a part of the 42-cursus group project, where we successfully built a simple shell program in C. The shell acts as a command-line interpreter, allowing users to execute commands, handle environment variables, perform input/output redirection, and manage pipelines. It also supports built-in commands like `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`.

### Project Overview
The Minishell project aims to create a basic shell similar to the popular Unix shell, bash. It provides a command prompt, reads user input, and executes commands with the correct output. The shell handles various scenarios, including input/output redirection, pipeline execution, and environment variable expansion.

### Features Implemented
- **Command Execution**: The Minishell is capable of searching and launching the appropriate executable file based on the PATH variable. Users can execute commands by providing an absolute or relative path.

- **Input/Output Redirection**: The shell supports redirection of input and output using the `<`, `>`, `>>` symbols. It allows users to redirect the standard input and output of commands to or from files.

- **Here-Documents**: The `<<` symbol allows users to specify a delimiter and read input until a line containing the delimiter is encountered. However, this input is not added to the command history.

- **Pipelines**: The Minishell implements pipes (`|`) to create a sequence of commands where the output of one command is connected to the input of the next command.

- **Environment Variable Expansion**: Users can use `$` followed by a sequence of characters to expand environment variables to their values.

- **Built-in Commands**: The shell supports various built-in commands, including:
   - `echo`: Displays text with an option to omit the trailing newline (`-n`).
   - `cd`: Changes the current directory to a specified relative or absolute path.
   - `pwd`: Displays the current working directory.
   - `export`: Sets environment variables.
   - `unset`: Unsets environment variables.
   - `env`: Displays the environment variables.
   - `exit`: Exits the shell.

- **Signal Handling**: The shell properly handles signals like `SIGINT` (Ctrl+C), `SIGQUIT` (Ctrl+\), and `SIGTERM`.

### Project Execution
When the Minishell is executed, it displays a prompt (e.g., `$` or `minishell$`) and waits for the user to input a command. Upon receiving a command, it parses the input, identifies built-in commands, and executes them if applicable. If the command is not built-in, it searches for the corresponding executable in the system's PATH or specified absolute/relative path and executes it.

The shell maintains a history of executed commands, allowing users to access previously entered commands using the arrow keys.

### Conclusion
The Minishell project provides a practical learning experience in C programming, file I/O, process management, signal handling, and string manipulation. It allows us to gain a better understanding of how shells work and how they execute commands in a Unix-like environment. By completing this project, we strengthen our programming skills and improve our ability to collaborate effectively in a team.
