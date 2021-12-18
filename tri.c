#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N 100000000 // 1億回ループ
int main(int argc, char * argv[]) {
    int i, k = atoi(argv[1]); //k * π  / 4まで
    double sum1 = 0, sum2=0,s, c, th,omega=k*3.141592653589793238/(4*N);
    for(i = 0; i<100000000; i ++ ) {
        th = i * omega;
        s = sin(th); 
        c = cos(th); 
        sum1 += s ; sum2 += c; 
    }
    printf("%20.16f\t%20.16f\n", sum1*omega,sum2*omega); 
}
