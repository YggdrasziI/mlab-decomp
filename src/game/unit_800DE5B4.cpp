// Game unit at 0x800DE5B4-0x800DEC28 (real file name unknown): SphereGraph (RTTI), a
// set of spheres joined by edges, used by CIDynamic. Bounds come from the static
// initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/CameraInfluence.h"
#include "game/unk_vec3.h"

SphereGraph::SphereGraph() {
    if (lbl_805F95F8 != -1) {
        m0 = 0;
        mEdgeCount = 0;
        mNodes = 0;
        mEdges = 0;
    }
}

SphereGraph::~SphereGraph() {
    if (mNodes) {
        fn_801D4980(mNodes);
    }
    if (mEdges) {
        fn_801D4980(mEdges);
    }
    m0 = 0;
    mEdgeCount = 0;
    mNodes = 0;
    mEdges = 0;
}

// Finds the edge whose capsule is closest to pos.
bool SphereGraph::fn_800DE678(UnkVec3 pos, UnkSphereGraphHit* hit, f32 radius) {
    hit->mDistance = 9999.0f;
    bool found = false;
    for (int i = 0; i < mEdgeCount; i++) {
        UnkSphereGraphHit edgeHit;
        if (fn_800DE780(pos, mNodes[mEdges[i].mA].mPos, mNodes[mEdges[i].mB].mPos, &edgeHit, radius,
                        mNodes[mEdges[i].mA].mRadius, mNodes[mEdges[i].mB].mRadius) &&
            edgeHit.mDistance < hit->mDistance) {
            found = true;
            *hit = edgeHit;
        }
    }
    return found;
}

// Distance from pos to the capsule between spheres a and b, both grown by radius.
// NOT MATCHING (75 %): the comparisons and the control flow match, but the original
// reloads pos, a and b from memory in every block where this version keeps them in
// callee-saved registers (f26-f30), which also changes the stack frame. The source
// shape that prevents this reuse is not found yet (pointer parameters, field-wise
// stores and optimizer pragmas do not change it).
bool SphereGraph::fn_800DE780(const UnkVec3& pos, const UnkVec3& a, const UnkVec3& b,
                              UnkSphereGraphHit* hit, f32 radius, f32 radiusA, f32 radiusB) {
    f32 ra = radiusA + radius;
    f32 rb = radiusB + radius;
    hit->mDistance = 9999999.0f;

    UnkVec3 axis = b - a;
    UnkVec3 rel = pos - a;
    f32 lengthSq = axis.LengthSq();
    bool zero = 0.0f == lengthSq;
    if (zero) {
        return false;
    }
    hit->mAxisX = axis.x;
    hit->mAxisY = axis.y;
    hit->mAxisZ = axis.z;

    f32 t = (rel.x * axis.x + rel.y * axis.y + rel.z * axis.z) / lengthSq;
    UnkVec3 closest;
    f32 r;
    bool before = t < 0.0f;
    if (before) {
        closest = a;
        r = ra;
    } else {
        bool after = t > 1.0f;
        if (after) {
            closest = b;
            r = rb;
        } else {
            closest = a + axis * t;
            r = ra + (rb - ra) * t;
        }
    }

    UnkVec3 diff;
    diff = pos - closest;
    f32 distSq = diff.LengthSq();
    bool inside = distSq < r * r;
    if (inside) {
        bool closer = distSq < hit->mDistance * hit->mDistance;
        if (closer) {
            f32 dist = UnkSqrtf(distSq);
            hit->mDistance = dist;
            hit->mDepth = r - dist;
        }
    }

    if (ra > rb) {
        UnkVec3 toA = a - pos;
        if (toA.LengthSq() - (radius + ra) * (radius + ra) <= 0.0f) {
            diff = pos - a;
            f32 dist = diff.Length();
            if (dist < hit->mDistance) {
                hit->mDistance = dist;
                hit->mDepth = ra - dist;
            }
        }
    } else {
        UnkVec3 toB = b - pos;
        if (toB.LengthSq() - (radius + rb) * (radius + rb) <= 0.0f) {
            diff = pos - b;
            f32 dist = diff.Length();
            if (dist < hit->mDistance) {
                hit->mDistance = dist;
                hit->mDepth = rb - dist;
            }
        }
    }
    return true;
}
