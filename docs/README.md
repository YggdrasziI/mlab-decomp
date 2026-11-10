# Decompilation learning guide

This project is also a way to learn how game decompilation works. This guide explains
it in two parts:

- **General** (`general/`): the concepts behind any matching decompilation,
  independent of the platform.
- **Project** (`project/`): how this project was actually done: tools, steps,
  a unit from start to finish, the problems met, and the daily workflow.

Read the general part first if the words "translation unit", "relocation" or "matching"
are new; otherwise jump to the project part.

## Contents

General concepts:

1. [What decompilation is](general/01-what-is-decompilation.md)
2. [Compilers and determinism](general/02-compilers-and-determinism.md)
3. [Anatomy of a program](general/03-anatomy-of-a-program.md)
4. [The matching workflow](general/04-the-matching-workflow.md)
5. [Thinking like the compiler](general/05-thinking-like-the-compiler.md)

This project:

1. [Setup and toolchain](project/01-setup-and-toolchain.md)
2. [Steps taken](project/02-steps-taken.md)
3. [A unit from start to finish](project/03-a-unit-from-start-to-finish.md)
4. [Problems met](project/04-problems-met.md)
5. [Daily workflow](project/05-daily-workflow.md)

## Where the reference documents fit

The guide explains; the reference documents hold the facts. When the guide and a
reference document disagree, the reference document wins.

| Document | Content |
|---|---|
| [`SPEC.md`](../SPEC.md) | target binary, toolchain, acceptance criteria, naming |
| [`notes/COMPILER.md`](../notes/COMPILER.md) | compiler and SDK evidence |
| [`notes/SPLITS.md`](../notes/SPLITS.md) | region map and every unit, NonMatching gaps |
| [`notes/MATCHING.md`](../notes/MATCHING.md) | catalogue of matching techniques |
| [`notes/REFERENCES.md`](../notes/REFERENCES.md) | tools and reference projects |
| [`notes/PROVENANCE.md`](../notes/PROVENANCE.md) | code reused from other projects |
| [`notes/PROGRESS.md`](../notes/PROGRESS.md) | progress milestones |

## Glossary

| Term | Meaning |
|---|---|
| **matching** | source that compiles to exactly the original bytes |
| **fuzzy match** | similarity score of a function, ignoring some details such as register numbers; a guide, not a proof |
| **linked** | the unit's compiled object replaces the assembly in the final link |
| **translation unit (TU)**, **unit** | one source file and its includes; one object file |
| **split** | the address ranges of one unit in each section (`splits.txt`) |
| **symbol** | a named function or object; unknown ones are named after their address (`fn_800B1EDC`, `lbl_8035E038`) |
| **relocation** | a place in code or data that holds an address filled in by the linker |
| **section** | `.text` (code), `.data`, `.rodata`, `.bss` (zeroed), `.sdata`/`.sdata2`/`.sbss` (small data), `.ctors` (static constructors) |
| **DOL** | the executable format of the Wii/GameCube main program (`main.dol`) |
| **REL** | relocatable module loaded at run time (this game has none) |
| **`__sinit`** | the static initializer that mwcc generates at the end of a file |
| **vtable** | table of virtual function pointers of a class; slots named `vfXX` here |
| **RTTI** | run-time type information: class name strings, the main source of real names |
| **name mangling** | encoding of class, name and parameter types in a symbol: `__ct__11Entity_PropFiP10EntityInfoi` is the constructor of `Entity_Prop` taking `(int, EntityInfo*, int)` |
| **inline / weak function** | function defined in a header, possibly emitted in several objects; the linker keeps one |
| **mwcc** | Metrowerks CodeWarrior C/C++ compiler, the original compiler |
| **dtk** | decomp-toolkit: analysis, split, DOL rebuild and checksum |
| **objdiff** | function-by-function diff and report |
| **gate** | the checks every change must pass before a commit |
