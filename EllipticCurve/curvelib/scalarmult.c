#include "ecurvelib.h"

static int curword;
u32bits mask;

void bmult(struct CurvePoint *final, struct CurvePoint *ina, u32bits a[], u32bits secret[], u32bits mod[], int alen, int slen, int modlen){
	mask = _ONEMASK_;
	curword = slen-1;

	/*
	(*final).x = 0;
	(*final).y = 0;
	(*final).xsize = 1;
	(*final).ysize = 1;
	*/

	while(!(secret[curword]))
		--curword;

	while(!(secret[curword]&mask))
		mask = mask/2;

	for (int i = 0; i < modlen; i++){
		final->x[i] = ina->x[i];
		final->y[i] = ina->y[i];
	}
	final->x[modlen] = 0;
	final->y[modlen] = 0;

	mask = mask/2;		//process 1 leading bit


	while(mask){
		eCurveDouble(final, *final, a, alen, mod, modlen);
		if(secret[curword]&mask)
			eadd(final, final, ina, mod, modlen);
		mask = mask/2;
	}
	-- curword;

	while(curword>=0){
		mask = _ONEMASK_;
		while(mask){
			eCurveDouble(final, *final, a, alen, mod, modlen);

			if(mask&secret[curword])
				eadd(final, final, ina, mod, modlen);

			mask = mask /2;
		}
		--curword;
	}
	
}