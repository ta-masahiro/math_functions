/* origin: FreeBSD /usr/src/lib/msun/src/s_sin.c */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */
/* sin(x)
 * Return sine function of x.
 *
 * kernel function:
 *      __sin            ... sine function on [-pi/4,pi/4]
 *      __cos            ... cose function on [-pi/4,pi/4]
 *      __rem_pio2       ... argument reduction routine
 *
 * Method.
 *      Let S,C and T denote the sin, cos and tan respectively on
 *      [-PI/4, +PI/4]. Reduce the argument x to y1+y2 = x-k*pi/2
 *      in [-pi/4 , +pi/4], and let n = k mod 4.
 *      We have
 *
 *          n        sin(x)      cos(x)        tan(x)
 *     ----------------------------------------------------------
 *          0          S           C             T
 *          1          C          -S            -1/T
 *          2         -S          -C             T
 *          3         -C           S            -1/T
 *     ----------------------------------------------------------
 *
 * Special cases:
 *      Let trig be any of sin, cos, or tan.
 *      trig(+-INF)  is NaN, with signals;
 *      trig(NaN)    is that NaN;
 *
 * Accuracy:
 *      TRIG(x) returns trig(x) nearly rounded
 */

//#include "libm.h"


#define asuint64(f) ((union{double _f; unsigned long _i;}){f})._i
#define asdouble(i) ((union{uint64_t _i; double _f;}){i})._f

#define GET_HIGH_WORD(hi,d)                       \
do {                                              \
  (hi) = asuint64(d) >> 32;                       \
} while (0)

#define GET_LOW_WORD(lo,d)                        \
do {                                              \
  (lo) = (unsigned)asuint64(d);                   \
} while (0)

extern int reduce_sincos(), reduce_sincos_long();
extern double __sin__(), __cos__();

double My_sin(double x)
{
	double d, dd;
	unsigned ix, n;

	/* High word of x. */
	GET_HIGH_WORD(ix, x);
	ix &= 0x7fffffff;

	/* |x| ~< pi/4 */
	if (ix <= 0x3fe921fb) {
		if (ix < 0x3e500000) {  /* |x| < 2**-26 */
			/* raise inexact if x != 0 and underflow if subnormal*/
			//FORCE_EVAL(ix < 0x00100000 ? x/0x1p120f : x+0x1p120f);
			return x;
		}
		return __sin__(x, 0.0, 0);
	} else if (ix < 0x419921fb) {
		n = reduce_sincos(x, &d, &dd);
	} else if (ix < 0x7ff00000) {
		n = reduce_sincos_long(x, &d, &dd);
	} else {
		/* sin(Inf or NaN) is NaN */
		return x - x;
	}
	/* argument reduction needed */
	switch (n&3) {
	case 0: return  __sin__(d, dd, 1);
	case 1: return  __cos__(d, dd);
	case 2: return -__sin__(d, dd, 1);
	default:
		return -__cos__(d, dd);
	}
}

double My_cos(double x)
{
	double d, dd;
	unsigned ix, n;

	GET_HIGH_WORD(ix, x);
	ix &= 0x7fffffff;

	/* |x| ~< pi/4 */
	if (ix <= 0x3fe921fb) {
		if (ix < 0x3e46a09e) {  /* |x| < 2**-27 * sqrt(2) */
			/* raise inexact if x!=0 */
			//FORCE_EVAL(x + 0x1p120f);
			return 1.0;
		}
		return __cos__(x, 0);
	} else if (ix < 0x419921fb) {
		n = reduce_sincos(x, &d, &dd);
	} else if (ix < 0x7ff00000) {
		n = reduce_sincos_long(x, &d, &dd);
	} else {
	/* cos(Inf or NaN) is NaN */
		return x-x;
	}
	/* argument reduction */
	switch (n&3) {
	case 0: return  __cos__(d, dd);
	case 1: return -__sin__(d, dd, 1);
	case 2: return -__cos__(d, dd);
	default:
		return  __sin__(d, dd, 1);
	}
}