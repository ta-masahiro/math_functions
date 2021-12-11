#include <stdio.h>
#include <math.h>
int main(int argc, char * argv[]) {
    int i;
    double sum1 = 0, sum2=0,s, c, th,omega=3.141592653589793238/200000000;
    for(i = 0; i<100000000; i ++ ) {
        th = i * omega;
        s = sin(th); 
        c = cos(th); 
        sum1 += s ; sum2 += c; 
    }
    printf("%20.16f\t%20.16f\n", sum1*omega,sum2*omega); 
}
