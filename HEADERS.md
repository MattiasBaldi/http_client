# C Headers Explained

## What is a Header File?

A header file (`.h`) declares what functions and types exist in your program without showing how they work. It's like a contract between files.

**Analogy:** If your program is a library, the header is the catalog that tells you what books are available. The `.c` file is the actual books.

## Why Headers?

When `test_http_client.c` needs to call `parse_url()` from `main.c`, the compiler needs to know:
1. Does `parse_url()` exist?
2. What arguments does it take?
3. What does it return?

The header file answers all these questions **before linking**.

## Declaration vs Definition

**Declaration** (in header):
```c
parsed_url parse_url(char *url);  // "This function exists"
```

**Definition** (in source file):
```c
parsed_url parse_url(char *url) {  // "Here's how it actually works"
  // implementation
}
```

## Header Guards

Prevents a header from being included multiple times in the same compilation:

```c
#ifndef MAIN_H
#define MAIN_H

// Content here

#endif
```

Without guards, if header is included twice, you get "duplicate definition" errors.

## What Goes in Headers

**DO include:**
- Type definitions (structs, enums)
- Function declarations
- Constants (`#define`)
- Macro definitions

**DON'T include:**
- Function implementations (goes in `.c`)
- Large blocks of code
- Global variable definitions (use `extern` for declarations)

## Compilation Flow

```
test_http_client.c includes main.h
        ↓
Preprocessor expands: replaces #include with header contents
        ↓
Compiler sees function declarations from header
        ↓
Compiler trusts those functions exist, creates object file
        ↓
Linker finds actual function definitions in main.o
        ↓
Links them together into executable
```

## Common Pattern

**main.h:**
```c
#ifndef MAIN_H
#define MAIN_H

typedef struct { ... } MyType;
MyType do_something(char *input);

#endif
```

**main.c:**
```c
#include "main.h"

MyType do_something(char *input) {
  // implementation
}
```

**test.c:**
```c
#include "main.h"  // Now I know do_something exists

void test_something() {
  MyType result = do_something("test");
  // Test assertions
}
```

## Conditional Compilation

Headers can use preprocessor directives:

```c
#ifdef DEBUG
  void debug_print(char *msg);
#endif
```

Or in Makefile: `-DDEBUG` adds `DEBUG` define.

## Key Takeaway

Headers are the **interface** to your code. They tell other files what's available without exposing implementation details.
