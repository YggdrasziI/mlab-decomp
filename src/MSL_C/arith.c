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

// Unknown function at 0x80332EF0, called by the date code at 0x8033ABF0: signed
// remainder whose result takes the sign of the divisor (floored modulo). Real name not
// known yet, so it keeps its address-based name. Decompiled from this game's DOL.
int fn_80332EF0(int numerator, int denominator)
{
    int i = 1;
    int j = 1;
    int sign;
    int quot;
    int rem;

    if (numerator < 0) {
        numerator = -numerator;
        i = -1;
    }

    if (denominator < 0) {
        denominator = -denominator;
        j = -1;
    }

    sign = i * j;
    quot = (numerator / denominator) * sign;
    rem = numerator * i - j * (quot * denominator);
    if (rem != 0 && sign < 0) {
        rem += denominator * j;
    }
    return rem;
}
