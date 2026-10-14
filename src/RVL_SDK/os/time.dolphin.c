// Written from the DOL; the file and function names are those of oot-vc's split.
#include <revolution/os.h>

// Seconds since 1900-01-01 (0xBC17C200 seconds from 1900 to 2000, the OS epoch).
unsigned long __get_time(void) {
    return (unsigned long)(OSTicksToSeconds(OSGetTime()) + 0xBC17C200);
}
