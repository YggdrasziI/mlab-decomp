# Matching techniques (mwcc GC 3.0a5.2, game code)

Lessons that made game functions match, each checked by a build. Read this when a
function is stuck at 99 %. Unit references point to `src/game/`.

## Loads, aliasing and `const`

- A load through a `const T*` parameter may be scheduled above the prologue stores
  (the stack stores cannot alias it); a load through a `T*` cannot. The named script
  callbacks take a const context (fixed `unit_80018C04`, `unit_80077B50`,
  `unit_800938A4` and three functions of `unit_80018E00`; flags, `-proc` models and
  compiler versions do not change this).
- In the same way a load through a const `this` can be reused where one through a
  non-const `this` is reloaded (`UnkTileRing::GetHead() const`).
- The address of a real member of a global (`lbl_803B53A0.mD5F0`,
  `lbl_803C4298.mStick`) keeps only the base in a register, as in the DOL; a cast of
  the base plus an offset is kept whole.
- A global accessed with `lis`/`@l` instead of `@sda21` is larger than 8 bytes:
  declare it as an array or struct (the texture cache `lbl_8040ADF0[8]` in
  `unit_800445E4`).

## Registers and stack

- An 8-byte struct the DOL returns through a hidden pointer in r3 (the caller passes
  a stack slot) is returned in r3/r4 by this build unless the struct has a
  user-defined copy constructor (`UnkTreeRange` in `unit_800A34C0`).
- A constant stored through a call result (`vf10()->m->f = 0;`) is kept in a saved
  register across the call; storing through a local pointer first
  (`p = vf10()->m; p->f = 0;`) gives the DOL's `li r0` after the call
  (`unit_800A34C0`).
- A run of widget lookups `w = find(screen, hash); if (w) set(w, ...);` with the
  screen and the widgets in the wrong saved registers: give each lookup its own
  local (or its own block) instead of reusing one variable (`fn_8013C6CC`).
- `if (a == 0 || f() == 0) return;` compiles to `bne` over a `b` to the epilogue; the
  DOL's two `beq` come from two separate early returns (`fn_8013DE7C`,
  `fn_8013CFA8`).
- A global pointer the DOL reloads after every call must stay a global in the source:
  passing it to an inline helper keeps it in a saved register (`fn_8013CFA8`).
- A loop `m = t->models[i]; f(t, i, 0); g(m);` whose saved registers came out in the
  wrong order matches as an inline helper doing the three steps (`ReleaseModel`).
- A constant the DOL keeps in `f31` across a call is a local `f32` variable
  (`f32 duration = 5.0f;`).
- The declaration order of locals sets their register numbers (a result declared
  before the entity pointer, for example). A local declared at the top of its block
  but assigned later takes a different saved register (`flag` in `fn_800D8280`).
- Stack slots: the first declared local gets the higher address; compiler
  temporaries created later get lower ones.
- A pointer to a member at a non-zero offset taken with a null check: an explicit
  `if (p) q = &p->m; else q = 0;` gives the DOL's `addi`/`b`/`li 0` form; a ternary or
  an implicit base-class conversion adds a second compare (`fn_8003FC6C`).
- A value returned by a function and passed by address only once can be a compiler
  temporary in the DOL (lowest stack slot, below the named locals of later blocks):
  pass it through an inline taking a `const&` (`XBaseActionSelectionCallback::Select`,
  `vf0C` of `unit_80040090`).
- A list element taken through an inline accessor and null-checked: when the
  pointer's register is numbered last instead of first, store the accessor result in
  a `void*` local, test that, and cast it to a second local (`fn_800B20A4`).
- Arguments of an inlined call are evaluated right to left: two member loads in the
  wrong order can mean the inline takes its parameters in the other order
  (`SweepSpheres` in `unit_800B1EDC`).
- `u32 % (int)(float)` gives the DOL's call-first order with an inline `fctiwz`; a
  `(u32)` cast of the float calls `__cvt_fp2unsigned` before the call (`fn_8003CE20`).
- An empty `__sinit` (a lone `blr`) comes from a global object whose class has an empty
  inline constructor, in a unit that does not include `unk_vec3.h` (`unit_801604FC`).
- Two identical loops with different register numbers: the second function may just
  call the first, which is inlined (`fn_80160AFC` calls `fn_80160A28`).
- Locals that look like an 8-byte array may be a 12-byte struct: the stack order only
  matched with the callback output typed as the 12-byte parameter struct
  (`fn_801606A4`).
- An all-zero object the DOL keeps in `.sdata`/`.data` needs `#pragma explicit_zero_data
  on` around its definition (`lbl_805F8200` in `unit_80010B5C`); objects are emitted in
  definition order, so define it after the functions whose literals come before it.
- A string literal that the DOL places before an earlier function's jump table is a
  named file-scope array defined before that function (`sDiscReadError`).
- `cntlzw`/`srwi.` on a call result before an `&&`: a `bool b = f() == 0;` local tested
  as `!b` (`fn_80010CAC`).
- A counted loop whose counter is not used and counts down with `subic.`: `int n = 4;
  ... do { } while (--n);`, with `n` declared at the top of the block.
- A local pointer computed before the updates can change register allocation of the
  whole tail (`fn_80044634`: `u8* p = base + used;` before `free -= size; used += size;`).

## Floats

- A float returned by a call, stored to a global and passed on: `g = f(); h(w, g);`
  with `f` returning `f32` gives the DOL's `stfs` then `frsp` (MWCC rounds the
  returned value again for the argument); an `f64` return gives `frsp` first
  (`fn_8013E4D0`).
- Three `bne` on one `fcmpu` (no reload in between): the source compares the same
  value three times, a copy-paste slip such as `-1.0f == x && -1.0f == x && -1.0f
  == x` (`fn_8013E898`).
- A copy of a global `const UnkVec3` loaded x, z, y before `+=`: build it with the
  three-float constructor, `UnkVec3 v(k.x, k.y, k.z); pos += v;` (`fn_8013E898`).
- A matrix-vector product whose scheduling and stack slots did not match as
  `UnkVec3 end = mtx.Transform(v);` matches when written through a pointer,
  `UnkVec3 end; mtx.Transform(v, &end);` (`fn_8013EDFC`, `fn_80139CC4`).
- A float member tested as `if (x)` compiles to `fcmpu x, 0` (member loaded first);
  `x != 0.0f` loads the constant first (`fn_8000C604`).
- Vector temporaries filled by chained assignment (`v.x = v.y = v.z = 1.0f`) are
  stored z, y, x (`fn_8000C508`).
- A `frsp` after a select means the value went through a `double`: an inline helper
  returning `double` reproduces it (`UnkMaxD` in `unit_8000C478`).
- A 0.0 constant the DOL loads inside a loop (not hoisted) and shares with a result
  variable: the variable is initialized to `0.0f` in the loop (`f32 t = 0.0f;` before
  an inline that may leave it unchanged) (`fn_800B20A4`).
- `d = b * b - a * c;` whose result took the register of `a * c`: `d = b * b;
  d -= a * c;` keeps it in the register of `b * b` (`fn_800B20A4`).
- A sized `.sdata2` symbol that covers a literal and its padding (size 0x8 for a
  float) leaves the section below 100 % in the report; give it the literal's size.
- Constants are pooled in `.sdata2` in parse order: the shared `(0, 1, 0)` vector of
  `unk_vec3.h` puts 0.0 and 1.0 first when the header is included first. An unused
  static function can give the pool its DOL order (the original files likely had
  dead code).

## Inlining and emission order

- `-inline auto`: small functions defined earlier in the file are inlined
  (`fn_80016CEC`); a slightly larger body keeps a call where the DOL has one
  (`fn_80016DB0`). Small setters the DOL calls out of line are defined after their
  caller (`UnkQuadVertex` in `unit_801210E4`).
- `-inline auto,deferred` (per unit) inlines functions defined later in the file and
  emits functions in reverse order of definition (QuestLog, `unit_8011EA20`).
  Switching all game code to it breaks other units.
- Generated destructors registered by a static initializer are emitted after
  `__sinit` in reverse order of use. When the DOL has them in registration order,
  `-ipa file` (per unit) is the only setting found that does it (`unit_801F59F0`);
  on all game code it reverses the inline functions of other units.
- Inline functions used by a unit are emitted after the function that first needs
  them; weak functions after the `__sinit` belong to the unit before them.
- Base-class RTTI copies emitted in a derived unit are dropped by the linker.

- MSL map destructors (`unit_8009F66C`, `unit_8006F794`): only `-ipa file` (per unit)
  gives the DOL's shape, the recursive node destruction inlined three levels deep and
  the element destructor chains inlined in full; `inline_depth`, `inline_max_size`,
  `nobottomup` and `deferred` each fix one function and break another. With it,
  implicit destructors of templates are inlined too. A template function shared by
  two units must have the same mangled name in both (the earlier unit emits it, an
  explicit instantiation is enough).
- A constructor makes the compiler emit unreferenced weak copies of the members'
  inline destructors; the linker strips them (they are not in the DOL).

## Data layout

- A struct copied with a word loop to the stack and then member by member (with
  spills) matches when the words are separate members, not an array
  (`UnkOpponentData`, `fn_8013CA74`).
- A static initializer that addresses each object with its own `lis` instead of one
  .bss base needs `#pragma pool_data off` (`unit_8013CA74`).
- `.bss` objects are laid out in declaration order, including the `sUnkVec3` that
  `Entity.h` pulls in through `unk_vec3.h`. An object the DOL places before that
  `sUnkVec3` is defined before the include (`unit_8013EB1C`); objdiff pairs the
  symbols by name and still shows 100 %, only the `main.dol` link catches it.
- Units with only a static initializer (`unit_80006B40`, `unit_80042C18`) are kept by
  FORCEFILES in `config/RM7P4F/ldscript.tpl` (nothing references them and
  FORCEACTIVE cannot name a local `__sinit`).

- The vtable pointer goes where the first virtual function is declared among the
  data members: a base whose members come before the vtable (`MiniGame` at +4,
  `ExWorldTileSetPool` at +0xC) declares those members first.
- Five `beq` on one pointer before a member's destructor body are five nested
  destructor calls (one `this` test each): the MSL tree owner is three classes below
  `__tree` (`include/msl_tree.h`).

## Control flow

- Switches whose cases are listed in descending order (with an empty `case 4`) give
  the DOL's compare trees; dead compare trees come from an inline function with a
  `switch` whose result is unused.
- `if (ok) { ...; return x; } return 0;` versus an early `return 0` changes which
  path falls through (`fn_801F5BB8`).
- A `b` over unreachable code (a call and a test) right after a member is set to 0:
  write the store, then the `if (member != 0) { ... }` test; CW folds the condition
  but keeps the dead block (constructor of `unit_800D7F24`).
- Search loops whose count the DOL reloads every iteration (no ctr loop) and whose
  group index stays in a register (`li 0` / `mullw`): not reproduced yet
  (`fn_800D8724`, `ExWorldWireTile::vfBC`).

- Two copies of the same code in a switch are two separate groups of `case` labels
  (CW does not merge them; `fn_800A0FB4`).
- A search loop returning true from inside with `li r3,0` set before the loop: a
  `bool found = false;` set and `break`, then `return found;` (`BuzzsawTileSetPool::vf20`).
- Calls through a member object's vtable, with `this + offset` recomputed for each
  call: the source goes through an inline accessor returning a base pointer
  (`BuzzSawExperiment::GetTrack`); a local pointer keeps it in a saved register.
- A clamp followed by `frsp` before a second clamp: the first one compares floats but
  returns a double (`UnkClampD` in `unit_8009F66C`).

## Bit fields and hardware registers

- Header words built with `(x & ~mask) | (v << shift)` give `slwi` + `rlwinm` + `or`;
  real bit fields give `rlwimi`. Use the form the DOL shows (`UnkMsgHeader`).
- Raw GX FIFO writes (`WGPIPE` at 0xCC008000) follow the DOL when the register value
  is the argument of an inline function (`UnkLoadBP`, `UnkLoadXF` in
  `unit_800445E4`): the value is computed before the command byte, as an argument.
  Include `<revolution/gx/GXHardware.h>` before the game headers (`types.h` clash).

## Unsolved patterns

- `.bss` order against initialization order (`unit_8013CA74`, and every DOL static
  initializer that builds a registered object and `sUnkVec3`): the DOL initializes
  `sUnkVec3` first but places it after the object and its destructor chain. All the
  GC 3.0 and Wii 1.x compilers tried lay .bss out in definition order (also with
  an `extern` declared first, copy-initialization, `-inline deferred`); `-common on`
  moves the object out of the unit.
- GameSpy units: empty debug branches keep a jump and a dead compare in the DOL that
  our compile removes.
- Identity matrix built on the stack as four 12-byte temporaries in row order, each
  copied into the matrix right after it (`fn_8012112C`): no spelling found.
