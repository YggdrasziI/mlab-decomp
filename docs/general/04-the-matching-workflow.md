# 4. The matching workflow

Every matching decompilation follows the same pipeline, whatever the platform. The
key idea: **the project always builds a byte-identical executable**, from day one.
At the start the whole program is assembly extracted from the original; over time,
pieces of assembly are replaced by C/C++ that compiles to the same bytes.

## The pipeline

```
 original executable
        │  analyse: find functions, data, references
        ▼
 symbols + split map ──► disassembled units (.s) ──┐
                                                   ├─► link ─► rebuilt executable
 decompiled units (.c/.cpp) ──compile──► .o ───────┘              │
        ▲                                                         ▼
        └────────── diff each function against the original ◄── checksum OK?
```

### Step 1: a building project, all assembly

- Get the original executable from your own copy.
- Analyse it: find every function and data object, follow references, recognise
  known library code.
- Generate one assembly file per region and a build that assembles and links them.
- Check that the result has the **same checksum** as the original. From now on every
  change must keep it.

### Step 2: identify the toolchain

Find the compiler, its version and the flags (see
[2. Compilers and determinism](02-compilers-and-determinism.md)). Without them nothing
will match.

### Step 3: split into translation units

Cut the code and each data section into **units**, one per original source file, each
with its ranges in `.text`, `.data`, `.bss` and so on. Evidence for boundaries: static
initialisers at the end of a file, data used by only one group of functions, alignment
padding, the known layout of libraries. A boundary is only a hypothesis until the unit
is decompiled and matches.

### Step 4: decompile unit by unit

For each unit: write the source of every function and data object, compile it, compare
it with the original, iterate. Small units and leaves of the call tree (functions that
call nothing undecompiled) first: they are easier and give types to the rest.

### Step 5: mark and link

A unit whose functions and data all match is marked **matching** and its compiled
object replaces the assembly in the link. If the checksum still holds, it is proven.
A unit that is close but not exact stays **non-matching**: it is compiled for
comparison, but the assembly is still linked, so the checksum stays green. The
remaining gap is written down.

### Step 6: repeat, name, document

Names and types improve as more code is understood. Progress is measured as matched
bytes and functions.

## Tool categories

| Category | Role | Example of use |
|---|---|---|
| Disassembler / splitter | turns the executable into per-unit assembly, keeps the symbol and split lists, checks the final checksum | step 1 and 3, every build |
| Compiler (original version) | compiles your source exactly as the original was compiled | every build |
| Build system | rebuilds only what changed, links, runs the check | every build |
| Diff tool | compares your compiled function with the original, instruction by instruction, with a match score | step 4, all the time |
| Interactive analysis tool (Ghidra-type disassembler/decompiler) | browse the program, rename things, follow cross-references, read a generated pseudo-C draft | understanding unknown code; its pseudo-C is never matching output, only a draft |
| Automatic decompiler for one function | produces a first C version of a function from its assembly | starting point for a long function |
| Permuter / brute force | tries many small random source changes and keeps the ones that raise the score | last few percent of a stubborn function |
| Shared scratch sites | compile one function online with many compiler versions, share it with others | asking for help, testing compiler versions |
| Emulator with debugger | watch the program run, read memory, set breakpoints | understanding what a value means |
| Version control and CI | one change per commit, the checksum checked on every push | the whole time |

No project needs all of them. The minimum is: splitter, compiler, build system, diff
tool, version control.

## Rules that keep a project healthy

- **The checksum is never broken** on the main branch.
- **Never claim a match from a score**: only the linked checksum proves it.
- **Evidence for every name and fact**; otherwise call it a hypothesis.
- **One change, one subject, one commit**, so any regression is easy to find.
- **Write down what you learn** (techniques, dead ends, open gaps): the same compiler
  habits come back again and again.

Next: [5. Thinking like the compiler](05-thinking-like-the-compiler.md).
