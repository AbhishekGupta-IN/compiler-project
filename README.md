## Compiler Construction

The repository contains the compiler implementation for a custom language as a part of course work for CS F363 - Compiler Construction at BITS Pilani under instructor [**Vandana Agarwal**](https://universe.bits-pilani.ac.in/pilani/vandana/profile).

The language supports features such as 
- Static scoping of variables
- Assignment, I/O, Declarative, Iterative, Conditional, Function call statements.
- Returning multiple values from a function
- Arithmetic and boolean expressions
- Strongly typed language

The language specifications can be found [here](./Language-specifications.pdf)

### Note

**All files on this repository are for educational purpose with no intentions of promoting unfair means in any evaluative component.**

## How to run

In the Code directory, run the following commands

```
    gcc -o stage1exe driver.c parser.c lexer.c ast.c symTable.c
    ./stage1exe testcase.txt parseTree.txt
```
