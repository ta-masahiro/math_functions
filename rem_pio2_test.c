#include <stdio.h>
#include <stdlib.h>
#include <math.h>
extern int __rem_pio2();
extern double __sin__(), __cos__();

int main(int argc, char * argv[]) {
	double x=atof(argv[1]);
	double y[2];
	int n=__rem_pio2(x,y);
	printf("%d\t%20.16f\t%20.16e\n",n,y[0],y[1]);
	//u_int32_t ix;
	switch (n&3) {
	case 0: printf("%20.16f\t%20.16f\t%20.16f\n", sin(x), __sin__(y[0], y[1], 1), __sin__(y[0],0,0));break;
	case 1: printf("%20.16f\t%20.16f\t%20.16f\n", sin(x),__cos__(y[0], y[1]),__cos__(y[0],0));break;
	case 2: printf("%20.16f\t%20.16f\t%20.16f\n", sin(x), -__sin__(y[0], y[1], 1), - __sin__(y[0],0,0));break;
	default:
		printf("%20.16f\t%20.16f\t%20.16f\n", sin(x), -__cos__(y[0], y[1]), - __cos__(y[0],0));break;
	}
}
