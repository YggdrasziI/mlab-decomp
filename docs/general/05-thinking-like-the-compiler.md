# 5. Thinking like the compiler

Decompiling one function to a byte match is an iterative loop. This page describes
the loop and the general habits of optimising compilers that the loop keeps meeting.

## The loop

1. **Read the assembly.** Find the arguments, the return value, the calls, the loops
   and branches, the memory accesses (`base + offset` gives struct fields).
2. **Infer the types.** A load of 4 bytes at offset 0x14 of the first argument is a
   field of a struct; a float load means a float field; a call through
   `object->table[n]` is a virtual call. Write minimal struct definitions with
   padding for unknown parts, and reuse the ones other functions already established.
3. **Write a first version** in plain, natural C/C++. Do not try to be clever yet.
4. **Compile and diff.** Look at the first difference, not the score.
5. **Change one thing**, rebuild, compare again. Keep what improves, undo what does
   not.
6. **Stop when it matches, or when you stop learning.** Time-box: after several
   families of attempts without progress, write down the exact remaining difference
   and move on. Later knowledge often solves it in minutes.

When many variants must be tried, script them: write each variant, build, read the
score or the byte difference, restore the best one.

## What the differences usually mean

| Symptom in the diff | Things to try |
|---|---|
| same instructions, different register numbers | declaration order of locals; splitting or merging a variable; a temporary local; an intermediate variable of another type (for example a `void*` before the cast) |
| instructions in another order | order of statements or of sub-expressions; order of function arguments (often evaluated right to left) |
| an extra or missing load | the compiler re-reads memory when it cannot prove nothing changed it: use a local copy, or the reverse; `const` on a pointer can change it too |
| an extra stack store / different stack slots | a temporary object created by an expression; passing a struct by value versus by reference; declaration order (first declared often gets the higher address) |
| a value kept in a register in the original, reloaded in yours (or the reverse) | a local variable versus re-reading a member or a global |
| a constant loaded inside a loop instead of before it | the constant is really the initial value of a variable modified in the loop |
| different branch layout | `if/else` versus early `return`; `continue` versus a nested `if`; inverted condition; ternary versus `if` |
| a call where the original inlined the code (or the reverse) | define the helper as inline, before or after its caller; change its size |
| one rounding or conversion instruction too many | the intermediate type (float versus double, signed versus unsigned) |
| whole functions in another order | definition order; inline functions emitted where first used |
| constants in another order in the pool | the order the compiler first meets them in the file, headers included |

## Habits of good matching source

- **Write like a programmer of the time would have**: plain loops, small inline
  helpers in headers, temporaries for readability. Odd code that happens to match is
  a sign the real source looked different.
- **Inline helpers are common.** Repeated code shapes (the same sequence in several
  functions) usually come from one inline function or macro: write it once.
- **Look for twins.** If another function has the same code shape, it was probably
  the same inline helper, and a solution for one is a solution for both.
- **Check exact bytes, not only the score.** Fuzzy scores may ignore register
  numbers; confirm with the real linked output.
- **Record every new technique** with the symptom it solves. The same compiler has
  the same habits everywhere.

## When nothing works

Some differences resist: the compiler version might be slightly wrong, the original
might have been compiled with special flags for that one file, or a source shape
nobody has thought of yet. Then keep the unit non-matching, describe the remaining
difference precisely (which instructions, which registers), and come back later.

This ends the general part. The project part starts with
[Setup and toolchain](../project/01-setup-and-toolchain.md).
