#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>
#include <mpfr.h>
extern double Sin(), Cos(); 
int main(int argc, char * argv[]) {
    int i;
    long j,base = atol(argv[1]); 
    double sum = 0, s, c, th;
    mpfr_t theta, ans; 
    mpfr_set_default_prec(2048);
    mpfr_init(theta), mpfr_init(ans); 
    char buf[1024];
    for(i = 0; i<180; i ++ ) {
        j=base+i;
        //th = j * 3.14159265358979 / 180;
        th=(double)j;
        //mpfr_const_pi(theta, MPFR_RNDN); mpfr_mul_si(theta, theta, j, MPFR_RNDN); mpfr_div_si(theta, theta, 180, MPFR_RNDN); 
        mpfr_set_si(theta,j,MPFR_RNDF);
        //th=mpfr_get_d(theta,MPFR_RNDN);
        mpfr_sin(ans, theta,MPFR_RNDN); mpfr_sprintf(buf,"%.16Rg",(mpfr_ptr)ans);
        printf("%ld\t%18.16f\t%18.16f\t%s\n", j,sin(th), Sin(th),buf);  
        // s = Sin(th); 
        // c = Cos(th); 
        // sum += s * s + c * c; 
    }
    // printf("%f\n", sum); 
}
