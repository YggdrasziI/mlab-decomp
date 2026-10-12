// Written for this project after mscharged-decomp's w_acos.c (fdlibm wrapper pattern).

/* @(#)w_fmod.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

/*
 * wrap_fmod(x)
 */

#include <internal/fdlibm.h>

#ifdef __STDC__
	double fmod(double x, double y)		/* wrapper fmod */
#else
	double fmod(x,y)			/* wrapper fmod */
	double x,y;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_fmod(x,y);
#else
	double z;
	z = __ieee754_fmod(x,y);
	if(_LIB_VERSION == _IEEE_ || isnan(x)) return z;
	if(fabs(x)>1.0) {
	        return __kernel_standard(x,x,1); /* fmod(|x|>1) */
	} else
	    return z;
#endif
}
