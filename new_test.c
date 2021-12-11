#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>
#include <mpfr.h>
extern double Sin(), new_Sin(); 
int main(int argc, char * argv[]) {
    int i,n = atoi(argv[2]);
    long j, base =atol(argv[1]); 
    double sum = 0, s, c, th;
    mpfr_t theta, ans,er1,er2,er3,ser1,ser2,ser3;
    mpfr_set_default_prec(2048);
    mpfr_init(theta);mpfr_init(ans);mpfr_init(er1);mpfr_init(er2);mpfr_init(er3); mpfr_init_set_si(ser1,0,MPFR_RNDN);mpfr_init_set_si(ser2,0,MPFR_RNDN);mpfr_init_set_si(ser3,0,MPFR_RNDN);
    char buf[1024];
    printf("theta\t\tsin(glibc)\t\tsin(musl)\t\tsin(original)\t\tsin(mpfr)\n");
    printf("==========================================================================================================\n");  
    for(i = 0; i<=90*n; i ++ ) {
        j=base*n+i;
        //th = i * 3.141592653589793238 / (180*n);
        //th=(double)j;
        mpfr_const_pi(theta, MPFR_RNDN); mpfr_mul_si(theta, theta, j, MPFR_RNDN); mpfr_div_si(theta, theta, 180*n, MPFR_RNDN); 
        //mpfr_set_si(theta,j,MPFR_RNDF);
        th=mpfr_get_d(theta,MPFR_RNDN);
        mpfr_sin(ans, theta,MPFR_RNDN); mpfr_sprintf(buf,"%.16Rg",(mpfr_ptr)ans);
        mpfr_set_d(er1,sin(th),MPFR_RNDN);mpfr_sub(er1,er1,ans,MPFR_RNDN);mpfr_fma(ser1,er1,er1,ser1,MPFR_RNDN);
        mpfr_set_d(er2,Sin(th),MPFR_RNDN);mpfr_sub(er2,er2,ans,MPFR_RNDN);mpfr_fma(ser2,er2,er2,ser2,MPFR_RNDN);
        mpfr_set_d(er3,new_Sin(th),MPFR_RNDN);mpfr_sub(er3,er3,ans,MPFR_RNDN);mpfr_fma(ser3,er3,er3,ser3,MPFR_RNDN);

        printf("%f\t%18.16f\t%18.16f\t%18.16f\t%s\n", th,sin(th), Sin(th),new_Sin(th),buf);  
        // s = Sin(th); 
        // c = Cos(th); 
        // sum += s * s + c * c; 
    }
    printf("==========================================================================================================\n");  
    printf("sum of error\t%18.13e\t%18.13e\t%18.13e\n", sqrt(mpfr_get_d(ser1,MPFR_RNDN))/(n*90), sqrt(mpfr_get_d(ser2,MPFR_RNDN))/(n*90), sqrt(mpfr_get_d(ser3,MPFR_RNDN))/(n*90)); 
}
