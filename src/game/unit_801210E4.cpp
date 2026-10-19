// Game unit at 0x801210E4-0x8012148C (real file name unknown): a full-screen quad
// drawn by the top-level game flow (object at lbl_805FAEB0) with its render state
// set up. Bounds come from the static initializers of this unit and the previous one.

#include "game/unk_vec3_rvl.mch"
#include <revolution/gx/GXHardware.h>
#include "game/Unk8012148C.h"
#include "game/unk_vec3.h"

extern "C" void fn_801EF980(u32 size);
extern "C" void fn_801EFA18();
extern "C" void fn_80150148();
extern "C" void fn_801D6780();
extern "C" void fn_801D66C4(f32 a, f32 b);
extern "C" void fn_801514F0();
extern "C" void fn_801542DC();
extern "C" void fn_80154318();
extern "C" void fn_8015B7D4();
extern "C" void fn_801534F0(int a, int b, int c, f32 d);
extern "C" void fn_8016FB20(void* obj);
extern "C" void fn_80170044(void* obj);
extern "C" void fn_800526D0(void* obj);
extern "C" void fn_800557B4(void* mtx);
extern "C" void fn_8005535C(void* obj, void* mtx);
extern "C" void fn_80155724(void* obj);
extern "C" void* fn_80044634(u32 size);
extern "C" void fn_80151644(int primitive, int count, void* vertices);

// Rows of a 4x3 matrix (real type unknown).
struct UnkMtx43 {
    /* 0x00 */ UnkVec3 mRow[4];
};

extern "C" void fn_801D6814(void* obj, UnkMtx43 mtx);

// Object whose field at 0x188 is cleared here (real type unknown).
struct UnkRenderer {
    /* 0x000 */ u8 unk0[0x188];
    /* 0x188 */ int m188;
};

struct UnkContext;

extern UnkRenderer* lbl_805FB75C;
extern int lbl_805F9614;
extern u8 lbl_805FB50C;
extern UnkContext lbl_8047C640;
extern "C" void fn_800446BC(UnkContext* context, void* texture);

// Vertex of the quad (real type unknown). These setters are defined after their
// caller, so they are not inlined.
struct UnkQuadVertex {
    void fn_80121434(f32 x, f32 y);
    void fn_80121440(u8 r, u8 g, u8 b, u8 a);
    void fn_8012145C(f32 x, f32 y, f32 z);

    /* 0x00 */ f32 mX;
    /* 0x04 */ f32 mY;
    /* 0x08 */ u32 mColor;
    /* 0x0C */ f32 mU;
    /* 0x10 */ f32 mV;
    /* 0x14 */ f32 mW;
};

// Pending flag (real name unknown); no code in the DOL sets it, this unit only
// tests and clears it.
bool lbl_805FB2C0;

// Owner of the quad's texture (real type unknown).
class Unk801210E4 {
public:
    void fn_801210E4();
    void fn_801210F8();
    void fn_80121254();

    /* 0x0 */ void* m0;
};

void Unk801210E4::fn_801210E4() {
    m0 = 0;
    fn_801EF980(0x20800);
}

void Unk801210E4::fn_801210F8() {
    fn_801EFA18();
    m0 = 0;
}

// NonMatching (98.6 %): the DOL builds four 12-byte vectors at sp+0x8..0x34 in row
// order and copies each one into the matrix at sp+0x38 right after it; this version
// keeps the matrix at sp+0x8 and its copy at sp+0x38. No tried spelling (vector
// temporaries, matrix constructors, inline identity, by-value or by-reference
// parameter) gives the DOL's stack order.
void fn_8012112C() {
    fn_80150148();
    fn_801D6780();
    fn_801D66C4(0.5f, 1000.0f);
    lbl_805FB75C->m188 = 0;
    lbl_805F9614 = 0;
    UnkMtx43 mtx;
    mtx.mRow[0].x = 1.0f;
    mtx.mRow[0].y = 0.0f;
    mtx.mRow[0].z = 0.0f;
    mtx.mRow[1].x = 0.0f;
    mtx.mRow[1].y = 1.0f;
    mtx.mRow[1].z = 0.0f;
    mtx.mRow[2].x = 0.0f;
    mtx.mRow[2].y = 0.0f;
    mtx.mRow[2].z = 1.0f;
    mtx.mRow[3].x = 0.0f;
    mtx.mRow[3].y = 0.0f;
    mtx.mRow[3].z = 0.0f;
    fn_801D6814(&lbl_8047C640, mtx);
    fn_801514F0();
    lbl_805FB50C &= ~8;
    GX_BP_LOAD_REG(0xFE00C000);
    GX_BP_LOAD_REG(0x00000000);
    fn_801542DC();
    fn_80154318();
    GX_BP_LOAD_REG(0xFE000FE0);
    GX_BP_LOAD_REG(0x410004A0);
}

void Unk801210E4::fn_80121254() {
    if (m0 != 0) {
        if (lbl_805FB2C0) {
            fn_8015B7D4();
            lbl_805FB2C0 = false;
        }
        fn_801534F0(5, 0, 0, 0.0f);
        fn_8016FB20(lbl_805FB75C);
        fn_8012112C();
        u8 camera[0x38];
        fn_800526D0(camera);
        u8 mtx[0x30];
        fn_800557B4(mtx);
        fn_8005535C(camera, mtx);
        ((Unk8012148C*)camera)->fn_80121490(0.0f, 0.0f, 0.0f);
        fn_80155724(camera);
        UnkQuadVertex* quad = (UnkQuadVertex*)fn_80044634(0x60);
        quad[0].fn_80121434(0.0f, 1.0f);
        quad[0].fn_80121440(0xFF, 0xFF, 0xFF, 0xFF);
        quad[0].fn_8012145C(-0.6666667f, -0.5f, 5.0f);
        quad[1].fn_80121434(1.0f, 1.0f);
        quad[1].fn_80121440(0xFF, 0xFF, 0xFF, 0xFF);
        quad[1].fn_8012145C(0.6666667f, -0.5f, 5.0f);
        quad[2].fn_80121434(0.0f, 0.0f);
        quad[2].fn_80121440(0xFF, 0xFF, 0xFF, 0xFF);
        quad[2].fn_8012145C(-0.6666667f, 0.5f, 5.0f);
        quad[3].fn_80121434(1.0f, 0.0f);
        quad[3].fn_80121440(0xFF, 0xFF, 0xFF, 0xFF);
        quad[3].fn_8012145C(0.6666667f, 0.5f, 5.0f);
        fn_800446BC(&lbl_8047C640, m0);
        fn_80151644(0x98, 4, quad);
        fn_800446BC(&lbl_8047C640, 0);
        fn_80170044(lbl_805FB75C);
    }
}

void UnkQuadVertex::fn_80121434(f32 x, f32 y) {
    mX = x;
    mY = y;
}

void UnkQuadVertex::fn_80121440(u8 r, u8 g, u8 b, u8 a) {
    mColor = (r << 24) | (g << 16) | (b << 8) | a;
}

void UnkQuadVertex::fn_8012145C(f32 x, f32 y, f32 z) {
    mU = x;
    mV = y;
    mW = z;
}
