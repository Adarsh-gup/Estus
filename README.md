# Estus

An interpreted programming language written in C++17.

## About

Estus is a small language that currently supports global variables and expression evaluation.
The interpreter is implemented as a tree-walk evaluator and can support two ways of running code: by passing a file path or running it interactively (REPL).

A code snippet of Estus is provided below:
```
let a = 10;
let b = 20;
print a + b;          // Output: 30

let name = "Estus";
print "Hello " + name; // Output: Hello Estus

let uninitialized;
print uninitialized;  // Output: nil

```

## Implementation

**Lexer** - Designed to take in raw source code as a stream of characters and group them into tokens.

**Parser** - A top-down recursive descent parser is used to build the abstract syntax tree by handling operator precedence and associativity (similar to the rules of the C language).

**AST Printer** - A debugging tool that prints the parsed abstract syntax tree (AST) in readable form.

**Interpreter** - Designed using the visitor pattern to evaluate the AST and produce a value.

## How to Build & Run Estus

### 1. Clone the Repository
```bash
git clone https://github.com/Adarsh-gup/Estus.git
cd Estus
```

### 2. Compile the Interpreter

#### On Linux / macOS (Terminal):
```bash
g++ -std=c++17 Estus.cpp Scanner/Scanner.cpp Parser/Parser.cpp Interpreter/interpreter.cpp Token/Token.cpp -o estus
```

#### On Windows (PowerShell / Command Prompt):
```powershell
g++ -std=c++17 Estus.cpp Scanner/Scanner.cpp Parser/Parser.cpp Interpreter/interpreter.cpp Token/Token.cpp -o estus.exe
```

---
## Usage Modes

### Mode A: Interactive REPL Mode
Run the executable without any arguments to enter the interactive REPL:

```bash
# Linux / macOS
./estus

# Windows
.\estus.exe
```
### Mode B: Executing a Script File
Create a `.estus` file (e.g. `script.estus`):

Run the script file by passing its path as an argument:

```bash
# Linux / macOS
./estus script.estus

# Windows
.\estus.exe script.estus
```
