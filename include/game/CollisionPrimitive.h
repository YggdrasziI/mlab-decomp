#ifndef GAME_COLLISIONPRIMITIVE_H
#define GAME_COLLISIONPRIMITIVE_H

// Collision primitive classes (names from RTTI: the type_info objects at
// 0x805F94E8-0x805F9518 name the vtables at 0x803839A8-0x80383B18 and their base
// CollisionPrimitive). Their functions are not decompiled yet; only the members used
// by decompiled code are known.

#include "types.h"

class CollisionPrimitive {
public:
    CollisionPrimitive() : m4(0) {}
    virtual ~CollisionPrimitive();
    virtual void vf0C();

    /* 0x00 */ // vtable
    /* 0x04 */ u32 m4;
    /* 0x08 */ u32 m8;
    /* 0x0C */ u32 mType; // 0 sphere, 3 hemisphere, 4 AABBox, 5 line segment, 6 ray, 7 OBBox
};

// Constructor at 0x801B11DC (sets mType to 6).
class CollisionPrimitive_Ray : public CollisionPrimitive {
public:
    CollisionPrimitive_Ray();
    virtual void vf0C();
};

class CollisionPrimitive_LineSeg : public CollisionPrimitive {
public:
    virtual void vf0C();
};

class CollisionPrimitive_Hemisphere : public CollisionPrimitive {
public:
    virtual void vf0C();
};

class CollisionPrimitive_Sphere : public CollisionPrimitive {
public:
    virtual void vf0C();
};

class CollisionPrimitive_OBBox : public CollisionPrimitive {
public:
    virtual void vf0C();
};

class CollisionPrimitive_AABBox : public CollisionPrimitive {
public:
    virtual void vf0C();
};

#endif // GAME_COLLISIONPRIMITIVE_H
