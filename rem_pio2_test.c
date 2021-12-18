#include <stdio.h>
#include <stdlib.h>
#include <math.h>
extern int __rem_pio2(), reduce_sincos(),reduce_sincos_long();
extern double __sin__(), __cos__();

int main(int argc, char * argv[]) {
	double x=atof(argv[1]);
	double d, dd, y[2];
	int n = __rem_pio2(x,y);
    int m;
	if (x<105414336) m= reduce_sincos(x, &d, &dd); else m = reduce_sincos_long(x,&d,&dd); 
	printf("rem_pio2     :%d\t%20.16f\t%20.16e\n",n,y[0],y[1]);
	printf("reduce_sincos:%d\t%20.16f\t%20.16e\n",m,d,dd);
	//u_int32_t ix;
	switch (n&3) {
	case 0: printf("%20.16f\t%20.16f\t%20.16f\n", sin(x), __sin__(y[0], y[1], 1), __sin__(d,dd, 1));break;
	case 1: printf("%20.16f\t%20.16f\t%20.16f\n", sin(x),__cos__(y[0], y[1]),__cos__(d,dd));break;
	case 2: printf("%20.16f\t%20.16f\t%20.16f\n", sin(x), -__sin__(y[0], y[1], 1), - __sin__(d,dd, 1));break;
	default:
		printf("%20.16f\t%20.16f\t%20.16f\n", sin(x), -__cos__(y[0], y[1]), - __cos__(d,dd));break;
	}
}
