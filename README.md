# kramer

A LISP interpreter implemented in C.

## Building

Requires CMake 3.10+ and a C11 compiler.

```sh
cmake -B build
cmake --build build
```

Unity (the test framework) is fetched automatically by CMake on first configure — no manual setup needed.

## Running tests

```sh
cd build
ctest --output-on-failure
```

## Project structure

```
kramer/
├── include/        # Public headers (token, lexer, parser, ast)
├── src/            # Source files
├── tests/          # Test suites (Unity)
│   ├── lexer/
│   ├── parser/
│   └── simple/
└── CMakeLists.txt
```

## Status

Early development. The lexer and parser are work in progress.
