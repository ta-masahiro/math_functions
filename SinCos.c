#include <math.h>
/********************************************************** 
** Sine function 
** programmed by Y. Tokieda,  Apr. 08, 2020. 
*/ 
#define _PI_16   1.963495408493621E-01 
#define _PI_8    3.926990816987241E-01 
#define _1_PI_8  2.546479089470326 
static double s_adfC[6] = { /* Expansion coef for cos(x) */ 
	-2.755731922398589E-07, 2.480158730158730E-05, 
	-1.388888888888889E-03, 4.166666666666666E-02, 
	-0.500000000000000E-01, 1.000000000000000E-00 
}; 
static double s_adfS[6] = { /* Expansion coef for sin(x) */ 
	-2.505210838544172E-08, 2.755731922398589E-06, 
	-1.984126984126984E-04, 8.333333333333333E-03, 
	-1.666666666666667E-01, 1.000000000000000E-00, 
}; 
static double s_adfCS[5] = { /* cos(pi * n / 8) */ 
	1.000000000000000E-00, 9.238795325112860E-01, 
	7.071067811865476E-01, 3.826834323650898E-01, 
	0.000000000000000 
}; 
void * SinCos(double dfX, double *S, double *C) 
{ 
	double dfN; 
	double dfX2; 
	double dfSin, dfCos; 
	double adfY[4]; 
	unsigned int uIdx; 
	unsigned int uIdx2; 
	dfX += _PI_16; 
	dfN  = floor(dfX * _1_PI_8); 
	dfX -= (dfN * _PI_8); 
	dfX -= _PI_16; 
	uIdx = (unsigned int)dfN; 
	dfX2 = dfX * dfX; 
	/**** Tayloer for sin(x) ****/ 
	dfSin = s_adfS[0]; 
	dfSin = dfSin * dfX2 + s_adfS[1]; 
	dfSin = dfSin * dfX2 + s_adfS[2]; 
	dfSin = dfSin * dfX2 + s_adfS[3]; 
	dfSin = dfSin * dfX2 + s_adfS[4]; 
	dfSin = dfSin * dfX2 + s_adfS[5]; 
	dfSin = dfSin * dfX; 
	/**** Taylor for cos(x) ****/ 
	dfCos = s_adfC[0]; 
	dfCos = dfCos * dfX2 + s_adfC[1]; 
	dfCos = dfCos * dfX2 + s_adfC[2]; 
	dfCos = dfCos * dfX2 + s_adfC[3]; 
	dfCos = dfCos * dfX2 + s_adfC[4]; 
	dfCos = dfCos * dfX2 + s_adfC[5]; 
	/**** Addition Theorem ****/ 
	uIdx2 = uIdx & 0x03; 
	dfN   = s_adfCS[4 - uIdx2] * dfCos + s_adfCS[    uIdx2] * dfSin; 
	dfCos = s_adfCS[    uIdx2] * dfCos - s_adfCS[4 - uIdx2] * dfSin; 
	dfSin = dfN; 
	/**** Symmetricity w.r.t. Rotation ****/ 
	adfY[0] =  dfSin;    adfY[1] =  dfCos;   
    adfY[2] = -dfSin;    adfY[3] = -dfCos; 
	*S = adfY[(uIdx >> 2) & 0x03]; 
	*C = adfY[((uIdx >> 2)+1) & 0x03]; 
} 
