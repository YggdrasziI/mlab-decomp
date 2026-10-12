#include <arith.h>

int abs(int n)
{
    if (n < 0)
        return -n;
    else
        return n;
}

// labs: from zeldaret/tp (CC0-1.0); present in this game's DOL.
long labs(long __x) {
    return __x > 0 ? __x : -__x;
}

// The DOL has a third function here (0x80332EF0, a signed remainder helper) whose
// name is not known yet; it is not TP's div(). Unit left NonMatching.
