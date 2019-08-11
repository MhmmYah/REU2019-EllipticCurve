#include "ecurvelib.h"

u32bits eddspace[2][_WORDSIZE_];
u32bits holdx[_WORDSIZE_];

void eadd(struct CurvePoint *final, struct CurvePoint *ina, struct CurvePoint *inb, u32bits mod[], int modlen){
	int counter;

	mysub(eddspace[0], (*ina).x, (*inb).x, (*ina).xsize, (*inb).xsize, mod, modlen);
	modinv(eddspace[1], eddspace[0], mod, modlen, modlen);
	mysub(eddspace[0], (*ina).y, (*inb).y, (*ina).ysize, (*inb).ysize, mod, modlen);
	cleanMult(eddspace[0], eddspace[1], eddspace[0], modlen, modlen, mod, modlen);
	
	//eddspace[0] holds the slope.

	cleanMult(eddspace[1], eddspace[0], eddspace[0], modlen, modlen, mod, modlen);
	mysub(eddspace[1], eddspace[1], (*ina).x, modlen, (*ina).xsize, mod, modlen);
	mysub(eddspace[1], eddspace[1], (*inb).x, modlen, (*inb).xsize, mod, modlen);
	
	counter = modlen;
	while((eddspace[1][counter-1]==0)&&(counter>0))
		--counter;
	(*final).xsize = counter;
	for(int i = modlen; i >= 0; i--)
		holdx[i] = eddspace[1][i];

	// x value and size calculated and stored into the struct.

	mysub(eddspace[1], (*ina).x, eddspace[1], (*ina).xsize, counter, mod, modlen);
	cleanMult(eddspace[1], eddspace[0], eddspace[1], modlen, counter, mod, modlen);
	mysub(eddspace[0], eddspace[1], (*ina).y, counter, (*ina).ysize, mod, modlen);

	for(int i = modlen; i >= 0; i--)
		(*final).x[i] = holdx[i];

	counter = modlen;
	while((eddspace[0][counter-1]==0)&&(counter>0))
		--counter;
	(*final).ysize = counter;
	for(int i = modlen; i >= 0; i--)
		(*final).y[i] = eddspace[0][i];
}