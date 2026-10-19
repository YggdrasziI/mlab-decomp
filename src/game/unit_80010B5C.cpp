// Game unit at 0x80010B5C-0x80011018 (real file name unknown): disc error handling.
// While the drive reports no disc, a wrong disc or a read error, shows the message for
// the current language and waits; also the reset and shutdown handling of that screen.
// The start follows the two destructors at 0x80010AC0 and 0x80010B1C, which the
// static initializer of the previous unit registers; the end is this unit's static
// initializer.

#include "game/unk_vec3_rvl.mch"
#include <revolution/dvd.h>
#include <revolution/os.h>
#include <revolution/wpad/WPAD.h>

#include "game/unit_80014230.h"
#include "game/unk_text.h"
#include "game/unk_vec3.h"

class UnkTimer {
public:
    UnkTimer();
    f32 GetSeconds();
    f32 GetSecondsAndRestart();

    /* 0x0 */ OSTime mStart;
};

// Global object at 0x8047C7F0 (real type unknown).
struct UnkSystem {
    /* 0x000 */ u8 unk0[0x2AC];
    /* 0x2AC */ u8 m2AC;
};

typedef bool (*UnkMessageCallback)();

extern "C" {
extern UnkSystem lbl_8047C7F0;
extern s32 lbl_805F81B4;
extern void* lbl_805FB970;
extern void* lbl_805FBB64;
extern s32 lbl_805FBA88;

int fn_8000CF8C(void);
int fn_8000CFA0(void);
void fn_80151258(int arg);
void fn_801FD1F0(UnkSystem* system);
void fn_801FF708(UnkSystem* system, int arg);
void fn_801FD8EC(UnkSystem* system);
void fn_801FD904(UnkSystem* system);
void fn_801B2CE4(int chan);
void fn_801B2D98(int chan);
void fn_80142ED0(s32* result, int arg);
void fn_80142818(s64 usec);
void fn_80142DE8(int mode);
int fn_801F56FC(int arg);
void fn_801F38E8(int arg);
void fn_8013BD7C(void);
void fn_801F5724(int arg);
void fn_801EE588(const char* text, UnkMessageCallback callback);

}

// Disc error messages: no disc, wrong disc (same texts), read error, and two other
// errors, each in English, French, German, Italian and Spanish.
static const char* sDiscMessages[5][10] = {
    {
        "Please insert the Monster Lab\n"
        " Game Disc.",
        "Veuillez ins\xE9rer le disque\n"
        " Monster Lab.",
        "Bitte schiebe die\n"
        " Monster Lab-Disc ein.",
        "Inserisci il disco di\n"
        " Monster Lab.",
        "Inserta el disco de\n"
        " Monster Lab.",
        0,
        0,
        0,
        0,
        0,
    },
    {
        "Please insert the Monster Lab\n"
        " Game Disc.",
        "Veuillez ins\xE9rer le disque\n"
        " Monster Lab.",
        "Bitte schiebe die\n"
        " Monster Lab-Disc ein.",
        "Inserisci il disco di\n"
        " Monster Lab.",
        "Inserta el disco de\n"
        " Monster Lab.",
        0,
        0,
        0,
        0,
        0,
    },
    {
        "The Game Disc could not be read.\n"
        "Refer to the Wii Operations Manual for details.",
        "Impossible de lire le disque.\n"
        "Veuillez vous r\xE9" "f\xE9rer au mode\n"
        " d'emploi Wii pour plus de d\xE9tails.",
        "Diese Disc kann nicht gelesen werden.\n"
        "Bitte lies die Wii-Bedienungsanleitung,\n"
        " um weitere Informationen zu erhalten.",
        "Impossibile leggere il disco.\n"
        "Per maggiori informazioni, consulta il\n"
        " manuale di istruzioni della console Wii.",
        "No se puede leer el disco.\n"
        "Consulta el manual de instrucciones de la\n"
        " consola Wii para obtener m\xE1s informaci\xF3n.",
        0,
        0,
        0,
        0,
        0,
    },
    {
        "An error has occurred.\n"
        "\n"
        "Press the Eject Button, remove the Game Disc,\n"
        "and turn off the power to the console.\n"
        "Refer to the Wii Operations Manual for further instructions.",
        "Une erreur est survenue.\n"
        "\n"
        "Appuyez sur le bouton EJECT, retirez le disque\n"
        "et \xE9teignez la console. Veuillez vous r\xE9" "f\xE9rer au\n"
        "mode d'emploi Wii pour plus de d\xE9tails.",
        "Ein Fehler ist aufgetreten.\n"
        "\n"
        "Dr\xFC" "cke den Ausgabeknopf, entnimm die Disc\n"
        "und schalte die Wii-Konsole aus. Bitte lies die\n"
        "Wii-Bedienungsanleitung, um weitere\n"
        "Informationen zu erhalten.",
        "Si \xE8 verificato un errore.\n"
        "\n"
        "Premi il pulsante EJECT, estrai il disco\n"
        "e spegni la console. Per maggiori informazioni,\n"
        "consulta il manuale di istruzioni della console Wii.",
        "Se ha producido un error.\n"
        "\n"
        "Pulsa el bot\xF3n EJECT, extrae el disco y apaga\n"
        "la consola. Consulta el manual de instrucciones\n"
        "de la consola Wii para obtener m\xE1s informaci\xF3n.",
        0,
        0,
        0,
        0,
        0,
    },
    {
        "An error has occurred.\n"
        "\n"
        "Press the EJECT Button, remove the Game Disc,\n"
        "and turn the power off. Please read the\n"
        "Wii Operations Manual for more information.",
        "Une erreur est survenue.\n"
        "\n"
        "Appuyez sur le bouton EJECT, retirez\n"
        "le disque et \xE9teignez la console.\n"
        "Veuillez vous r\xE9" "f\xE9rer au\n"
        "mode d'emploi Wii pour plus de d\xE9tails.",
        "Ein Fehler ist aufgetreten.\n"
        "\n"
        "Dr\xFC" "cke den Ausgabeknopf, entnimm\n"
        "die Disc und schalte die Wii-Konsole aus.\n"
        "Bitte lies die Wii-Bedienungsanleitung,\n"
        "um weitere Informationen zu erhalten.",
        "Si \xE8 verificato un errore.\n"
        "\n"
        "Premi il pulsante EJECT, estrai il\n"
        "disco e spegni la console.\n"
        "Per maggiori informazioni, consulta il manuale\n"
        "di istruzioni della console Wii.",
        "Se ha producido un error.\n"
        "\n"
        "Pulsa el bot\xF3n EJECT, extrae el disco y apaga\n"
        "la consola. Consulta el manual de instrucciones\n"
        "de la consola\n"
        "Wii para obtener m\xE1s informaci\xF3n.",
        0,
        0,
        0,
        0,
        0,
    },
};

// Read error text used for the English language when fn_8001425C is not 0.
static char sDiscReadError[] = "The Game Disc could not be read.\n"
                               "Please read the Wii Operations Manual\n"
                               "for more information.";

static u32 sLastStatus;
static f32 sTime;
static bool sChanged;
static UnkTimer sTimer;

static inline bool IsDiscError(s32 status) {
    bool error = false;
    switch (status) {
    case DVD_STATE_FATAL:
    case DVD_STATE_NO_DISK:
    case DVD_STATE_WRONG_DISK_ID:
    case DVD_STATE_DISK_ERROR:
        error = true;
        break;
    }
    return error;
}

bool fn_80010B5C() {
    bool done = false;
    s32 status = DVDGetDriveStatus();
    if (status != sLastStatus && !sChanged) {
        sChanged = true;
        sTime = sTimer.GetSeconds();
    }
    if (sChanged) {
        if (IsDiscError(status) || status == DVD_STATE_IDLE) {
            done = true;
        } else if (sTimer.GetSeconds() > 10.0f + sTime) {
            done = true;
        }
    }
    if (fn_8000CF8C()) {
        fn_80151258(0);
        if (lbl_8047C7F0.m2AC) {
            fn_801FD1F0(&lbl_8047C7F0);
        }
        switch (fn_8000CFA0()) {
        case 1:
            OSRestart(0);
            break;
        case 2:
            OSReturnToMenu();
            break;
        case 3:
            OSShutdownSystem();
            break;
        }
        done = true;
    }
    return done;
}

bool fn_80010CA4() {
    return false;
}

void fn_80010CAC() {
    if (lbl_8047C7F0.m2AC) {
        int count = 4;
        fn_801FF708(&lbl_8047C7F0, 1);
        fn_801FD8EC(&lbl_8047C7F0);
        for (int i = 0; i < 4; i++) {
            fn_801B2CE4(i);
        }
        do {
            lbl_805FBA88 = 0;
            fn_80142ED0(&lbl_805FBA88, 4);
            while (lbl_805FBA88 == 0) {
                fn_80142818(1000);
            }
        } while (--count);
    }
    if (lbl_805FB970 && !fn_801F56FC(2)) {
        if (lbl_805FBB64) {
            fn_801F38E8(1);
        } else {
            fn_8013BD7C();
        }
        fn_801F5724(0x110);
    }
    WPADControlMotor(0, WPAD_MOTOR_STOP);
    WPADControlMotor(1, WPAD_MOTOR_STOP);
    WPADControlMotor(2, WPAD_MOTOR_STOP);
    WPADControlMotor(3, WPAD_MOTOR_STOP);
    s32 status = DVDGetDriveStatus();
    while (IsDiscError(status)) {
        sLastStatus = status;
        sTimer.GetSecondsAndRestart();
        sTime = 0.0f;
        sChanged = false;
        const char* text;
        switch (status) {
        case DVD_STATE_NO_DISK:
            text = sDiscMessages[0][lbl_805F81B4];
            fn_8001425C();
            if (text == 0) {
                text = sDiscMessages[0][0];
            }
            if (text == 0) {
                text = "(null)";
            }
            fn_801EE588(text, fn_80010B5C);
            break;
        case DVD_STATE_WRONG_DISK_ID:
            text = sDiscMessages[1][lbl_805F81B4];
            fn_8001425C();
            if (text == 0) {
                text = sDiscMessages[1][0];
            }
            if (text == 0) {
                text = "(null)";
            }
            fn_801EE588(text, fn_80010B5C);
            break;
        case DVD_STATE_DISK_ERROR:
            text = sDiscMessages[2][lbl_805F81B4];
            bool region0 = fn_8001425C() == 0;
            if (!region0 && lbl_805F81B4 == 0) {
                text = sDiscReadError;
            }
            if (text == 0) {
                text = sDiscMessages[2][0];
            }
            if (text == 0) {
                text = "(null)";
            }
            fn_801EE588(text, fn_80010B5C);
            break;
        }
        status = DVDGetDriveStatus();
    }
    for (int i = 0; i < 4; i++) {
        fn_801B2D98(i);
    }
    fn_801FF708(&lbl_8047C7F0, 0);
    fn_801FD904(&lbl_8047C7F0);
}

// The empty string many units return as a fallback (real name unknown). The DOL has
// it in .sdata after this unit's "(null)", so it is an explicitly zero-initialized
// object: without the pragma MWCC puts it in .sbss.
extern "C" {
#pragma explicit_zero_data on
char lbl_805F8200[8] = "";
#pragma explicit_zero_data reset
}

int fn_80010F64() {
    fn_80010CAC();
    return 0;
}

int fn_80010F88(s32 result) {
    switch (result) {
    case -4:
        UnkTextTable* table = lbl_805FB910;
        const char* text;
        if (table->mTexts != 0 && table->mCount > 0x1080) {
            text = table->mTexts[0x1080];
        } else {
            text = lbl_805F8200;
        }
        fn_801EE588(text, fn_80010CA4);
        break;
    }
    return 0;
}

void fn_80010FE8() {
    fn_80142DE8(0);
}

void fn_80010FF0() {
    fn_80010CAC();
}
