# 3. Anatomy of a program

To decompile a binary you need to read it: where the code and data are, how functions
call each other, and what C++ leaves behind.

## Sections

An executable (and each object file) is divided into **sections** by kind of content:

| Section (typical names) | Content |
|---|---|
| `.text` | machine code |
| `.data` | initialised, writable data (tables, vtables, global variables with a value) |
| `.rodata` | read-only data (constant tables, strings) |
| `.bss` | zero-initialised data: only its size is stored, the loader clears it |
| `.ctors` / `.init_array` | list of functions to run before `main` (static constructors) |
| small-data sections (`.sdata`, `.sbss`, `.sdata2`...) | on some architectures, small objects reached through a dedicated base register for shorter code |
| exception tables | data used by C++ exception handling |

Each object file contributes a piece of each section; the linker concatenates the
pieces **in link order**. That is why the layout of the final file reveals where one
source file ended and the next one began.

## Symbols and relocations

Inside an object file, a call to `foo` is not yet an address: it is a **relocation**
"put the address of `foo` here". The linker resolves it. In a stripped executable the
names are gone, but the pattern remains: every place that holds an address is a
reference from one piece of code or data to another. Following those references
("cross-references") is the main way to understand who uses what.

Addresses are often built in two halves (a high part and a low part), so a reference
may span two instructions; tools that understand the instruction set rebuild them.

## Calling conventions

Every platform has an **ABI** (application binary interface) that fixes how functions
talk to each other. The details change between architectures, the ideas do not:

- **arguments** go in given registers (first argument in register A, second in B...),
  extra ones on the stack;
- the **return value** comes back in a given register (one for integers and pointers,
  one for floats);
- registers are split into **volatile** ones (a called function may destroy them) and
  **saved** ones (a function that uses them must restore them);
- each function has a **prologue** (make a stack frame, save the return address and
  the saved registers it needs) and an **epilogue** (restore and return).

A hand-written, generic example (not taken from any game):

```
  stack_alloc 0x10          ; prologue: make a 16-byte frame
  save        ret_addr
  save        s0            ; s0 is a saved register: keep the caller's value
  move        s0, a0        ; keep the first argument across the call
  call        helper
  add         a0, s0, 1     ; use it after the call
  restore     s0
  restore     ret_addr
  stack_free  0x10
  return
```

Reading a function starts here: which arguments are used, which values must survive
calls (they go to saved registers), what is returned.

## What C++ leaves behind

- **Name mangling**: C++ encodes the class, the function name and the parameter types
  into the symbol name, for example `__ct__6EntityFv` (a constructor of `Entity` taking
  no arguments) in one compiler's scheme. When the original names are known, the
  mangled name also gives the parameter types.
- **Virtual tables (vtables)**: a class with virtual functions has a table of function
  pointers in the data section; each object starts with (or contains) a pointer to it.
  A vtable lists the class's virtual functions in declaration order, inherited ones
  included, so comparing vtables shows the class hierarchy.
- **RTTI** (run-time type information): when enabled, each polymorphic class gets a
  type descriptor with its **name as a string** and its base classes. It is one of the
  richest sources of real names.
- **Static initialisers**: global objects with constructors are built by a generated
  function listed in `.ctors`. There is usually one per source file, at the end of that
  file's code: a natural marker of a file boundary.
- **Inline and weak functions**: functions defined in headers (small accessors,
  implicit destructors, template code) may be compiled into every file that uses them;
  the linker keeps one copy. Where that copy ends up tells which file used it first.
- **Templates**: the same template code instantiated for several types appears
  several times, once per type.

Next: [4. The matching workflow](04-the-matching-workflow.md).
