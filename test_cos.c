#include <stdio.h>
#include <stdlib.h>
#include <math.h>
extern double My_cos();
int main(int argc, char * argv[]) {
	double x;
		x=atof(argv[1])*3.1415926535897932/180.0;
		printf("glibc:%20.17f\tmy cosin:%20.17f\n",cos(x),My_cos(x));
}	
