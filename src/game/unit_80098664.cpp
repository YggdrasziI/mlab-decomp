// Game unit at 0x80098664-0x80098BC0 (real file name unknown): the
// Entity_ReactorLeakLight class. Bounds come from the static initializers of this
// unit and the previous one.
// The two per-difficulty tables at 0x80347150-0x80347170 belong here but stay in
// .rodata unsplit (link order cycle with unit_80014230, as for unit_8009F66C).

#include "game/unk_vec3.mch"
#include "game/Entity.h"
#include "game/unk_vec3.h"

struct UnkColor {
    UnkColor() {}
    UnkColor(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}

    UnkColor operator-(const UnkColor& other) const {
        UnkColor result;
        result.r = r - other.r;
        result.g = g - other.g;
        result.b = b - other.b;
        result.a = a - other.a;
        return result;
    }

    f32 r, g, b, a;
};

// Parameter block of a model node; its getters are chosen by mType (real names
// unknown).
struct UnkParamBlock {
    /* 0x0 */ u8 unk0[0x8];
    /* 0x8 */ void* mData;
    /* 0xC */ u8 mType;
};

struct UnkLightNodeData {
    /* 0x00 */ u8 unk0[0x70];
    /* 0x70 */ UnkParamBlock* mParams;
};

struct UnkLightNode {
    /* 0x00 */ u8 unk0[0x10];
    /* 0x10 */ UnkLightNodeData* mData;
    /* 0x14 */ u8 unk14[0x5C - 0x14];
    /* 0x5C */ UnkColor mColor;
};

extern "C" {
int fn_80070458(void);
UnkLightNode* fn_801DA788(void* model, int index);
}

// Parameter getters per block type (10 operations x 3 types, .rodata).
extern int (*const lbl_80348F38[10][3])(void* data, int id);

// Count thresholds per difficulty (index from fn_80070458).
extern const int lbl_80347150[4];
extern const int lbl_80347160[4];

inline f32 UnkGetParamF(UnkParamBlock* params, int id) {
    union {
        int i;
        f32 f;
    } value;
    value.i = lbl_80348F38[4][params->mType](params->mData, id);
    return value.f;
}

Entity_ReactorLeakLight::Entity_ReactorLeakLight(int type, EntityInfo* info, int init)
    : Entity_Prop(type, info, init) {}

Entity_ReactorLeakLight::~Entity_ReactorLeakLight() {}

void Entity_ReactorLeakLight::fn_80098700() {
    UnkEntity94* model = &m94;
    int index = model->fn_801EC880((void*)0x60D49509);
    UnkVec3 pos = model->mBones[index].mPos;
}

void Entity_ReactorLeakLight::fn_8009875C(const UnkColor& color) {
    UnkParamBlock* params = fn_801DA788(vf0C(), 0)->mData->mParams;
    UnkColor current;
    current.r = UnkGetParamF(params, 0x135);
    current.g = UnkGetParamF(params, 0x136);
    current.b = UnkGetParamF(params, 0x137);
    current.a = UnkGetParamF(params, 0x138);
    fn_801DA788(vf0C(), 0)->mColor = color - current;
}

// 99.95 %: in the inlined copy of fn_8009875C the DOL gives the four parameter reads
// and the two colors the opposite stack slots of the out-of-line copy (first read
// lowest); this build uses the same order in both.
void Entity_ReactorLeakLight::fn_800988F0(int count) {
    static UnkColor sOff(0.2f, 0.2f, 0.2f, 1.0f);
    UnkColor color = sOff;
    if (count > lbl_80347160[fn_80070458()]) {
        color = UnkColor(1.0f, 0.0f, 0.0f, 1.0f);
    } else if (count > lbl_80347150[fn_80070458()]) {
        color = UnkColor(1.0f, 0.5f, 0.0f, 1.0f);
    } else if (count > 0) {
        color = UnkColor(1.0f, 1.0f, 0.0f, 1.0f);
    }
    fn_8009875C(color);
}
