// Game unit at 0x80091D04-0x8009210C (real file name unknown): a door of the robot
// evolution experiment (the callback names "RobotEvolutionExperiment_DoorOpenning",
// "..._DoorOpen", "..._DoorClosing" and "..._DoorClosed" give its four states).
// Up to three doors are registered in a static table. Bounds come from the static
// initializers of this unit and the previous one.

#include "game/externs.h"

class UnkDoor;

// Registered doors, by slot.
static UnkDoor* lbl_803C6A58[3];

#include "game/unk_vec3.h"

// Door state machine (real names unknown).
class UnkDoor {
public:
    UnkDoor();
    ~UnkDoor();

    void Reset() {
        if (mSlot != -1) {
            lbl_803C6A58[mSlot] = 0;
        }
        mOwner = 0;
        mState = -1;
        mSlot = -1;
        mTimerActive = false;
        mTimer = 0.0f;
    }

    /* 0x00 */ void* mOwner;
    /* 0x04 */ int mState;
    /* 0x08 */ int mSlot;
    /* 0x0C */ bool mTimerActive;
    /* 0x10 */ f32 mTimer;
};

// Context passed to the named callbacks (the owner is at 0x10).
struct UnkDoorContext {
    /* 0x00 */ u8 unk0[0x10];
    /* 0x10 */ void* mOwner;
};

typedef void (*UnkDoorCallback)(UnkDoorContext* ctx);

extern "C" void fn_801DC91C(const char* name, int type, UnkDoorCallback func);

static inline UnkDoor* FindDoor(void* owner) {
    for (int i = 0; i < 3; i++) {
        if (lbl_803C6A58[i]->mOwner == owner) {
            return lbl_803C6A58[i];
        }
    }
    return 0;
}

static inline void SendDoorEvent(UnkDoor* door, u32 hash) {
    fn_801BA190(lbl_805FB6C8, door->mOwner, 0, hash, 0, 0, 1, 0);
}

UnkDoor::UnkDoor() {
    mOwner = 0;
    mState = -1;
    mSlot = -1;
    mTimerActive = false;
    mTimer = 0.0f;
}

UnkDoor::~UnkDoor() {
    Reset();
}

extern "C" {

void fn_80091F6C(UnkDoorContext* ctx);
void fn_80091FCC(UnkDoorContext* ctx);
void fn_8009202C(UnkDoorContext* ctx);
void fn_8009208C(UnkDoorContext* ctx);

void fn_80091DAC(void) {
    fn_801DC91C("RobotEvolutionExperiment_DoorOpenning", 78, fn_80091F6C);
    fn_801DC91C("RobotEvolutionExperiment_DoorOpen", 78, fn_80091FCC);
    fn_801DC91C("RobotEvolutionExperiment_DoorClosing", 78, fn_8009202C);
    fn_801DC91C("RobotEvolutionExperiment_DoorClosed", 78, fn_8009208C);
}

void fn_80091E28(UnkDoor* door, int slot, void* owner) {
    lbl_803C6A58[slot] = door;
    door->mOwner = owner;
    door->mSlot = slot;
}

void fn_80091E44(UnkDoor* door) {
    door->Reset();
}

void fn_80091E88(UnkDoor* door, f32 dt) {
    if (door->mTimerActive) {
        door->mTimer -= dt;
        if (door->mTimer <= 0.0f) {
            door->mTimerActive = false;
            door->mState = 0;
            SendDoorEvent(door, 0xCF9DDB47);
        }
    }
}

void fn_80091EE8(UnkDoor* door) {
    door->mState = 0;
    SendDoorEvent(door, 0x31F1D9BF);
}

void fn_80091F18(UnkDoor* door, f32 time) {
    door->mTimer = time;
    door->mTimerActive = true;
}

void fn_80091F28(UnkDoor* door) {
    if (door->mState != 3 || door->mState != 2) {
        SendDoorEvent(door, 0xCF9DDB47);
    }
}

void fn_80091F6C(UnkDoorContext* ctx) {
    UnkDoor* door = FindDoor(ctx->mOwner);
    if (door != 0) {
        door->mState = 0;
    }
}

void fn_80091FCC(UnkDoorContext* ctx) {
    UnkDoor* door = FindDoor(ctx->mOwner);
    if (door != 0) {
        door->mState = 1;
    }
}

void fn_8009202C(UnkDoorContext* ctx) {
    UnkDoor* door = FindDoor(ctx->mOwner);
    if (door != 0) {
        door->mState = 2;
    }
}

void fn_8009208C(UnkDoorContext* ctx) {
    UnkDoor* door = FindDoor(ctx->mOwner);
    if (door != 0) {
        door->mState = 3;
    }
}

}
