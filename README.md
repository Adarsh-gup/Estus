# Estus

A dynamically typed, interpreted programming language written in C++.

## About

Estus is a small, expression-oriented language with clean syntax.
The interpreter is implemented as a tree-walk evaluator and can support two ways of running code: by passing a file path or running it interactively (REPL).

<!--

// drop in a real Estus snippet here once you have one

-->

## Implementation

**Lexer** - Designed to take in the raw source code as a series of characters and group them into a series of tokens.

**Parser** - A top-down recursive descent parser is used to build the abstract syntax tree by handling operator precedence and associativity (similar to the rules of the C language).

**AST Printer** - A debugging tool that prints the parsed abstract syntax tree (AST) in readable form.

**Interpreter** - Designed using visitor pattern to evaluate the AST and produce a value.
