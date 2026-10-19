# 1. What decompilation is

## From source code to an executable

A program starts as source files written by people. A **compiler** turns each source
file (a *translation unit*: the file plus everything it `#include`s) into an **object
file**: machine code plus data, with a table of **symbols** (names of functions and
variables) and **relocations** (places that will hold an address once it is known).
A **linker** then puts all object files together, chooses the final address of every
function and variable, fills in the relocations, and writes the **executable**.

```
 a.cpp ──compiler──► a.o ─┐
 b.cpp ──compiler──► b.o ─┼──linker──► program executable
 libc  ─────────────────────┘
```

## What is lost, what survives

Shipping an executable throws most of the source information away:

| Lost (usually) | Survives |
|---|---|
| function and variable names | the machine code itself, instruction by instruction |
| types, struct and class definitions | data: constants, strings, tables |
| comments, formatting | the **order** in which the linker placed functions and data |
| file names and boundaries | the **shape** of the code: which compiler, which options |
| local variable names | sometimes: symbols, debug strings, assert messages, C++ type names (RTTI) |

Decompilation is the work of recovering the lost half from the surviving half.

## Disassembly, decompilation, matching decompilation

- **Disassembly** turns bytes back into assembly instructions. It is mechanical and
  exact, but hard to read and impossible to modify at scale.
- **Decompilation** produces readable source code (C, C++) that does what the binary
  does. Automatic decompilers give a draft; a human makes it readable.
- **Functional decompilation** stops there: the source behaves like the original
  (useful for ports, mods, understanding).
- **Matching decompilation** goes further: the source, compiled with the *same
  compiler and options* as the original, must produce **the same bytes**. Every
  function, every data object, in the same order, until the rebuilt executable is
  identical to the original one.

## Why matching

Matching sounds harder than it needs to be, but it gives something nothing else gives:
**proof**. A functional decompilation can hide subtle mistakes (a wrong sign, an
off-by-one, a missing side effect). A matching one cannot: if the bytes are identical,
the source is a faithful description of the program, and a checksum (SHA-1 of the
rebuilt file) says so in a fraction of a second.

It also makes the work **incremental and measurable**: each function either matches or
it does not; progress is a percentage of bytes; a mistake in one place is caught
immediately because the checksum breaks.

The price: the source must be written the way the original programmers (and their
compiler) would have produced it, not just any way that works. Most of the craft is
in that gap (see [5. Thinking like the compiler](05-thinking-like-the-compiler.md)).

## What a matching decompilation is not

- It is **not** the original source. Names are guesses unless evidence exists, file
  boundaries are reconstructed, comments are new. Two different sources can compile to
  the same bytes; the project picks a plausible one.
- It does **not** contain the game. The repository holds only new source code; the
  original executable and the game data come from the user's own copy and stay out of
  version control.

## Legal and ethical frame

The usual practice of decompilation projects, and the rules of this one:

- work from a copy you own; never distribute the original executable, assets or
  extracted data;
- write the source yourself; reuse other people's code only under a known license, and
  record where it came from;
- keep the goal clear (preservation, study, learning).

Next: [2. Compilers and determinism](02-compilers-and-determinism.md).
