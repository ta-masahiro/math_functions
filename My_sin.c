
#define HIGH_HALF	1
#define LOW_HALF	0
typedef union { int i[2]; double x;} mynumber;
//extern int reduce_sincos(double, double *, double *), reduce_sincos_long(double, double *, double *);
extern int reduce_sincos_long(double, double *, double *);

static const double
S1  = -1.66666666666666324348e-01, /* 0xBFC55555, 0x55555549 */
S2  =  8.33333333332248946124e-03, /* 0x3F811111, 0x1110F8A6 */
S3  = -1.98412698298579493134e-04, /* 0xBF2A01A0, 0x19C161D5 */
S4  =  2.75573137070700676789e-06, /* 0x3EC71DE3, 0x57B1FE7D */
S5  = -2.50507602534068634195e-08, /* 0xBE5AE5E6, 0x8A2B9CEB */
S6  =  1.58969099521155010221e-10; /* 0x3DE5D93A, 0x5ACFD57C */

double __sin(double x, double y, int iy)
{
	double z,r,v,w;

	z = x*x;
	w = z*z;
	r = S2 + z*(S3 + z*S4) + z*w*(S5 + z*S6);
	v = z*x;
	if (iy == 0)
		return x + v*(S1 + z*r);
	else
		return x - ((z*(0.5*y - v*r) - y) - v*S1);
}

static const double
C1  =  4.16666666666666019037e-02, /* 0x3FA55555, 0x5555554C */
C2  = -1.38888888888741095749e-03, /* 0xBF56C16C, 0x16C15177 */
C3  =  2.48015872894767294178e-05, /* 0x3EFA01A0, 0x19CB1590 */
C4  = -2.75573143513906633035e-07, /* 0xBE927E4F, 0x809C52AD */
C5  =  2.08757232129817482790e-09, /* 0x3E21EE9E, 0xBDB4B1C4 */
C6  = -1.13596475577881948265e-11; /* 0xBDA8FAE9, 0xBE8838D4 */

double __cos(double x, double y)
{
	double hz,z,r,w;

	z  = x*x;
	w  = z*z;
	r  = z*(C1+z*(C2+z*C3)) + w*w*(C4+z*(C5+z*C6));
	hz = 0.5*z;
	w  = 1.0-hz;
	return w + (((1.0-w)-hz) + (z*r-x*y));
}

static const double mp1 = 0x1.921FB58000000p0;    /*  1.5707963407039642      */
static const double mp2 = -0x1.DDE973C000000p-27; /* -1.3909067564377153e-08  */
static const double pp3 = -0x1.CB3B398000000p-55; /* -4.9789962314799099e-17  */
static const double pp4 = -0x1.d747f23e32ed7p-83; /* -1.9034889620193266e-25  */
static const double hpinv = 0x1.45F306DC9C883p-1; /*  0.63661977236758138     */
static const double toint = 0x1.8000000000000p52; /*  6755399441055744        */

//typedef union { int i[2]; double x; double d; } mynumber;

//#define LOW_HALF	0

// static inline int4
int
reduce_sincos (double x, double *a, double *da)
{
  mynumber v;

  double t = (x * hpinv + toint);
  double xn = t - toint;
  v.x = t;
  double y = (x - xn * mp1) - xn * mp2;
  int n = v.i[LOW_HALF] & 3;

  double b, db, t1, t2;
  t1 = xn * pp3;
  t2 = y - t1;
  db = (y - t2) - t1;

  t1 = xn * pp4;
  b = t2 - t1;
  db += (t2 - b) - t1;

  *a = b;
  *da = db;
  return n;
}

double My_sin(double x)
{
	double d, dd;
	unsigned ix, n;
	mynumber zz;
	
	zz.x = x;
	ix = zz.i[HIGH_HALF];	/* High word of x. */
	ix &= 0x7fffffff;

	/* |x| ~< pi/4 */
	if (ix <= 0x3fe921fb) {
		if (ix < 0x3e500000) {  /* |x| < 2**-26 */
			/* raise inexact if x != 0 and underflow if subnormal*/
			//FORCE_EVAL(ix < 0x00100000 ? x/0x1p120f : x+0x1p120f);
			return x;
		}
		return __sin(x, 0.0, 0);
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
	case 0: return  __sin(d, dd, 1);
	case 1: return  __cos(d, dd);
	case 2: return -__sin(d, dd, 1);
	default:
		return -__cos(d, dd);
	}
}

double My_cos(double x)
{
	double d, dd;
	unsigned ix, n;
	mynumber zz;

	zz.x = x;
	ix = zz.i[HIGH_HALF];
	//GET_HIGH_WORD(ix, x);
	ix &= 0x7fffffff;

	/* |x| ~< pi/4 */
	if (ix <= 0x3fe921fb) {
		if (ix < 0x3e46a09e) {  /* |x| < 2**-27 * sqrt(2) */
			/* raise inexact if x!=0 */
			//FORCE_EVAL(x + 0x1p120f);
			return 1.0;
		}
		return __cos(x, 0.0);
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
	case 0: return  __cos(d, dd);
	case 1: return -__sin(d, dd, 1);
	case 2: return -__cos(d, dd);
	default:
		return  __sin(d, dd, 1);
	}
}