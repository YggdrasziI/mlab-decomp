#ifndef GAME_BUZZSAWEXPERIMENT_H
#define GAME_BUZZSAWEXPERIMENT_H

// The buzz saw experiment (class names from RTTI: "BuzzSawExperiment", "Experiment",
// "MiniGame", "BuzzsawTrack", "BuzzsawTileSetPool", "ExWorldTileSetPool"; game unit
// at 0x8009F66C-0x800A34C0). Method names unknown (vfXX = vtable offset).

#include "msl_tree.h"
#include "game/ExWorldTrack.h"

// Base of the mini-games (functions at 0x80074CC8, vtable at 0x8035A89C). Its first
// word comes before the vtable pointer.
class MiniGame {
public:
    /* 0x00 */ u32 m0; // declared before the virtual functions: the vtable pointer follows it

    MiniGame(int arg);
    virtual ~MiniGame();
    virtual void vf0C() = 0;
    virtual void vf10() = 0;
    virtual void vf14(f32 dt) = 0;
    virtual void vf18() = 0;
    virtual void vf1C() = 0;
    virtual void vf20(f32 dt);
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C() = 0;
    virtual void vf30() = 0;
    virtual void vf34(f32 dt) = 0;
    virtual void vf38() = 0;
    virtual void vf3C() = 0;
    virtual void vf40(f32 dt) = 0;
    virtual void vf44();
    virtual void vf48();
    virtual void vf4C();
    virtual void vf50();

    /* 0x04 */ // vtable
    /* 0x08 */ int m8;
    /* 0x0C */ int mC;
    /* 0x10 */ u8 unk10[0x30 - 0x10];
    /* 0x30 */ u32 m30;
    /* 0x34 */ u32 m34;
    /* 0x38 */ u8 unk38[0x68 - 0x38];
};

// Base of the experiments (functions at 0x8006FA74, vtable at 0x8035A760).
class Experiment : public MiniGame {
public:
    Experiment(int arg);
    virtual ~Experiment();
    virtual void vf0C();
    virtual void vf14(f32 dt);
    virtual void vf20(f32 dt);
    virtual void vf44();
    virtual void vf54();
    virtual void vf58(f32 dt) = 0;
    virtual void vf5C() = 0;
    virtual void vf60();
    virtual f32 vf64() = 0;
    virtual f32 vf68();
    virtual f32 vf6C();

    /* 0x68 */ int m68;
    /* 0x6C */ f32 m6C;
    /* 0x70 */ f32 m70;
    /* 0x74 */ u8 unk74[0x7C - 0x74];
    /* 0x7C */ f32 m7C;
    /* 0x80 */ f32 m80;
    /* 0x84 */ f32 m84;
    /* 0x88 */ f32 m88;
};

// A list of tile hashes (0x8 bytes, real name unknown; constructor at 0x800C2174).
struct UnkTileSet {
    UnkTileSet(const u32* hashes, int count);

    /* 0x0 */ const u32* mHashes;
    /* 0x4 */ int mCount;
};

// Base of the tile set pools (functions at 0x800C20FC, vtable at 0x8035ED08).
class ExWorldTileSetPool {
public:
    // Declared before the virtual functions: the vtable pointer follows them.
    /* 0x0 */ u32 m0;
    /* 0x4 */ int m4;
    /* 0x8 */ int m8;

    ExWorldTileSetPool();
    virtual ~ExWorldTileSetPool();
    virtual void vf0C(UnkTileSet* sets, int count);
    virtual void vf10();
    virtual void vf14() = 0;
    virtual void vf18() = 0;
    virtual void vf1C() = 0;
    virtual bool vf20(u32 hash) = 0;

    /* 0xC */ // vtable
};

class BuzzsawTileSetPool : public ExWorldTileSetPool {
public:
    BuzzsawTileSetPool() {
        m10 = 0;
        m14 = 0;
    }
    virtual ~BuzzsawTileSetPool() {}
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual bool vf20(u32 hash);

    /* 0x10 */ int m10;
    /* 0x14 */ int m14;
};

class BuzzsawTrack : public ExWorldWireTrack {
public:
    BuzzsawTrack() { mD0 = 1; }
    virtual ~BuzzsawTrack() {}
    virtual bool vf2C(ExWorldTile* tile);
    virtual bool vf30(ExWorldTile* tile);

    /* 0xBE4 */ u8 unkBE4[0xBE8 - 0xBE4];
};

// Comparators of the maps below (real types unknown).
struct UnkSawLess {};
struct UnkSawPoolLess {};
struct UnkSawGroupLess {};

typedef std::pair<const u32, Entity*> UnkSawEntry;

// Saws of one type (real type unknown; the DOL has its own tree functions, apart
// from those of UnkSawMultimap).
struct UnkSawGroup {
    std::__tree<UnkSawEntry, UnkSawGroupLess, std::allocator<UnkSawEntry>, 1> tree_;
};

typedef std::pair<const u32, UnkSawGroup> UnkSawPoolEntry;

// Pool of saws by type (real type unknown; functions at 0x800A3CAC-0x800A4EE4).
struct UnkSawPool {
    struct Map {
        std::__tree<UnkSawPoolEntry, UnkSawPoolLess, std::allocator<UnkSawPoolEntry>, 0> tree_;
    };

    Map mMap;
};

// Tiles by hash (real type unknown; searched with equal_range by Entity_BuzzsawTile).
struct UnkSawMultimap {
    typedef std::__tree<UnkSawEntry, UnkSawLess, std::allocator<UnkSawEntry>, 1> tree_type;

    void insert(const UnkSawEntry& x) { tree_.insert_multi(x); }
    std::pair<tree_type::iterator, tree_type::iterator> equal_range(const u32& k) { return tree_.equal_range(k); }

    std::__tree<UnkSawEntry, UnkSawLess, std::allocator<UnkSawEntry>, 1> tree_;
};

class BuzzSawExperiment : public Experiment {
public:
    BuzzSawExperiment();
    virtual ~BuzzSawExperiment();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14(f32 dt);
    virtual void vf18();
    virtual void vf1C();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34(f32 dt);
    virtual void vf38();
    virtual void vf3C();
    virtual void vf40(f32 dt);
    virtual void vf54();
    virtual void vf58(f32 dt);
    virtual void vf5C();
    virtual void vf60();
    virtual f32 vf64();

    // The DOL dispatches these calls through the vtable of mTrack.
    ExWorldTrack* GetTrack() { return &mTrack; }
    UnkVec3 TrackPos() {
        return GetTrack()->vf24(GetTrack()->vf20());
    }

    void fn_800A0BAC(f32 dt);
    void fn_800A0FB4();
    void fn_800A11F4();
    void fn_800A1758();
    void fn_800A17A0();
    void fn_800A17F8();
    void MoveCameraEntity();
    void PlaceSaw();

    /* 0x08C */ BuzzsawTrack mTrack;
    /* 0xC74 */ BuzzsawTileSetPool mPool;
    /* 0xC8C */ Entity* mC8C; // the saw robot (functions at 0x800A6538)
    /* 0xC90 */ f32 mC90;
    /* 0xC94 */ UnkVec3 mC94;
    /* 0xCA0 */ UnkVec3 mCA0;
    /* 0xCAC */ Entity* mCAC;
    /* 0xCB0 */ UnkVec3 mCB0;
    /* 0xCBC */ UnkVec3 mCBC;
    /* 0xCC8 */ u8 mCC8;
    /* 0xCCC */ f32 mCCC;
    /* 0xCD0 */ f32 mCD0;
    /* 0xCD4 */ Entity* mCD4;
    /* 0xCD8 */ UnkSawPool mSawPool;
    /* 0xCE8 */ UnkSawMultimap mTiles;
    /* 0xCF4 */ f32 mCF4;
    /* 0xCF8 */ int mCF8;
    /* 0xCFC */ int mCFC;
    /* 0xD00 */ f32 mD00;
};

// The running experiment and its two maps (defined in the unit at 0x8009F66C).
extern BuzzSawExperiment* lbl_805FB158;
extern UnkSawPool* lbl_805FB160;
extern UnkSawMultimap* lbl_805FB164;

#endif // GAME_BUZZSAWEXPERIMENT_H
