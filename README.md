# Minishell
### Our own little Bash

⠀ ⠀⠀ ⠀⠀              🤠<br/>
　　　　　💯💯💯<br/>
　　　　💯 　💯　💯<br/>
　　　👇🏽　  💯💯　👇🏽<br/>
　　　　　💯　  💯<br/>
　　　　　💯　　💯<br/>
　　　　　 👢　　👢 <br/>

Minishell is a simple shell implementation that mimics the behavior of the GNU Bash shell. It supports built-in commands, redirections, pipes, signal handling, and an interactive command-line interface.

---

## 📖 Table of Contents

- [Features](#features)
- [Built-in Commands](#built-in-commands)
- [Redirections](#redirections)
- [Pipes](#pipes)
- [Signal Handling](#signal-handling)
- [Environment Variables](#environment-variables)
- [Installation](#installation)
- [Authors](#authors)

---

## 🚀 Features

✔️ Display a **prompt** when waiting for a new command 🖥️\
✔️ Maintain a working **history** of commands\
✔️ Search and launch the correct executable (via `PATH`, relative, or absolute path)\
✔️ Avoid using more than one global variable for signal handling\
✔️ Correctly handle unclosed quotes and special characters\
✔️ Handle:

- **Single quotes (`'`)**: Prevents meta-character interpretation
- **Double quotes (`"`)**: Prevents meta-character interpretation except for `$`

✔️ Implement **redirections**:
- `<` Redirect input
- `>` Redirect output
- `<<` Read input until a specified delimiter appears (without history update)
- `>>` Append output to a file

✔️ Implement **pipes (`|`)** for command chaining\
✔️ Support **environment variables (`$VAR`)** and **exit status (`$?`)**\
✔️ Handle **signals**:
- `Ctrl+C`: Displays a new prompt on a new line
- `Ctrl+D`: Exits the shell
- `Ctrl+\`: Does nothing

✔️ Implement the following built-in commands:
- `echo` (with `-n` option)
- `cd` (supports relative/absolute paths)
- `pwd`
- `export`
- `unset`
- `env`
- `exit`

✔️ **Memory-leak free** 🔥


---

## 🔧 Built-in Commands

| Command   | Description |
|-----------|-------------|
| `echo`    | Prints text to the terminal. Supports the `-n` option (no newline). |
| `cd`      | Changes the current directory (supports relative and absolute paths). |
| `pwd`     | Prints the current working directory. |
| `export`  | Sets environment variables. |
| `unset`   | Unsets environment variables. |
| `env`     | Displays the environment variables. |
| `exit`    | Exits the shell. |

---

## 🔄 Redirections

| Symbol | Description |
|--------|-------------|
| `<`    | Redirects input from a file. |
| `>`    | Redirects output to a file. |
| `<<`   | Reads input until a line containing a specified delimiter appears. |
| `>>`   | Redirects output to a file (append mode). |

---

## 🔗 Pipes 

| Symbol | Description |  
|--------|-------------|  
|`\|`    | Pipes the output of one command into the input of another. |  


---

## ⚡ Signal Handling

| Keybinding | Behavior |
|------------|------------|
| `Ctrl+C`   | Displays a new prompt on a new line. |
| `Ctrl+D`   | Exits the shell. |
| `Ctrl+\`  | Does nothing. |

---

## 🌍 Environment Variables

| Variable | Description |
|----------|-------------|
| `$VAR`   | Expands to the value of the specified environment variable. |
| `$?`     | Expands to the exit status of the last executed command. |

---

## 📥 Installation

To install and run **Minishell**, follow these steps:

1. **Clone the repository:**
   ```sh
   git clone
   ```

2. **Navigate to the project directory:**
   ```sh
   cd minishell
   ```

3. **Build the project:**
   ```sh
   make
   ```

4. **Run Minishell:**
   ```sh
   ./minishell
   ```

---

## 📚 Resources
[GNU Bash manual](https://www.gnu.org/savannah-checkouts/gnu/bash/manual/) <br/>
readline: libreadline-dev 

## 👨‍💻 Authors

- [Mariana Pereira](https://github.com/mfaria-p)
- [Eduardo Corona](https://github.com/Corona040)
