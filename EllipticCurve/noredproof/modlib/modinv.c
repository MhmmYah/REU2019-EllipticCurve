#include "modlib.h"

static u32bits inspace[2][_WORDSIZE_];
static u32bits mask = _ONEMASK_;

void modinv(u32bits final[], u32bits input[], u32bits modnum[], int insize, int modsize){

	final[0] = 1;
	for(int i = 1; i< modsize; i++)
		final[i] = 0;
	
	u32bits twoval[1] = {2};
	mysub(inspace[0], modnum, twoval, modsize, 1, modnum, modsize);

	int curword = modsize -1;
	while(inspace[0][curword] == 0)
		--curword;

	while(inspace[0][curword]&mask)
		mask = mask/2;
	
	while(mask != 0){
		mamul(final, final, final, modsize, modsize, modnum, modsize);
		if(mask&inspace[0][curword])
			mamul(final, final, input, modsize, insize, modnum, modsize);
		mask = mask /2;
	}
	--curword;

	while(curword>=0){
		mask = _ONEMASK_;
		while(mask != 0){
			mamul(final, final, final, modsize, modsize, modnum, modsize);
			if(mask&inspace[0][curword])
				mamul(final, final, input, modsize, insize, modnum, modsize);
			mask = mask /2;
		}
		--curword;
	}
}