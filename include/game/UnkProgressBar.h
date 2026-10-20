#ifndef GAME_UNKPROGRESSBAR_H
#define GAME_UNKPROGRESSBAR_H

// Vertical progress bar of the book sections (game unit 0x80116CA8; real name of the
// class unknown).

#include "game/unk_vec3.h"

class UnkProgressBar {
public:
    UnkProgressBar();
    ~UnkProgressBar();
    void fn_80116D40(u32 topHash, u32 bottomHash);
    void fn_80116E54(int step, int total);
    void fn_80116EF0(int value);
    void fn_80116F08();

    /* 0x00 */ UnkVec3 mTop;
    /* 0x0C */ UnkVec3 mBottom;
    /* 0x18 */ u32 mBarHash;
    /* 0x1C */ int mMax;
    /* 0x20 */ int mValue;
    /* 0x24 */ f32 mBarHeight;
    /* 0x28 */ f32 mHeight;
};

#endif // GAME_UNKPROGRESSBAR_H
