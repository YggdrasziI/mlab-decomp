# Code and data regions

Region map of `main.dol` (RM7P4F), used to split SDK, runtime/libc and game code.
Every boundary below comes from `config/RM7P4F/symbols.txt` (names recognized by dtk),
`<< RVL_SDK - … >>` version strings, or cross-references computed from the
disassembly in `build/RM7P4F/asm`.

## Important: what the build does and does not prove

dtk splits the original bytes and the linker puts them back together, so
`main.dol: OK` holds for **any** consistent set of split boundaries. A build that
matches proves the bytes; it does **not** prove that a split matches a real translation
unit (TU). A TU boundary is only proven once the TU is decompiled and matches.

## `.text` regions

| Region | Start | End (excl.) | Evidence |
|---|---|---|---|
| `.init` (startup, `memcpy`, `memset`) | 0x80004000 | — | `__check_pad3`, `__start`, `__init_registers`, `memcpy` 0x80004338, `memset` 0x8000443C |
| Game (+ possible middleware) | `.text` start | 0x80232AF8 | see "Game / SDK boundary" |
| Wii SDK (RVL) and network libraries | 0x80232AF8 | 0x8032EEE4 | `PPCMfmsr` 0x80232AF8; version strings registered by `EXIInit`, `SIInit`, `GXInit`, `DVDInit`, `SCInit`, … up to SSL (`fn_8032E18C`) |
| Runtime.PPCEABI.H | 0x8032EEE4 | 0x8032FEF0 | see "Runtime.PPCEABI.H units" below |
| MSL_C (including its math library) | 0x8032FEF0 | 0x8033F460 | see "MSL_C units" below; the 43 unnamed functions after `fwide` are MSL math (`e_pow.c` … `w_sqrt.c`) |
| MetroTRK | 0x8033F460 | 0x80344FD4 | see "MetroTRK units" below |
| Debugger communication driver (EXI2) | 0x80344FD4 | 0x80345AD8 | 31 functions; MetroTRK calls `DBInitComm`, `DBInitInterrupts`, `DBQueryData`, `DBRead`, `DBWrite`, `DBOpen`, `DBClose` here (0x80345110-0x80345388), the `DebuggerDriver.c` API. Function sizes match neither Petari's `NDEV` nor TP's `NdevExi2A`; exact library variant unknown, not split yet |

`strlen` (0x8032EEE4, weak) belongs to the runtime's `__mem.c` (see below).

### Game unit boundaries (static initializers)

Every game unit that includes the header with the shared `(0, 1, 0)` vector object gets
a `__sinit_*` function at the end of its `.text`, with one `.ctors` entry. 293 of the
300 `.ctors` entries point into game `.text`, so each one marks the end of a unit
(a range may still hold several units without a static initializer). Each unit also
has its own `.sdata2` float constants (0.0, 1.0...), its own 12-byte vector in
`.bss`, and its vtables, class-name strings and `__RTTI__` objects in `.data`/`.sdata`.
Class names come from those RTTI strings; method and file names are unknown, so
game units are named `game/unit_<start>.cpp` and virtual functions `vfXX` (vtable
offset).

| Unit | .text | Content |
|---|---|---|
| `game/unit_8000C7C8.cpp` | 0x8000C7C8-0x8000CF8C | top-level game flow: delayed event, state stack update, start-up of the global managers, screen set loading (`"ScreenSet load start: zone=%d"`), forwarders (NonMatching: 30/31, `fn_8000C838` 96 %) |
| `game/unit_80014230.cpp` | 0x80014230-0x8001425C | build date string `"Aug 28 2008 (14:30)"` (.rodata 0x803476A8) and the two build numbers 519 / 142835 (.sdata2 0x805FC720) (matches, linked) |
| `game/unit_8001425C.cpp` | 0x8001425C-0x80014328 | build region word (0 = `NA`, 1 = `EU`; this DOL holds 1) and the `"%s %d: %s@%d (%s)"` build string, type `"REL"` (matches, linked) |
| `game/unit_80014328.cpp` | 0x80014328-0x80014350 | one function returning 0, used as a function pointer (matches, linked) |
| `game/unit_80018C04.cpp` | 0x80018C04-0x80018E00 | registers three named animation predicates (`"anim_done"`, `"anim_past_impact_frame"`, `"anim_past_preload_time"`) through `fn_801DC91C`, then calls six other registration functions (NonMatching: `fn_80018CB0` 95.6 %, prologue scheduling) |
| `game/unit_800197A8.cpp` | 0x800197A8-0x80019CC8 | named script callbacks for props and hazard props (`"prop_play_anim_and_fx_once"`, `"hazard_prop_idle_update"`, `"NavTest"`...) (NonMatching: 12/18, float registers and scheduling) |
| `game/unit_80019CC8.cpp` | 0x80019CC8-0x80019FB0 | script callbacks for props sending up to three delayed events (`"check_delayed_timer"`, `"send_delayed_event_to_zone"`, `"reset_trigger_send_delayed_events"`); timers are `Entity::m28` plus a delay (matches, linked) |
| `game/unit_80019FB0.cpp` | 0x80019FB0-0x8001A17C | launch speed for a projectile between two points at a given angle under gravity (NonMatching, 89.8 %: the `height` / `s` registers are swapped and two stores are reordered) |
| `game/unit_8001AFB4.cpp` | 0x8001AFB4-0x8001B20C | a stack of states: init, lookup by id, pending state, push and pop (matches, linked) |
| `game/unit_8001FCA8.cpp` | 0x8001FCA8-0x80020480 | `CAbstractInputEventStage` base, the source-update function, the stage sequence and factory (matches, linked) |
| `game/unit_80020480.cpp` | 0x80020480-0x800214D4 | the 10 `CAbstractInput*` stages (29/31 functions match) |
| `game/unit_800214D4.cpp` | 0x800214D4-0x80021594 | the set of input sequences (8 fixed, 100 indexed, 34-entry index table) (matches, linked) |
| `game/unit_800232A4.cpp` | 0x800232A4-0x80023884 | queries and actions on the object returned by `fn_80021750`: state checks, counters, events chosen by its mode (switches written with descending cases and an empty `case 4`), `"dirt_fling"` (matches, linked) |
| `game/unit_8002B4D0.cpp` | 0x8002B4D0-0x8002B58C | class `BugCachingChallenge::ButterFly` (RTTI), derived from `BugCachingChallenge::Bug` (functions at 0x800293C8, not split); constructor sets the float at 0x4 to 6.4 (matches, linked) |
| `game/unit_80030228.cpp` | 0x80030228-0x80030604 | class `SewerItem` (RTTI): doubly linked item bound to an actor, random interval timer, proximity test against a squared radius (matches, linked; subclasses `SewerBottle`, `SewerMetal`, `SewerRing`, `SewerBubble`, `SewerFish` in the next unit) |
| `game/unit_80030604.cpp` | 0x80030604-0x8003107C | `SewerItem` subclasses `SewerBottle`, `SewerMetal`, `SewerRing`, `SewerBubble`, `SewerFish` (RTTI): per-type sound cycling; `SewerFish` runs a six-state catch sequence (NonMatching: 37/38, one register in `SewerFish::vf10`) |
| `game/unit_8003F91C.cpp` | 0x8003F91C-0x8003FB24 | class `DebugMenuActionSelectionCallback` (RTTI), named "Debug menu callback"; owns a 0x9C-byte debug menu object (functions at 0x8003FB24, not split); `XBaseActionSelectionCallback`'s RTTI is emitted first here (matches, linked) |
| `game/unit_80040638.cpp` | 0x80040638-0x80040824 | class `NetworkInputActionSelectionCallback` (RTTI), derived from `XBaseActionSelectionCallback`: current and queued action selections (matches, linked) |
| `game/unit_80041874.cpp` | 0x80041874-0x800418E0 | constructor and destructor of a one-byte class without vtable (matches, linked) |
| `game/unit_800418E0.cpp` | 0x800418E0-0x80041AB4 | class `CombatOMaticDamageCallback` (RTTI) derived from `XBaseCombatCallback` (RTTI, emitted here): constructor, destructor, two virtual functions (NonMatching: `vf0C` 86 %, an indirect call through ctr and a dead compare) |
| `game/unit_80042C18.cpp` | 0x80042C18-0x80042C38 | only the static initializer; code and data match but the object is not linked (nothing references it, see the source comment) |
| `game/unit_8004421C.cpp` | 0x8004421C-0x800444FC | class `PlayerInputGestureCallback` (RTTI), derived from `XBaseGestureCallback`; embeds an `AccelerometerMotionRecognizer` (RTTI; Wii Remote gesture recognizer) and sends pool-3 messages; `XBaseGestureCallback`'s RTTI is emitted first here (matches, linked) |
| `game/unit_800444FC.cpp` | 0x800444FC-0x800445E4 | class `NetworkGestureCallback` (RTTI), derived from `XBaseGestureCallback` (functions at 0x8020F498, not split) (matches, linked) |
| `game/unit_80044C58.cpp` | 0x80044C58-0x80044CDC | constructor and destructor of a 0x148-byte class without vtable, built as an array of two by `fn_80040824` (matches, linked) |
| `game/unit_80049628.cpp` | 0x80049628-0x800496DC | class `CharacterPartAttachment` (RTTI), derived from `XCharacterPartAttachment` (functions at 0x8020E9CC, not split); the base class's RTTI object and name are emitted in this unit too (matches, linked) |
| `game/unit_800496DC.cpp` | 0x800496DC-0x800497F4 | class `CompositeCharacterActionSelectionCB` (RTTI), derived from `XBaseActionSelectionCallback` (functions at 0x8020F36C, not split), with a 12-byte selection member (matches, linked) |
| `game/unit_80051EEC.cpp` | 0x80051EEC-0x8005230C | text providers returning strings for the text system (line breaks, current item name and value through the localized text lookup, counters, build string); the strings `""`, `"Not found"` and `"NOT FOUND"` it returns belong to another unit (NonMatching: 10/11, two registers in `fn_8005224C`) |
| `game/unit_8005230C.cpp` | 0x8005230C-0x800523B8 | releases (explicit virtual destructor, then frees `lbl_805FBBC0`) or keeps the object at `lbl_803C2A20 + 0xDC8`; the first call only sets a static flag (matches, linked) |
| `game/unit_800523B8.cpp` | 0x800523B8-0x80052504 | 32-bit key for an id: fixed pairs for ids 30-38 in modes 2/3, otherwise the 40-entry table at 0x803881B4 (matches, linked) |
| `game/unit_8005CA88.cpp` | 0x8005CA88-0x8005CBC4 | class `Entity_Dynamic` (RTTI), derived from `Entity_Game` (matches, linked) |
| `game/unit_80062C80.cpp` | 0x80062C80-0x8006307C | class `Entity_Game` (RTTI), derived from `Entity` (functions at 0x801B4334, not split; vtable at 0x80383D68); its RTTI and Entity's were emitted by an earlier unit (matches, linked) |
| `game/unit_8006CCC4.cpp` | 0x8006CCC4-0x8006CFA4 | class `Entity_Hazard_Prop` (RTTI), derived from `Entity_Prop`; implicit destructor emitted at the end; handles from the global object at 0x8047C760 (matches, linked) |
| `game/unit_8006CFA4.cpp` | 0x8006CFA4-0x8006D2D0 | class `Entity_Challenge_Projector` (RTTI); 5/6 functions match, `vf20` differs by one register choice (not linked) |
| `game/unit_8006F5A0.cpp` | 0x8006F5A0-0x8006F61C | registers the `"count_deaths"` condition callback (type 22) with `fn_801DC91C` (matches, linked) |
| `game/unit_8006F61C.cpp` | 0x8006F61C-0x8006F6BC | class `Entity_Static` (RTTI), derived from `Entity_Game`; its implicit destructor is emitted at 0x800630E0 (matches, linked) |
| `game/unit_8006F6BC.cpp` | 0x8006F6BC-0x8006F794 | class `Entity_Trigger_Script` (RTTI), derived from `Entity_Sensor` (functions at 0x8006D2D0, not split); its RTTI, name and base list are in this unit (matches, linked) |
| `game/unit_80075434.cpp` | 0x80075434-0x800756E0 | static table of 20 clamped values with threshold conditions parsed from `"<"`, `"<="`, `">"`, `">="` (NonMatching: `fn_80075590` 96.7 %, one stack store placed before the loop guard) |
| `game/unit_80076AB8.cpp` | 0x80076AB8-0x80076D38 | class `Entity_SpiritToken` (RTTI), derived from `Entity_Prop`; moves along a path and sends hashed events (matches, linked) |
| `game/unit_80077B50.cpp` | 0x80077B50-0x80077D1C | 17 named callbacks registered by the next unit through `fn_801DC91C`, each working on the object at 0x10 of its argument; 16/17 match (`fn_80077BC8`: one load scheduled before the link register save), not linked. `unk_vec3.h` must be included after the code: the pool has 1.0 before 0.0 |
| `game/unit_8007A394.cpp` | 0x8007A394-0x8007A680 | screen rectangle of a sprite-like object from its image size (constructor, destructor, getters, flag clear, two setup functions) (NonMatching: 6/9, a pair copy and float registers) |
| `game/unit_800836A0.cpp` | 0x800836A0-0x80083A6C | abstract class `Entity_BrainTossTarget` (RTTI; two pure virtual functions), derived from `Entity_Prop` (matches, linked) |
| `game/unit_80086780.cpp` | 0x80086780-0x80086BE8 | class `Entity_BrainTossHand` (RTTI): holds an entity at the hand joint (hash 0x1DC9D499) of its model; 7/8 functions match, one swaps two stack slots (not linked) |
| `game/unit_8008C9F8.cpp` | 0x8008C9F8-0x8008CBB0 | class `Entity_BrazierBowl` (RTTI), derived from `Entity_Prop`; state and value setters send hashed events through `fn_801BA190` (matches, linked) |
| `game/unit_80091D04.cpp` | 0x80091D04-0x8009210C | door of the robot evolution experiment: constructor, destructor, a three-slot registry, a timer, events, and four named state callbacks (`"RobotEvolutionExperiment_DoorOpenning"`...) (matches, linked) |
| `game/unit_800938A4.cpp` | 0x800938A4-0x800940BC | smasher of the robot evolution experiment: constructor, destructor, three-slot registry, impact frame, hazards, seven named callbacks (`"RobotEvolutionExperiment_Smasher..."`) (NonMatching: 13/15, `fn_80093E5C` / `fn_80093F0C` 97.3 %, one load scheduled differently) |
| `game/unit_80098BD8.cpp` | 0x80098BD8-0x80098DA8 | class `Entity_ReactorLeakHammer` (RTTI), derived from the abstract `Entity_ReactorLeakManipulator`; embeds a `DirectionalSpikeRecognizer` (matches, linked). The two functions at 0x80098BC0-0x80098BD8 are left to the previous unit |
| `game/unit_8009B19C.cpp` | 0x8009B19C-0x8009B264 | class `Entity_AstralTile` (RTTI), derived from `ExWorldCurvedTile` and `ExWorldTile` (not split); the RTTI, names and base lists of both base classes are emitted first here (matches, linked) |
| `game/unit_800B5990.cpp` | 0x800B5990-0x800B5E64 | button sequence challenge (`UnkButtonSequence`, real name unknown): up to ten random buttons shown as `A>B>C>Z>` text, pressed in order before the timer runs out (matches, linked) |
| `game/unit_800B7934.cpp` | 0x800B7934-0x800B7A08 | class `Entity_ArmWrestleBolt` (RTTI), derived from `Entity_Prop` (functions at 0x80069028, not split; implicit destructor inlined here) (matches, linked) |
| `game/unit_800B7A08.cpp` | 0x800B7A08-0x800B7E34 | class `Entity_ArmWrestleHand` (RTTI), derived from `Entity_Prop`: attaches to a bone of another entity (matches, linked) |
| `game/unit_800BF654.cpp` | 0x800BF654-0x800BF97C | class `Entity_MysticBrewVortex` (RTTI), derived from `Entity_Prop`: a 0-100 level with hashed events (matches, linked) |
| `game/unit_800C407C.cpp` | 0x800C407C-0x800C42B8 | class `ExWorldWireTile` (RTTI; its RTTI record is emitted by an earlier unit) derived from `ExWorldTile`: constructor, destructor, `vfBC` (finds its wire by bone id), `vfC0`, `vfD0` (wire length) (NonMatching: `vfBC` 87 %, ctr loop) |
| `game/unit_800C42B8.cpp` | 0x800C42B8-0x800C4530 | class `ExWorldWireTrack` (RTTI; its RTTI record is emitted by an earlier unit) derived from `ExWorldTrack` (functions at 0x800C2670, not split; vtable pointer at 0xD4 after a 20-tile ring): forwards to a wire path object at 0xD8 (functions at 0x8010B4AC) (NonMatching: 9/10, `vf0C` 97.8 %, one reload of the ring head) |
| `game/unit_800C5E90.cpp` | 0x800C5E90-0x800C5F84 | class `ExWorldTrackItem` (RTTI), derived from `Entity_Prop`, with four new virtual functions (matches, linked) |
| `game/unit_800D00B4.cpp` | 0x800D00B4-0x800D02C0 | camera influence `CIStitching` (RTTI), derived from `CIDynamic` and the abstract `CameraInfluence`: fixed position and rotation (matches, linked) |
| `game/unit_800DE5B4.cpp` | 0x800DE5B4-0x800DEC28 | class `SphereGraph` (RTTI): spheres joined by edges; finds the capsule closest to a point. The vtable pointer is at 0x10, after the data members, so they come from a base class. 4/5 functions match; the capsule distance `fn_800DE780` does not (75 %, register reuse), so the unit is not linked |
| `game/unit_800DF630.cpp` | 0x800DF630-0x800DFFA4 | camera influence `CIFree` (RTTI), derived from `CameraInfluence` with a new virtual `vf24` (place from a position and a rotation); `vf0C` (quaternion rotation of a velocity) and `fn_800DF7C4` (controller input) are not written yet; `vf24` matches; not linked |
| `game/unit_800DFFA4.cpp` | 0x800DFFA4-0x800E02A8 | camera influence `CIPlayerOffset` (RTTI), derived from `CIDynamic`: camera at an offset from the controller's eye point, optionally pulled in by a ray query (`fn_800DA8F0`); implicit destructor (matches, linked) |
| `game/unit_800DEC28.cpp` | 0x800DEC28-0x800DEFB8 | camera influence `CIDynamic` (RTTI), base of most camera influences: queries its `SphereGraph` member (RTTI) and blends its weight in and out (matches, linked) |
| `game/unit_800DEFB8.cpp` | 0x800DEFB8-0x800DF204 | `CIFixed_DJ1::vf0C` (RTTI class derived from `CameraInfluence`): cosine-eased transition of the camera position and rotation; the vtable is emitted here because the destructor and the other overrides are inline (matches, linked) |
| `game/unit_800DF204.cpp` | 0x800DF204-0x800DF570 | camera influence `CIFixedEyeLook` (RTTI), derived from `CIDynamic`: eases the eye and the look-at point towards two followed objects (matches, linked) |
| `game/unit_800DF570.cpp` | 0x800DF570-0x800DF630 | `CIFixedPlayerTracking_DJ1::vf0C` (RTTI class derived from `CameraInfluence`): fixed position looking at the player's eye (matches, linked) |
| `game/unit_800E391C.cpp` | 0x800E391C-0x800E3CF4 | camera influence `CITrackTarget` (RTTI), derived from `CIDynamic`: turns the camera towards a tracked target; 5/6 functions match (not linked) |
| `game/unit_800E3CF4.cpp` | 0x800E3CF4-0x800E42E8 | camera influence `CITrackTargetDan` (RTTI), derived from `CIDynamic`: follows an entity found from a handle, or one of its joints, with a ray query on the view; only `vf10`, `vf1C` and the static initializer are written (`vf0C` is the key function, so the vtable and the implicit destructor are not emitted yet); not linked |
| `game/unit_800E2EBC.cpp` | 0x800E2EBC-0x800E2FE8 | registers `CIWaypointPath::StaticSetPosition` / `StaticSetOrientation` (real names: the strings they are registered under) and a task dispatch on a type byte; the "CompoundTask" string after its data belongs to a function-local static of an inline used elsewhere (matches, linked) |
| `game/unit_800E2FE8.cpp` | 0x800E2FE8-0x800E3300 | camera influence `CIFixedTrackPlayer` (RTTI), derived from `CIDynamic`: eases the camera between two track points with a cosine curve (matches, linked) |
| `game/unit_800E3300.cpp` | 0x800E3300-0x800E374C | camera influence `CIDirectionGuide` (RTTI), derived from `CIDynamic`: turns the camera yaw towards the camera direction, aligned with the closest SphereGraph capsule, scaled by the speed of the followed object; 4/5 functions match (`vf0C` 98.4 %, float registers), not linked |
| `game/unit_800E374C.cpp` | 0x800E374C-0x800E3834 | camera influence `CIDist` (RTTI), derived from `CIDynamic`: feeds a weighted distance to the camera state; implicit destructor emitted last (matches, linked) |
| `game/unit_800E3834.cpp` | 0x800E3834-0x800E391C | camera influence `CIPitch` (RTTI), same shape for the pitch (matches, linked) |
| `game/unit_800E6228.cpp` | 0x800E6228-0x800E6360 | flag helpers on the entity at `lbl_803E6D00.m0->m8` (flags word at `mA4[1]`, masks from `fn_801B94F8`) (matches, linked) |
| `game/unit_80106884.cpp` | 0x80106884-0x801068A8 | a function-pointer wrapper around `fn_801039C0` (matches, linked) |
| `game/unit_801068A8.cpp` | 0x801068A8-0x80106AA8 | five predicates on an id (ranges and `switch` sets, one returns a hash), registered through `fn_801DC574`-`fn_801DC594` (matches, linked) |
| `game/unit_80106AA8.cpp` | 0x80106AA8-0x8010711C | accessors and debug prints for two global register sets (`lbl_803B53A0` + 0x5C8 / + 0x6C8) and for the register set of an object (at 0x58, default set in `.bss`), plus a copy function (matches, linked) |
| `game/unit_801091B4.cpp` | 0x801091B4-0x801091EC | one function copying `lbl_803C2A20.mDC0` into the `.sbss` global `lbl_805FB218` (the eight empty destructors before it belong to the previous unit, whose static initializer registers them) (matches, linked) |
| `game/unit_8010A634.cpp` | 0x8010A634-0x8010A678 | 80-entry size table and its lookup, used as a function pointer (matches, linked) |
| `game/unit_8010FA48.cpp` | 0x8010FA48-0x8010FD14 | wrappers that start or stop objects through the lazily created global manager `lbl_805FBBAC` (`new` of a 1-byte object built by `fn_80209814`) (matches, linked) |
| `game/unit_8010FD14.cpp` | 0x8010FD14-0x8010FDA4 | class `book` (RTTI): constructor, virtual destructor, vtable with nine pure virtual functions (matches, linked) |
| `game/unit_8010B16C.cpp` | 0x8010B16C-0x8010B4AC | `UnkPath` (real name unknown): polyline of 0x1C-byte segments built from the points of a wire, position at a parameter t (NonMatching: 2/4, `fn_8010B200` 77 %, `fn_8010B2D8` 98.3 %) |
| `game/unit_80116BE8.cpp` | 0x80116BE8-0x80116CA8 | class `section` (RTTI): constructor, virtual destructor, five pure virtual functions, `"%d/%d"` text of two counters (matches, linked) |
| `game/unit_8011A3BC.cpp` | 0x8011A3BC-0x8011A778 | class `InventoryBook` (RTTI) derived from `book`: four sections `Creatures`, `Ingredients`, `IngredientSelection`, `Parts` (RTTI) chosen by name hash (matches, linked; `book` has eight pure virtual functions, not nine) |
| `game/unit_8011D1C0.cpp` | 0x8011D1C0-0x8011D5C4 | class `Journal` (RTTI) derived from `book`: four sections `Badges`, `Statistics`, `QuestLog`, `Recipes` (RTTI) chosen by name hash (matches, linked) |
| `game/unit_80116CA8.cpp` | 0x80116CA8-0x80117090 | vertical progress bar built from three actors found by hash (`UnkProgressBar`, real name unknown) (NonMatching: 3/6, register numbering) |
| `game/unit_80120AAC.cpp` | 0x80120AAC-0x80120D34 | a 16-character text entry buffer allocated on open: typed characters, backspace, space, trailing-space trim, lock flag (matches, linked) |
| `game/unit_80120D34.cpp` | 0x80120D34-0x80120DC0 | binary search of a 32-bit hash in a 4274-entry table sorted by hash (0x8036D370), returning an index 0-4273 (matches, linked) |
| `game/unit_80120DC0.cpp` | 0x80120DC0-0x801210E4 | load progress item `LoadProgressProjectile` (RTTI), derived from the abstract `LoadProgressItem` (RTTI): moves an actor along six markers as the progress goes from 0 to 100; factory `fn_80120FD8` (NonMatching: 3/4, `vf0C` 94.6 %) |
| `game/unit_8012148C.cpp` | 0x8012148C-0x80121508 | empty methods and forwarders of the object at `lbl_803C2A20 + 0x10` (`Unk8012148C`, real name unknown); the forwarded methods follow at 0x8012189C-0x80121A08 (matches, linked) |
| `game/unit_80122A10.cpp` | 0x80122A10-0x80122B5C | reserve / release a slot in the 255-entry active entity id table of the large .bss object at 0x803B53A0 (matches, linked) |
| `game/unit_801420EC.cpp` | 0x801420EC-0x801424B8 | online error handling: polls the network object (`lbl_805FB964`, `vf58`) for an error type and code, shows a message chosen from the error code ranges, state getters (matches, linked; calls MSL `abs`, not the `__abs` intrinsic) |
| `game/unit_801424B8.cpp` | 0x801424B8-0x80142500 | only the global test logger instance (`UnkLogger`, real name unknown: writes `fail.log`); the static initializer builds it and registers its destructor (matches, linked) |
| `game/unit_80142500.cpp` | 0x80142500-0x80142E94 | `main` and the platform layer: a timer class over `OSGetTime`, sleep, frame callbacks, MEM allocators for WPAD, system initialization (OS, SC, DVD, fast cast, heaps, KPAD, AI, AX, reset/power callbacks, error handlers), the fatal error screen and the shutdown mode; the static initializer runs the initialization. The exception name table has "isiexti" (missing comma in the original source) and a null 17th entry (matches, linked) |

RTTI objects (`__RTTI__*`) and their name strings are emitted as weak data in every
unit that needs them (for example a derived class's base list). Our object may then
hold a copy of a base class's RTTI that the original placed elsewhere; objdiff reports
lower data match for it, but the linker keeps the first copy and the DOL still matches
(seen with `BugCachingChallenge::Bug` in `unit_8002B4D0`). When the copy in a unit is
the first one in link order, it is the one in the DOL (`XCharacterPartAttachment` in
`unit_80049628`).

A unit does not always end at its static initializer: MWCC emits inline functions
used by the unit after it. `fn_80062C7C` (an empty inline constructor passed to
`__construct_array` by the static initializer just before it) belongs to the unit
that ends at 0x80062C80. Inline virtual functions are likewise emitted as weak
copies in the first unit that needs them (the Entity vtable points into eight other
units).

objdiff may report 100 % for a function that loads the wrong float constant from
.sdata2 (seen in `unit_80076AB8`: 1.0 instead of 0.0) or that calls the wrong function
(seen in `unit_80098BD8`: the base class constructor instead of the derived one). Only linking the unit and the
`main.dol` check prove a match, so every matching game unit is linked.

MWCC orders the float constants of a unit's .sdata2 by the first textual appearance
of each literal, even in code it later removes (seen in `unit_800DF204`: the DOL has
0.0 first, which the source reproduces by initializing a local to 0.0 at the start of
the function).

Source shapes found while matching `unit_800DEC28` (MWCC GC 3.0a5.2):
- a test against several constants (`type != 4 && type != 5 && ...`) used as a value
  compiles to a bit test (`subi`, `cmplwi`, `slw`, `andi.`); the same test in an `if`
  or a `switch` compiles to a chain of comparisons;
- `return f(x) == false;` keeps the result of an inline `bool` function and inverts it
  with `cntlzw`/`srwi`, while `return !f(x);` folds the negation into the branches;
- a clamp written as a conditional expression and one written with `if`/`else` and a
  result variable give the same instructions but different float registers.

A float comparison stored in a local `bool` and then tested compiles to
`fcmpo`/`mfcr`/`srwi.` (or `extrwi.`) plus a branch; the same comparison written
directly in the `if` compiles to a plain conditional branch (seen in `unit_800DE5B4`).

### Game / SDK boundary (0x80232AF8)

- 0x80232AF8 is `PPCMfmsr`, the first SDK function recognized by dtk; the previous
  function `fn_802326C0` is unnamed.
- Calls from code below 0x80232AF8 into the SDK region: 2368.
  Calls from the SDK region into code below 0x80232AF8 (excluding `.init`): 37, all to
  `fn_801D426C` (32), `fn_801D40E0` (3), `fn_801D4340` (1) and `fn_800063F4` (1).
  Most callers are in the HBM area (0x802C5xxx–0x802C7xxx). The first two look like
  the C++ allocation operators defined on the game side (hypothesis, not named).
- Whether some code below 0x80232AF8 is third-party middleware (the disc has `fmv`
  videos and `Sounds`) is **unknown**.

### SDK libraries located by their version string

| Library | Version string | Registered by |
|---|---|---|
| EXI | 0x803972B8 | `EXIInit` |
| SI | 0x80397300 | `SIInit` |
| VI | 0x80397420 | `fn_80244408` |
| GX | 0x80397F20 | `GXInit` |
| DVD | 0x80398798 | `DVDInit` |
| AI | 0x8039A0A0 | `fn_802577E4` |
| AX | 0x8039A0E8 | `fn_80257C34` |
| DSP | 0x8039F808 | `fn_80264FD8` |
| SC | 0x8039FC28 | `SCInit` |
| WPAD | 0x8039FEF0 | `fn_80271358` |
| KPAD | 0x803A1AF0 | `fn_8027FC30` |
| HBM | 0x803A9498 | referenced by `fn_802C8FC8`, `fn_802C9ABC`, `fn_802CA3A0` |
| DWC | 0x803AD138 | referenced by `fn_802E49B8` |
| NCD | 0x803B3DA8 | `fn_8032149C` |
| NWC24 | 0x803B3E98 | `fn_80322658` |
| SO | 0x803B3FA0 | `fn_80323644` |
| SOCKET | 0x803B4180 | `fn_80324778` |
| NHTTP | 0x803B4420 | `fn_8032BE48` |
| REX-PPC (RevoEX) | 0x803B4720 | `fn_8032D5B8` |
| SSL | 0x803B4920 | `fn_8032E18C` |

(OS and NAND strings are not referenced through a dtk symbol; not located yet.)

## Data regions

Each data object was classified by the region of the functions that reference it.
In every section the order is game → SDK → runtime → MSL ("unknown" column = MSL math)
→ TRK, which matches the link order seen in `.text`.

| Section | Game | SDK | Runtime | MSL | Unknown (after MSL) | TRK |
|---|---|---|---|---|---|---|
| `.rodata` | 0x80345FC0–0x80349FF8 | 0x8034A8A0–0x803521A4 | 0x80352208–0x80352290 | 0x80352290–0x803528F8 | 0x80352F30–0x803531B0 | 0x80353268–0x8035334C |
| `.data` | 0x80353460–0x80389498 | 0x803894F0–0x803B4968 | — | 0x803B4AB8–0x803B51CC | — | 0x803B5210–0x803B5328 |
| `.bss` | 0x803B5380–0x8055CAB0 | 0x8055CB60–0x805F2920 | 0x805F5940 | 0x805F5950–0x805F5CA8 | — | 0x805F5CD0–0x805F8170 |
| `.sdata` | 0x805F81A0–0x805FA0F8 | 0x805FA100–0x805FAE20 | `fragmentID` 0x805FAE28 | 0x805FAE38–0x805FAE50 | — | — |
| `.sbss` | 0x805FAE60–0x805FBBE0 | 0x805FBBE8–0x805FC56C | 0x805FC570 | 0x805FC578–0x805FC594 | 0x805FC588–0x805FC5A0 | 0x805FC5A8–0x805FC5B0 |
| `.sdata2` | 0x805FC5E0–0x80600654 | 0x80600658–0x80600B30 | — | 0x80600B38–0x80600BC8 | 0x80600BD0–0x806010D8 | — |
| `.sbss2` | 0x806010E0–0x80601104 | 0x80601110 | — | — | — | — |

Ranges are the first and last **referenced** object of each class; unreferenced
objects in between are not classified. Exceptions:

- `.data` 0x803982F0–0x80398458: a few objects referenced by both game and SDK code,
  interleaved inside the SDK range. Not explained yet.
- `.data` 0x803B4978 and 0x803B4DD0: referenced by several regions.
- `.sdata` 0x805FAE58 and `.sbss` 0x805FC5B8–0x805FC5C8: referenced by the unknown
  block after TRK.

## Runtime.PPCEABI.H units

TU names and order are **hypotheses taken from other decompilation projects** using
the same Metrowerks runtime, both CC0-1.0:
[SMGCommunity/Petari](https://github.com/SMGCommunity/Petari) (`config/RMGP01/splits.txt`)
and [yannicksuter/mscharged-decomp](https://github.com/yannicksuter/mscharged-decomp)
(`config/R4QE01/splits.txt`), read on 2026-10-12. Only split boundaries were used, no
code. Each unit was checked against this DOL by function sizes and order, and by the
data its functions reference:

| Unit | `.text` | Other sections | Check against this DOL |
|---|---|---|---|
| `__mem.c` | 0x8032EEE4–0x8032EF00 | `.init` 0x80004338–0x8000446C | `strlen` 0x1C; `memcpy`/`__fill_mem`/`memset` same layout as Petari |
| `__va_arg.c` | 0x8032EF00–0x8032EFC8 | — | `__va_arg` 0xC8 |
| `global_destructor_chain.c` | 0x8032EFC8–0x8032F028 | `.sbss` 0x805FC570–0x805FC578 | 0x18 + 0x48; `__global_destructor_chain` |
| `NMWException.cpp` | 0x8032F028–0x8032F3D8 | `extab` 0x800064E0–0x80006528, `extabindex` 0x80006780–0x800067BC | 5 functions, sizes identical to Petari (0x104, 0xBC, 0xF8, 0x78, 0x80); extab 0x48 / extabindex 0x3C as in Petari |
| `ptmf.c` | 0x8032F3D8–0x8032F400 | — | only one function (0x28 = `__ptmf_scall` size; it adjusts `this` and calls through `r12`). The other ptmf functions are absent (dead-stripped, hypothesis) |
| `runtime.c` | 0x8032F400–0x8032FCAC | `.rodata` 0x80352208–0x80352220 | first function 0x5C (`__cvt_fp2unsigned` size), `__save_fpr`…`__mod2u` named by dtk; `.rodata` 0x18 as in Petari. Two extra functions (0xB0, 0xA0) not in Petari |
| `__init_cpp_exceptions.cpp` | 0x8032FCAC–0x8032FD1C | `.ctors$10`, `.dtors$10`, `.dtors$15`, `.sdata` `fragmentID` | required by the GC 3.0a5.2 linker |
| `Gecko_ExceptionPPC.cpp` | 0x8032FD1C–0x8032FD80 | `.bss` 0x805F5940–0x805F5950 | `__register_fragment` + `__unregister_fragment` = 0x64; `fragmentinfo` |
| `GCN_mem_alloc.c` | 0x8032FD80–0x8032FEF0 | `.rodata` 0x80352220–0x80352290 | two 0xB8 functions (0x170 total, as in mscharged); `.rodata` 0x70 as in Petari; strings `GCN_Mem_Alloc.c` and "Metrowerks CW runtime library initializing default heap" in this DOL |

All units are `NonMatching` (nothing decompiled yet); the build still matches.

## MetroTRK units

Names, order and sections come from
[yannicksuter/mscharged-decomp](https://github.com/yannicksuter/mscharged-decomp)
`config/R4QE01/splits.txt` (CC0-1.0); Petari (`config/RMGP01`) and
[zeldaret/tp](https://github.com/zeldaret/tp) (`config/RZDP01`, CC0-1.0) have the same
unit list. Read on 2026-10-12; only split boundaries were used, no code.

Check against this DOL: function sizes were aligned with mscharged (sequence alignment
of size lists). 151 of 152 functions match in size and order, from `mainloop.c` to
`MWCriticalSection_gc.c`; every unit's `.text` size is identical to the reference,
except:

- `mslsupp.c`: 0x5BC instead of 0x640; the missing 0x84 is `__close_console`, absent
  here (dead-stripped, hypothesis);
- `InitMetroTRK`: dtk sized it 0x94 instead of 0x98; the missing 4 bytes are its final
  `blr` (0x80344230 = `4E800020`), left as a gap by the analysis. Corrected in
  `symbols.txt` (not a version difference).

Data was assigned from the objects each unit's functions reference, and every range
size was compared with the reference (e.g. `dispatch.c` `.data` 0x6C, `msghndlr.c` two
0x1C jump tables, `targimpl.c` `.rodata` 0x88, `cc_gdev.c` `.bss` 0x500 + 0x1C,
`dolphin_trk_glue.c` `.rodata` 0x108 starting at `EndofProgramInstruction`).
`.init` parts: `mem_TRK.c` (`TRK_memset`/`TRK_memcpy` sizes 0x30/0x24),
`__exception.s` (`gTRKInterruptVectorTable` … `gTRKInterruptVectorTableEnd`,
0x1F34 as in the reference), `dolphin_trk.c` (`fn_800063F4`, 0x2C).
Small differences: `targimpl.c` `.bss` is 0x580 here (0x57C in the reference), and a
few ranges end on dtk symbol sizes (padding included) where dtk sized a symbol larger
than the reference.

## MSL_C units

References (all CC0-1.0, split boundaries only, no code):
[zeldaret/tp](https://github.com/zeldaret/tp) `config/RZDP01` (primary: the most mature
project), [SMGCommunity/Petari](https://github.com/SMGCommunity/Petari) `config/RMGP01`
and [yannicksuter/mscharged-decomp](https://github.com/yannicksuter/mscharged-decomp)
`config/R4QE01`, read on 2026-10-12.

This MSL is a different version from each reference, so units were placed with a
**three-way view**: function sizes aligned against each reference separately, then each
of our functions attributed to the unit that at least one reference aligns to it.
Where alignment by size alone was ambiguous, the code itself was used:

- **`w_*` wrappers** (0x8033F1F4–0x8033F210, 4 bytes each) are single `b` instructions
  to their `e_*` function: `e_acos`, `e_asin`, `e_atan2`, `e_fmod`, `e_log`, `e_log10`,
  `e_pow` (sizes equal to the references). This corrected an earlier size-only split:
  0x8033F1F8 is `w_asin.c`, not `w_atan2.c`. `w_log.c` is named by the `w_<name>.c`
  convention; no reference has it (hypothesis).
- **`extras.c`**: the three 4-byte functions inside it (0x8033B1B8, 0x8033B1BC,
  0x8033B310) branch to `__msl_strnicmp` / `__msl_itoa`, so they are `strnicmp`-style
  and `itoa` aliases, not math wrappers (TP and Petari align them as `w_*` by size).
- **`arith.c`** starts at 0x80332ED0: both 0x10 functions there have the `abs` code
  (`srawi`/`xor`/`subf`), i.e. `abs` and `labs`, followed by `div`.
- **`alloc.c`** includes `fn_80330F50` (0x318): it uses the allocator pools and calls
  `allocate_from_fixed_pools` / `allocate_from_var_pools`.
- **`uart_console_io_gcn.c`** includes `fn_8033B08C`, which sits between two functions
  of that unit (units are contiguous).

Data: `extab`/`extabindex` ranges come from this DOL (each entry belongs to a function
of the unit). Other sections were kept when their size equals at least one reference,
e.g. `ansi_files.c` `.bss` 0x300 (three 0x100 buffers), `scanf.c` `.data` 0x120 (three
jump tables), `strtoul.c` `.data` 0x88, `signal.c` `.bss` 0x20,
`abort_exit_ppc_eabi.c` `.sbss` 0x8 (`__stdio_exit` included), `e_log.c` /
`e_log10.c` `.sbss` 0x8 each, `e_pow.c` `.rodata` 0x30 + `.sdata2` 0x110.

59 units are split, covering 0x8032FEF0–0x8033F460 except three gaps left as `auto_*`:

| Gap | Content | Why not split |
|---|---|---|
| 0x80333B34–0x80333B60 | one 0x2C function calling `__close_console` | no reference has it |
| 0x8033ABF0–0x8033AE98 | three functions doing date arithmetic, calling an OS function | time functions, not in any reference |
| 0x8033AFB4–0x8033AFBC | `li r3, 0; blr`, called by the date code | owner unknown |

Data not assigned yet (owner unclear, shared between units): `printf.c` data (the
references give `.rodata` 0x30, `.data` 0x250, `.sdata` 0x8, `.sdata2` 0x8), the 0x48
table at 0x803B4DD0 (used by `file_io.c`, `mbstring.c`, `scanf.c`, `strtoul.c`,
`extras.c`), `errno`-like 0x805FC580 (used by math and `strtoul.c`) and
0x805FAE48 / 0x805FAE4C.

## RVL_SDK units

References: TP (`config/RZDP01`, `revolution/*` and `homebuttonLib`), Petari
(`config/RMGP01`, `RVL_SDK/*`) and mscharged-decomp (`config/R4QE01`, `RVL_SDK/*`, the
only one with the network libraries DWC/GameSpy/NHTTP/SSL). All CC0-1.0; split
boundaries only, no code. Read on 2026-10-12.

Method (automated, scripts kept out of the repository):

1. Each of our 3634 functions in 0x80232AF8-0x8032EEE4 gets, per reference, the unit of
   the reference function with the **same name and size** (254 functions are named by
   dtk), otherwise the unit given by **size alignment**, counting only aligned runs of
   at least 3 functions. Priority on conflict: name before size, then TP > Petari >
   mscharged (5 functions disagreed). 2312 functions got a label.
2. Unlabeled functions are absorbed only when both neighbours carry the same unit.
3. A unit is kept only when it is **anchored**: its label appears in a single run, and
   the functions right before and after it are labeled with other units. Units touching
   an unlabeled gap could extend into it and were not split.
4. Data: objects referenced **only** by the unit's functions, forming a contiguous block
   with nothing else inside, kept only when the block size equals the same unit's
   section size in at least one reference. Two ranges were then removed because dtk
   reported a misaligned section start (`bte_main.c` `.sbss`, `vi3in1.c` `.sbss`): an
   unreferenced object probably belongs at their start, which is not proven.

5. Second pass: a unit adjacent to an unlabeled gap was also split when its `.text`
   size is **exactly** the size of the same unit in a reference (the unit is then
   complete and cannot extend into the gap). 23 units: `OSRtc.c`, `OSSync.c`,
   `OSIpc.c`, `OSPlayRecord.c`, `OSPlayTime.c`, `__ppc_eabi_init.cpp`, `GXInit.c`,
   `AXCL.c`, `AXVPB.c`, `AXFXHooks.c`, `dsp_task.c`, `NANDCheck.c`, `NANDLogging.c`,
   `scsystem.c`, `scapi_prdinfo.c`, `arc.c`, `euart.c`, `TPL.c`, `dwc_common.c`,
   `hashtable.c`, `gpiInfo.c`, `gpiUnique.c`, `gbucket.c`. `arc.c` and `TPL.c` are
   also confirmed by the source file name strings `arc.c` / `TPL.c` used by a function
   inside each range. Data ranges ending on an unaligned address were dropped (two in
   `TPL.c`), since the following split would start misaligned.
   Correction: the 0x40 block taken as `AXVPB.c` is in fact `AXProf.c`
   (`__AXGetCurrentProfile`); `AXVPB.c` is 0x8025A0B0-0x8025B3F4 (see
   `notes/PROVENANCE.md`, third pass). A size match alone is not enough.

Data check: the reference scan was rerun over every disassembly file (including the
per-unit subfolders created by earlier splits); all 162 data sections of the first pass
were confirmed.

Result: **177 units**, 0x82EEC of the 0xFC3EC bytes of SDK code (about 52 %), with 177
data sections. For 148 units the `.text` size is identical to the same unit in a
reference; the other 29 differ (version differences) but are anchored by their
neighbours. No overlap, no dtk warning, `main.dol` matches. Unit paths follow the
references under `RVL_SDK/` (e.g. `RVL_SDK/os/OSAlarm.c`,
`RVL_SDK/gamespy/serverbrowsing/sb_server.c`).

Source file name strings that locate one function of a unit not split yet (extent
unknown): `OSStateTM.c` (0x8023E080-0x8023E330), `OSLaunch.c` (0x8023FC84),
`dwci_nd.c` (0x80320B8C), `ncdsystem.c` (0x8032149C), `NHTTP_bgnend.c` (0x80325ED4),
`NHTTP_os_RVL.c` (0x8032683C), `d_nhttp.c` (0x8032C2B0).

Not split yet (`auto_*`, 0x4a83c bytes in 4 regions of the RVL_SDK range, recomputed 2026-10-14): mostly AX/AXFX, HBM and its
`nw4hbm` layout code, DWC, GameSpy and NCD/NWC24/SO/NHTTP/SSL, whose versions differ
from every reference. Units next to these regions whose size does not match a
reference (e.g. `OSStateTM.c`, `mtx.c`, `GXFrameBuf.c`, `ai.c`, `mem_expHeap.c`,
`HBMGUIManager.cpp`, `dwc_main.c`) stay unsplit.

0x802608AC-0x80265690 was inside the `mem_list.c` split until 2026-10-14 (the unit is
only 0x114 bytes). It starts with the MIX library, now split as `mix/mix.c`
(0x802608AC-0x80263A38) and `mix/remote.c` (0x80263A38-0x80263F18), both matching.
0x80263F18-0x80264F90 (calls `AXSetVoice*`) is not identified yet; the DSP functions
after it are split as `dsp.c`, `dsp_debug.c` and `dsp_task.c`.

| Range | Size |
|---|---|
| 0x8025D2E0-0x8025F934 | 0x2654 |
| 0x80263F18-0x80264F90 | 0x1078 |
| 0x802B3E88-0x802E5EEC | 0x32064 |
| 0x802E6298-0x802FB3A4 | 0x1510c |

## Splits in `splits.txt`

| Unit | Status |
|---|---|
| `RVL_SDK/*` (177 units) | Split, NonMatching (about 52 % of SDK code) |
| `Runtime.PPCEABI.H/*` (9 units) | Split, NonMatching |
| `MSL_C/*` (59 units) | Split, NonMatching (3 small gaps, see above) |
| `MetroTRK/*` (29 units) | Split, NonMatching |
| Everything else | dtk `auto_*` objects (gap-based, not TU-accurate) |
