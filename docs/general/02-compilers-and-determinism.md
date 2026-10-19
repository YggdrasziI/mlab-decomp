# 2. Compilers and determinism

Matching decompilation rests on one fact: **a compiler is deterministic**. Given the
same source, the same compiler version and the same options, it produces the same
bytes every time. Change any of the three and the output may change.

## The three inputs

1. **The compiler and its exact version.** Two versions of the same compiler often
   differ in small ways: one removes a dead store the other keeps, one uses a jump
   table from 7 `case` labels and another only from 8. Those differences are how a
   version is identified, and why a wrong version can make a function impossible to
   match.
2. **The options (flags).** Optimisation level, inlining policy, floating-point
   contraction (fusing `a * b + c` into one instruction), alignment, language
   features (exceptions, RTTI). One wrong flag can change every function.
3. **The source.** Within the right compiler and flags, the source decides the
   rest, including things that look irrelevant (see below).

## What a source change moves

An optimising compiler has freedom. The source steers the choices it makes:

- **Register allocation**: which variable lives in which register. Declaring
  variables in another order, splitting one variable in two, or using a temporary
  can renumber registers without changing anything else.
- **Instruction scheduling**: the order of independent instructions. It follows the
  order of the source expressions more than one would expect.
- **Inlining**: a small function may be copied into its caller or called; whether it
  is depends on its size, where it is defined and the inlining flags.
- **Constant pools**: floating-point and string constants are stored in pools, often
  in the order the compiler first meets them while reading the file.
- **Stack layout**: local variables and compiler temporaries each get a slot; their
  order depends on declaration order and on how expressions create temporaries.
- **Control-flow shape**: `if/else`, early `return`, `continue`, ternaries, `for`
  versus `while` can all produce different branches for the same logic.
- **Emission order**: functions usually come out in source order, but inline and
  template functions may be emitted where they are first used.

So two pieces of source that are *equivalent* for a human may compile to different
bytes. The goal is to find the one that gives the original bytes.

## Identifying the compiler

You cannot match anything before you know the compiler. The usual evidence:

- **Strings** in the binary: library version banners, build dates, copyright lines
  of the compiler's runtime library.
- **Runtime library code**: every compiler ships its own startup code and C library;
  recognising it names the compiler family.
- **Code idioms**: the way prologues save registers, how switch statements are
  compiled, which instructions are used for comparisons. Each family and version has
  habits.
- **Experiments**: rewrite a few small functions in C, compile them with every
  candidate version and flag set, and compare the bytes. The candidates that match
  every probe survive. A function that only one candidate reproduces settles the
  question; until then, the choice is a hypothesis and must be called one.

A binary can mix compiler versions: libraries are often built separately from the
game, and each part may need its own compiler.

## Fuzzy scores versus exact bytes

Diff tools usually give a **fuzzy match percentage**: how similar the compiled
function is to the original, ignoring some details (for example register numbers or
addresses that the linker will fill in). It is a good guide while iterating, but only
**exact bytes** count in the end. A function can show a high fuzzy score and still
differ in register numbers; only the final checksum of the linked executable proves
the match.

Next: [3. Anatomy of a program](03-anatomy-of-a-program.md).
