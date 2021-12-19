#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>
#include <mpfr.h>
extern double My_sin(double), My_cos(double); 
int main(int argc, char * argv[]) {
    int i,n = atoi(argv[2]);
    long j, base =atol(argv[1]); 
    double sum = 0, s1, s2, c1, c2, th;
    mpfr_t theta, ans_sin, ans_cos,er1,er2,er3,er4,ser1,ser2,ser3,ser4;
    mpfr_set_default_prec(2048);
    mpfr_init(theta);mpfr_init(ans_sin);mpfr_init(ans_cos);mpfr_init(er1);mpfr_init(er2);mpfr_init(er3);mpfr_init(er4);
    mpfr_init_set_si(ser1,0,MPFR_RNDN);mpfr_init_set_si(ser2,0,MPFR_RNDN);mpfr_init_set_si(ser3,0,MPFR_RNDN);mpfr_init_set_si(ser4,0,MPFR_RNDN);
    char buf_1[1024], buf_2[1024];
    printf("theta(degree)\ttheta(rad)\tsin(glibc)\t\tcos(glibc)\t\tsin(original)\t\tcos(original)\t\tsin(gmp)\t\tcos(gmp)\n");
    printf("===========================================================================================================================\n");  
    for(i = 0; i<=90*n; i ++ ) {
        j=base*n+i;
        //th = i * 3.141592653589793238 / (180*n);
        //th=(double)j;
        mpfr_const_pi(theta, MPFR_RNDN); mpfr_mul_si(theta, theta, j, MPFR_RNDN); mpfr_div_si(theta, theta, 180*n, MPFR_RNDN); 
        //mpfr_set_si(theta,j,MPFR_RNDF);
        th=mpfr_get_d(theta,MPFR_RNDN);
        mpfr_sin(ans_sin, theta, MPFR_RNDN); mpfr_sprintf(buf_1,"%20.18Rg",(mpfr_ptr)ans_sin);
        mpfr_cos(ans_cos, theta, MPFR_RNDN); mpfr_sprintf(buf_2,"%20.18Rg",(mpfr_ptr)ans_cos);
        mpfr_set_d(er1, (s1 = sin(th)), MPFR_RNDN); mpfr_sub(er1,er1, ans_sin, MPFR_RNDN); mpfr_fma(ser1, er1, er1, ser1, MPFR_RNDN);
        mpfr_set_d(er2, (c1 = cos(th)), MPFR_RNDN); mpfr_sub(er2,er2, ans_cos, MPFR_RNDN); mpfr_fma(ser2, er2, er2, ser2, MPFR_RNDN);
        mpfr_set_d(er3, (s2 = My_sin(th)), MPFR_RNDN); mpfr_sub(er3,er3, ans_sin, MPFR_RNDN); mpfr_fma(ser3, er3, er3, ser3, MPFR_RNDN);
        mpfr_set_d(er4, (c2 = My_cos(th)), MPFR_RNDN); mpfr_sub(er4,er4, ans_cos, MPFR_RNDN); mpfr_fma(ser4, er4, er4, ser4, MPFR_RNDN);
        //if (My_cos(th)<-1.0 || My_cos(th) > 1.0) printf("%lx\n",*(long*)&th);
        printf("%ld\t%f\t%18.16f\t%18.16f\t%18.16f\t%18.16f\t%s\t%s\n", j, th, s1, c1, s2, c2, buf_1,buf_2);  
    }
    printf("===========================================================================================================================\n");  
    printf("sum of error\t\t\t%18.13e\t%18.13e\t%18.13e\t%18.13e\n", sqrt(mpfr_get_d(ser1,MPFR_RNDN))/(n*90), sqrt(mpfr_get_d(ser2,MPFR_RNDN))/(n*90), sqrt(mpfr_get_d(ser3,MPFR_RNDN))/(n*90), sqrt(mpfr_get_d(ser4,MPFR_RNDN))/(n*90)); 
}
