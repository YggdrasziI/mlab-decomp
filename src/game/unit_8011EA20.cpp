// Game unit at 0x8011EA20-0x8011F410 (real file name unknown): the QuestLog section
// of the Journal (RTTI). Counts the active quests (the main quest first when it is
// running), loads the quest icon texture ("ui/msa_quest_icons/quest_icon_%03d") and
// gives names, states and "%d/%d" progress texts. Bounds come from the static
// initializers of this unit and the previous one.

#include "game/Journal.h"
#include "game/externs.h"
#include "game/unk_text.h"
#include "game/unk_vec3.h"

// Quest description (0x1C bytes, real type unknown).
struct UnkQuestInfo {
    /* 0x00 */ u32 m0;
    /* 0x04 */ u32 mNameHash;
    /* 0x08 */ u8 unk8[0x18 - 0x8];
    /* 0x18 */ s8 mIcon;
    /* 0x19 */ u8 unk19[3];
};

struct UnkQuestData {
    /* 0x0000 */ u8 unk0[0x8508];
    /* 0x8508 */ UnkQuestInfo* mQuests;
};

struct UnkQuestDatabase {
    /* 0x0 */ u8 unk0[0xC];
    /* 0xC */ UnkQuestData* mData;
};

extern "C" {
extern void* lbl_805FBBE0;
extern UnkQuestDatabase* lbl_805FBBC0;

UnkQuest* fn_80222704(void* quests, int main, int index);
int fn_80222974(void* quests, int id);
int fn_80222994(void* quests);
u32 fn_80226C24(void* quests);
u32 fn_80226C2C(void* quests);
UnkTexture* fn_80109330(const char* name);
UnkTexture* fn_801094F4(const char* name, int arg);
void fn_80184028(UnkTexture* texture);
}

const char* lbl_805F8F90 = "ui/msa_quest_icons/";

static inline UnkQuest* QuestLog_GetQuest(QuestLog* log, int index) {
    UnkQuest* quest = 0;
    if (log->mHasMainQuest) {
        quest = fn_80222704(lbl_805FBBE0, index + 1, 0);
    } else if (index < log->mQuestCount) {
        quest = fn_80222704(lbl_805FBBE0, 0, log->mQuestCount - index - 1);
    }
    return quest;
}

// With -inline auto,deferred the compiler emits the functions in reverse order of
// definition, so they are defined here from the last to the first.

const char* QuestLog::fn_8011F32C() {
    return GetText(fn_80226C2C(lbl_805FBBE0));
}

int QuestLog::fn_8011F2FC() {
    return fn_80226C2C(lbl_805FBBE0) != 0;
}

const char* QuestLog::fn_8011F1E0(int index) {
    UnkQuest* quest = QuestLog_GetQuest(this, index);
    if (quest != 0) {
        return GetText(quest->mNameHash);
    }
    return 0;
}

UnkTexture* QuestLog::fn_8011F1D8() {
    return mIcon;
}

int QuestLog::fn_8011F1C4() {
    return mIcon != 0;
}

char* QuestLog::fn_8011F104(int index) {
    if (index < mQuestCount) {
        UnkQuest* quest = QuestLog_GetQuest(this, index);
        if (quest != 0 && quest->mGoal > 0) {
            char* text = mProgressText[index];
            fn_80142EF8(text, "%d/%d", quest->mProgress, quest->mGoal);
            return text;
        }
    }
    return 0;
}

int QuestLog::fn_8011F080(int index) {
    if (index < mQuestCount) {
        UnkQuest* quest = QuestLog_GetQuest(this, index);
        if (quest != 0 && quest->mGoal > 0) {
            return 1;
        }
    }
    return 0;
}

int QuestLog::fn_8011EFFC(int index) {
    if (index < mQuestCount) {
        UnkQuest* quest = QuestLog_GetQuest(this, index);
        if (quest != 0 && quest->mState == 2) {
            return 1;
        }
    }
    return 0;
}

int QuestLog::fn_8011EFE0(int index) {
    return index < mQuestCount;
}

int QuestLog::fn_8011EFB8() {
    fn_80222994(lbl_805FBBE0);
    return 0;
}

const char* QuestLog::fn_8011EEF4() {
    return GetText(fn_80226C24(lbl_805FBBE0));
}

const char* QuestLog::fn_8011EDE0() {
    int index = fn_80222994(lbl_805FBBE0);
    UnkQuestInfo info = lbl_805FBBC0->mData->mQuests[index];
    return GetText(info.mNameHash);
}

int QuestLog::fn_8011EDAC() {
    return fn_80222994(lbl_805FBBE0) != -1;
}

UnkTexture* QuestLog::fn_8011ED38(s8 icon) {
    // Real size unknown; a multiple of 8 changes its stack alignment once inlined.
    char name[0xFF];
    fn_80142EF8(name, "%squest_icon_%03d", lbl_805F8F90, icon);
    UnkTexture* texture = fn_801094F4(name, 0);
    if (texture == 0) {
        fn_80142EF8(name, "%squest_icon_placeholder", lbl_805F8F90);
        texture = fn_801094F4(name, 0);
    }
    return texture;
}

UnkTexture* QuestLog::fn_8011ECCC(s8 icon) {
    // Real size unknown; a multiple of 8 changes its stack alignment once inlined.
    char name[0xFF];
    fn_80142EF8(name, "%squest_icon_%03d", lbl_805F8F90, icon);
    UnkTexture* texture = fn_80109330(name);
    if (texture == 0) {
        fn_80142EF8(name, "%squest_icon_placeholder", lbl_805F8F90);
        texture = fn_80109330(name);
    }
    return texture;
}

void QuestLog::vf18() {}

void QuestLog::vf14() {}

void QuestLog::vf10() {
    if (mIcon != 0) {
        UnkTexture* icon = mIcon;
        int count = --icon->mRefCount;
        if (count <= 0 && count == 0) {
            fn_80184028(icon);
        }
        mIcon = 0;
    }
}

void QuestLog::vf0C() {
    mHasMainQuest = 0;
    mQuestCount = 0;
    fn_80222704(lbl_805FBBE0, 1, 0);
    if (fn_80222974(lbl_805FBBE0, 0x16) > 0) {
        mHasMainQuest = 1;
    }
    for (int i = 0; i < 3; i++) {
        if (mHasMainQuest) {
            if (fn_80222704(lbl_805FBBE0, i + 1, 0)->mState != 0) {
                mQuestCount++;
            }
        } else {
            if (fn_80222704(lbl_805FBBE0, 0, i)->mState != 0) {
                mQuestCount++;
            }
        }
    }
    mIcon = 0;
    if (fn_80222994(lbl_805FBBE0) != -1) {
        int index = fn_80222994(lbl_805FBBE0);
        UnkQuestInfo info = lbl_805FBBC0->mData->mQuests[index];
        if (info.mIcon != 0 && info.mIcon != -1) {
            mIcon = fn_8011ECCC(info.mIcon);
        }
    }
}

QuestLog::~QuestLog() {}

QuestLog::QuestLog() {
    mHasMainQuest = 0;
    mQuestCount = 0;
    mIcon = 0;
}
