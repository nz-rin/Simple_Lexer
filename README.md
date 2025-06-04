# SimpleLexer

A small and fast C++ header-only lexer for basic tokenization of strings and files.
Supports splitting text into numeric, alphabetic, alphanumeric, and symbol tokens.
Only works on Linux.

---

## Features

- Header-only design
- Tokenizes strings and files using `mmap`
- Customizable token rules (via `#define`)
- Small and dependency-light

---

## Customizeable Macros/Token Rules
- redefine:
- #define SINGLE_TOKENS "(){},;"
- #define WHITESPACE_TOKENS " \t\n"
- #define ALLOW_ALPHANUM

---

## Build

No build step is required — just include the header in your project:

```cpp
#define LEX_IMPL
#include "SimpleLexer.hxx"
```

---

## Example
- run build.sh to compile main.cxx.
- run main which will tokenize the test.txt file into tokens based on the customizeable #define and print it.
