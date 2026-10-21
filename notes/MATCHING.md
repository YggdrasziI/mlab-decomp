# Matching techniques (mwcc GC 3.0a5.2, game code)

Lessons that made game functions match, each checked by a build. Read this when a
function is stuck at 99 %. Unit references point to `src/game/`.

## Loads, aliasing and `const`

- A load through a `const T*` parameter may be scheduled above the prologue stores
  (the stack stores cannot alias it); a load through a `T*` cannot. The named script
  callbacks take a const context (fixed `unit_80018C04`, `unit_80077B50`,
  `unit_800938A4` and three functions of `unit_80018E00`; flags, `-proc` models and
  compiler versions do not change this).
- A virtual whose first member load sits above the prologue (`lfs` before `stwu`):
  declare it `const` in the whole hierarchy (`ExWorldTile::vfD0() const`,
  `unit_800C4530`; mangled names become `...CFv`).
- In the same way a load through a const `this` can be reused where one through a
  non-const `this` is reloaded (`UnkTileRing::GetHead() const`).
- The address of a real member of a global (`lbl_803B53A0.mD5F0`,
  `lbl_803C4298.mStick`) keeps only the base in a register, as in the DOL; a cast of
  the base plus an offset is kept whole.
- A compare CW folds (`i <= tail` where both come from the same member load) but the
  DOL keeps (`cmpw r7,r7`): pass the index to the inline function by `const int&`
  and copy it into a local (`UnkDvdRequestQueue::Contains`, `unit_801E47D8`).
- A member the DOL reloads where this build reuses the register of an earlier
  inlined load: read it through a local reference (`int& tailRef = mTail;`,
  `UnkDvdRequestQueue::PopBack`).
- Fields of a stack copy of a struct reloaded after each call where this build keeps
  them in saved registers: the copy is passed by reference to an inline function
  (`UnkBadge::Init(const UnkBadgeInfo&)` in `unit_8011C460`).
  A local reference to the copy does the same in place (`UnkMonsterDesc& desc = temp;`,
  `Parts::fn_8011B060` in `unit_8011A778`).
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
- A bounds-checked getter whose DOL code loads the element after the `li 0` branch is
  written `i >= count ? 0 : data[i]` (`UnkList::Get`, `unit_8012F6B0`).
- Placement new on a pointer tested once: the DOL's `cmpwi cr1` before a switch and
  `beq cr1` in each case come from `if (p != 0) switch (...) { new (p) T; }`; without
  the outer test each case gets its own `cmpwi` (`fn_8012F818`).
- Several saved registers in the wrong order in a loop over a switch (case
  constants and vtables hoisted): declaring the loop pointer before the count and the
  index at the top of the function fixed `fn_8012FD0C`; script the permutations.
- A value stored to a global and then compared: the DOL compares the loaded value
  (`int id = p->mId; global = id; if (id == -1)`); comparing `p->mId` again reloads
  it after the store (`unit_8001EE48`).
- `if (g != 0) { delete g; g = 0; }` with the cleanup in an inline destructor gives the
  DOL's saved registers; the same code written out before `delete` numbers them
  differently (`fn_8001EF50`).
- Small local arrays initialized with constants are copied from compiler-made
  `.sdata2` objects of the array's full size; dtk splits them into 4-byte symbols, so
  `symbols.txt` must give each one the array size for `.sdata2` to match
  (`unit_8001A17C`).
- The declaration order of locals sets their register numbers (a result declared
  before the entity pointer, for example). A local declared at the top of its block
  but assigned later takes a different saved register (`flag` in `fn_800D8280`).
- Stack slots: the first declared local gets the higher address; compiler
  temporaries created later get lower ones.
- A pointer to a member at a non-zero offset taken with a null check: an explicit
  `if (p) q = &p->m; else q = 0;` gives the DOL's `addi`/`b`/`li 0` form; a ternary or
  an implicit base-class conversion adds a second compare (`fn_8003FC6C`).
- `addic. r0, rX, off` / `beq` on a pointer already tested for null is the address of
  a member struct compared with 0 (`if (&part->mLink == 0)`); a base-class
  conversion adds a second compare (`vf0C` of `unit_8004AC68`).
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
- Struct copies in the wrong stack slots: the DOL sorts them by size; when this build
  does not, get them from an inline function that returns the struct by value
  (`GetAbilityInfo` in `unit_8011A778`).
- A struct field copied into a local (`u8 slot = part.mSlot;`) and used for several
  indexes gets the DOL's registers only when each use reads the field itself
  (`part.mSlot - 1`, `IngredientSelection::fn_8011A04C`).
- A clamp that reads the member again in each branch (`lwz` in both arms): write it
  as a macro on the member, not an inline function taking the value
  (`UNK_CLAMP_BETWEEN`, `Parts::fn_8011B898`).
- A local pointer computed before the updates can change register allocation of the
  whole tail (`fn_80044634`: `u8* p = base + used;` before `free -= size; used += size;`).
- `u8 flag = x == 1;` gives `srwi` with a register order the DOL does not use;
  a `bool` member, set before the other member update and read back from the
  member (`mFlag = mTab == 1; mTab += d;`), matches (`fn_80118FC0`). A getter
  `return a - b * 4;` with the operands in the wrong registers matches as
  `int r = a; r -= b * 4; return r;` (`fn_80119538`).

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
  stored z, y, x (`fn_8000C508`); the three-float constructor
  (`UnkVec3(1.0f, 1.0f, 1.0f)`) stores x, y, z (`BuzzsawGate::vfBC`).
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
- A function too large for `-inline auto` that the DOL inlines anyway: write its
  body as calls to the smaller functions it dispatches to (each inlined in turn), so
  it fits the size limit (`fn_80118858` in `unit_80117AE4`). The `inline` keyword
  drops the out-of-line copy the DOL also has, and `#pragma inline_max_size` had no
  effect.
- Generated destructors registered by a static initializer are emitted after
  `__sinit` in reverse order of use. When the DOL has them in registration order,
  `-ipa file` (per unit) is the only setting found that does it (`unit_801F59F0`);
  on all game code it reverses the inline functions of other units.
- The order of those destructors follows the order in which the objects are
  defined; the class definitions, `extern` declarations, template or plain classes
  and `-inline deferred` do not change it, nor do explicit (inlined) destructor
  calls in an earlier function: only the out-of-line reference made for the
  registration counts (`unit_801088C0`, still unsolved).
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

- Vtables (with their class name strings and base lists) are emitted in reverse order
  of the class declarations: declare the classes so that the last one declared is the
  first vtable in the DOL (`unit_800A5BCC`).
- A 0 entry in a vtable is a pure virtual function (`BuzzsawItem::vfC4`); the class
  can still have an out-of-line constructor called by derived classes.

- A vector assigned from a member through a stack temporary (copied to the stack,
  then to the local): `pos = UnkVec3(obj->m27C);` (copy constructor temporary)
  (`fn_800B182C`).
- Two stack temporaries in swapped slots: a named local (`UnkVec3 d = a - b;`) gets
  its slot before the temporaries of later expressions; writing it as an
  expression (`(a - b).LengthSq()`) makes it a temporary with a lower slot
  (`fn_800B182C`).
- Locals of an inline member function take stack slots below the temporaries of
  its caller; when the DOL has the last vectors of a function at the lowest
  addresses, move that code into an inline member (`UpdateBounds` in
  `unit_8007C4AC`). One named vector assigned twice (`p = a + b; e->SetPosition(p);`)
  gives the single shared slot the DOL uses for two such calls.
- A unit whose .rodata cannot be split (link order cycle) can still match and link:
  its tables stay in the unsplit .rodata and the source declares them `extern`
  (`unit_800B1498`).
- While a unit is `NonMatching` its assembly is linked, so a `main.dol` byte diff
  says nothing about it: score variants with the objdiff report, and switch to
  `Matching` only to compare the linked bytes.
- An index register numbered like the DOL (`mulli r0` for the index, the array base in
  `r3`): store the call result in a local (`int index = model->fn(...);`) before
  indexing; a pointer to the member object (`UnkEntity94* model = &m94;`) also keeps
  the stack stores of a dead vector copy (`fn_80098700`).
- Four float reads stored straight into a struct (not through constructor arguments,
  which are evaluated right to left): assign the members one by one
  (`fn_8009875C`).
- A struct assignment whose float members the DOL copies with `lfs`/`stfs`: the
  implicit assignment of a struct of `UnkVec3`/`UnkQuat` copies them as words
  (`lwz`/`stw`), and placement `new` with the copy constructor adds a null check.
  Copying the members one by one (floats as floats) matches (`UnkCopyInfo` in
  `unit_8010C750`).
- objdiff does not compare the values behind constant relocations: a function at
  100 % can load 0.0 where the DOL loads 1.0. Check the `.sdata2` values (and the
  linked `main.dol`) when the pool order looks wrong; here the "zero" vector was
  `(1, 1, 1)` (`fn_8010C90C`).
- A `while (test) { advance; }` loop is laid out as a branch to the test at the
  bottom; a test that needs a flag variable (`int free = 0; if (...) free = 1;`)
  gives the DOL's `li 0` / `li 1` / `cmpwi` (`fn_80109600`).
- Function bodies that are identical in the DOL but use different registers when one
  of them goes through an inline helper: write the loop out in the function instead
  of calling the helper (`fn_80109A20`).
- Two locals in the other registers: declare the one the DOL numbers higher first,
  even uninitialized (`int page;` in `Recipes::fn_801205EC`, `f32 minY;` before
  `f32 minX = mMin.x;` for a float pair copy in `unit_8007C4AC`), or split the
  initialization (`int element = mElement; element += delta;`, also for floats:
  `f32 t = x - a; t /= range;`).
- The unroll factor tells the loop: a 64-word loop gives 2 iterations of 32 stores
  (zeroing) or 4 iterations of 16 copies, where a loop over four 16-word structs
  is unrolled by 2 (`unit_800D4C64`).
- A copy whose loads all come before the stores (many saved registers, no stack):
  straight-line member assignments let the scheduler hoist them; a loop in the
  middle stops it, and the implicit `operator=` or copy constructor interleaves the
  loads and stores (`Unk800D4C64::operator=`).
- Where a weak-looking function belongs: the sdata2 constants it uses. Functions
  after a unit's static initializer that load the constant of the next static
  initializer are their own unit (`unit_800D4C64`).
- Descending loops `for (i = n - 1; i > -1; i--)`: the DOL's `cmpwi i, -1` comes
  from `i > -1`; `i >= 0` gives `addic.`/`blt` (`unit_800D3428`).
- Bounds-checked container accessors inlined everywhere (`PathAPI.h`): the clamp is
  a conditional expression (`v < lo ? lo : (v > hi ? hi : v)`; the if/return form
  numbers the registers differently), and `if (mUsed[i] == false)` gives the DOL's
  branch order where `!mUsed[i]` inverts it.
- The vtable pointer of a class whose base has none goes after the data members
  only when they are declared before the first virtual function; declared after,
  it lands at offset 0 (`Exploration::PathAPI`).
- Member subobject destructors are called with -1, base class destructors with 0:
  this tells a member at offset 0 from a base.
- Small constructors and destructors that the DOL has after the last function of a
  unit, in definition order: define them out of line at the end of the file; the
  compiler places generated copies right after their first user instead. With
  `-ipa file` an inline function defined last is still inlined earlier, and the
  inline functions it needs are emitted at the end (not needed in the end for
  `unit_800D3428`).
- Unreferenced weak destructors of container templates emitted by the compiler are
  stripped at link time: the unit still links byte for byte.
- A byte loop over an image row with a pointer increment in the DOL: index the row
  (`row[x] &= mask;`); an explicit `p++` gives the same code with other registers
  (`fn_8007C50C`).
- `a = b = x;` stores b first: swap the chain when the DOL stores the other global
  first (`fn_80109A20`).
- String literals are pooled in parse order (small ones in .sdata, others in
  .data); when the DOL has them in another order, an unused static function using
  them first gives the order (`UnusedArchiveNames` in `unit_801091EC`). `static const
  char* const` pointers are not folded: they become .sdata2 objects.
- A character the DOL reloads after the stores of a copy loop: the source pointer
  is `char*`, not `const char*` (mwcc assumes stores do not change data read through
  a pointer to const) (`fn_80121508`).
- A global reloaded after a call to `strlen` where this build keeps it in a
  register: declare `strlen` yourself (`unsigned long strlen(const char*)`) instead of
  including `<string.h>`, whose declaration lets mwcc assume the call does not
  touch memory (`fn_80121508`).
  The same holds for `strcmp` (`unit_8011D5C4`): with `<string.h>` CW treats it as
  pure and keeps compared values in saved registers across the calls.
- A small struct copied through registers (no stack temporary) in a swap: give the
  type an inline copy constructor (`UnkStatRow` in `unit_8011D5C4`); the implicit
  copy goes through a stack temporary.
- A value read in the condition of one loop and reused by the next one:
  `while (less(i->mName, key = mid->mName)) i++; while (less(key, j->mName)) j--;`.
- An array base the DOL keeps in a saved register through an inlined function
  (instead of recomputing `r1 + offset`): copy the parameter into a local inside the
  inline function.
- `lis r9` then `addi r31, r9` for a global's address: the source uses the global
  directly (the compiler keeps the common address in a saved register); a local
  `T* g = &global;` gives `lis r31` (`fn_80121508`).
- An external object of 8 bytes or less is addressed through r13 (small data) unless
  its declared size matches the real one: declare the full size (`UnkScreenSize`,
  0x18 bytes, in .bss) (`fn_80121B08`).
- A `switch` with cases 1 and 2 whose compare tree starts at 1: add `case 0:` with
  the default label (`fn_80121B08`, also `fn_801091EC`).
- A value kept in r4 (not r0) across a call that takes this object in r3: the
  callee takes it as a second argument (`fn_8004D760(stands, creature)` in
  `unit_80117090`); add the parameter.
- An array searched through a pointer to its start, with the count read at an
  offset from that pointer: the array and its count form a sub-struct with its own
  inline search method (`UnkCreatureList::IndexOf`).
- A struct copied with a word loop to the stack and then member by member (with
  spills) matches when the words are separate members, not an array
  (`UnkOpponentData`, `fn_8013CA74`).
- A sum of many call results (`fn_801D2BF8()->mXX + ...`) is reassociated by mwcc:
  parentheses do not change the code. Successive `n += ...;` statements keep the
  DOL's add-after-each-call order (`GetBlockCount` in `unit_801088C0`). Operands of
  equal weight are evaluated right first (`a() * b()` calls `b` first).
- A static initializer that addresses each object with its own `lis` instead of one
  .bss base needs `#pragma pool_data off` (`unit_8013CA74`).
- **Precompiled header.** In every game unit of the DOL the shared `(0, 1, 0)` vector
  of `unk_vec3.h` is the last `.bss` object, although its initializer runs first.
  mwcc lays out `.bss` in definition order, except that objects coming from a
  precompiled header are emitted after the unit's own objects (tested with every GC
  3.0 version; flags such as `-ipa file` or `-inline deferred` change nothing). So the
  game was built with a precompiled header: every game unit that has the vector
  starts with `#include "game/unk_vec3.mch"` (built from
  `include/game/unk_vec3.pch++`), or `game/unk_vec3_rvl.mch` when it includes Wii
  SDK headers (that variant puts the SDK's `types.h` first, as those units did
  before). It must be the first `#include`; units without the vector (only a static
  initializer of their own, `unit_801604FC`...) must not use it. It fixed the order
  of a registered object (`unit_8013CA74`), function-local statics
  (`unit_80098664`, `unit_8009F66C`) and replaced the old workaround of defining
  objects before the includes. objdiff pairs `.bss` symbols by name, so only the
  `main.dol` link shows a wrong order.
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

- A call whose float result is never stored, followed by a call that reads f1
  (`GetTrack()->vf20(); GetTrack()->vf24();`), is one call passing the result of the
  other (`vf24(vf20())`): check the callee for an f1 argument before writing a void
  signature (`ExWorldTrack::vf1C/vf20/vf24`, `unit_800C52B4`).
- Two copies of the same code in a switch are two separate groups of `case` labels
  (CW does not merge them; `fn_800A0FB4`).
- A search loop returning true from inside with `li r3,0` set before the loop: a
  `bool found = false;` set and `break`, then `return found;` (`BuzzsawTileSetPool::vf20`).
- Calls through a member object's vtable, with `this + offset` recomputed for each
  call: the source goes through an inline accessor returning a base pointer
  (`BuzzSawExperiment::GetTrack`); a local pointer keeps it in a saved register.
- A square root rounded once (`fmul` then a single `frsp` into f1): an inline
  returning `f32`; `UnkSqrtf` returns a double and gives two `frsp` when the caller
  returns a float (`ExWorldCurvedTile::vfD0`).
- An `frsp` on a float value just before it scales a vector: the value comes from an
  inline returning a double (`UnkRatio` in `unit_800C4530`); a `double` local instead
  pools a double constant.
- Results written back into a vector's own stack slots without a temporary:
  subtract component by component (`v.x -= c.x; ...`); `v = v - c` adds temporaries
  and enlarges the frame.
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

- A struct copied twice on the stack, first word by word then member by member
  (`lha`/`lbz`), with both copies kept and the base pointer reloaded every iteration
  (`Recipes::fn_80120990`): copy constructor, assignment, second type, by-value or
  member predicate all let CW drop the copies.

- An inlined member function whose stack temporaries come out in the opposite order
  of its out-of-line copy (`fn_8009875C` inlined in `fn_800988F0`): not reproduced
  (shared inline helper, getter inline, color by value tried).
- GameSpy units: empty debug branches keep a jump and a dead compare in the DOL that
  our compile removes.
- Identity matrix built on the stack as four 12-byte temporaries in row order, each
  copied into the matrix right after it (`fn_8012112C`): no spelling found.
