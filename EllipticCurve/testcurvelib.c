#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "curvelib/ecurvelib.h"
#define _LARGEST_ 7
#define _TRIALSIZE_ 1

int main(){
	struct CurvePoint final, ina, inb;
	clock_t start, stop;
	double timed;
	srand(time(0));

	u32bits mod[] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0};
	u32bits inax[] = {0x6982A888, 0x29A70FB1, 0x1588A3F6, 0xD3553463, 0x28783F2A, 0xDAFEBF58, 0x0};
	u32bits inay[] = {0x5C7E93AB, 0x59331AFA, 0x141B868F, 0x46B27BBC, 0x993DA0FA, 0xDD6BDA0D, 0x0};
	u32bits inbx[] = {0x82FF1012, 0xF4FF0AFD, 0x43A18800, 0x7CBF20EB, 0xB03090F6, 0x188DA80E, 0x0};
	u32bits inby[] = {0x1E794811, 0x73F977A1, 0x6B24CDD5, 0x631011ED, 0xFFC8DA78, 0x07192B95, 0x0};
	u32bits finalx[7] = {0x0};
	u32bits finaly[7] = {0x0};
	int modsize = 6;

	final.x = finalx;
	final.y = finaly;
	ina.x = inax;
	ina.y = inay;
	inb.x = inbx;
	inb.y = inby;
	final.xsize = final.ysize = ina.xsize = ina.ysize = inb.xsize = inb.ysize = 7;

	eadd(&final, &inb, &ina, mod, modsize);

	printf("final X val: \n");
	for(int i = 6; i  >= 0; i--)
		printf("%08x, ", finalx[i]);
	printf("\n\n");
	printf("final Y val: \n");
	for(int i = 6; i  >= 0; i--)
		printf("%08x, ", finaly[i]);
	printf("\n\n");

}