
#include "modlib.h"

static u32bits carry, hold;
static u32bits subspace[4][_WORDSIZE_];
static int finalsize;

void mysub(u32bits final[], u32bits ina[], u32bits inb[], int alen, int blen, u32bits mod[], int modlen){

	for (int i = 0; i <  alen; i++){
		subspace[0][i] = ina[i];
		subspace[2][i] = 0;
	}
	for (int i = alen; i <  _WORDSIZE_; i++){
		subspace[0][i] = 0;
		subspace[2][i] = 0;
	}

	for (int i = 0; i <  blen; i++)
		subspace[1][i] = ~inb[i];
	for (int i = blen; i <  _WORDSIZE_; ++i ){
		subspace[1][i] = 0;
		subspace[1][i] = ~subspace[1][i];
	}

	carry = 1;
	for (int i=0; i<_WORDSIZE_; ++i){
		hold = subspace[1][i] + subspace[2][i] + carry;

		if(carry) {
			carry =!(hold>subspace[1][i] && hold>subspace[2][i]);
		}else{
			carry = (hold<subspace[1][i] || hold<subspace[2][i]);
		}

		subspace[1][i] = hold;
	}

	carry = 0;
	for (int i=0; i<_WORDSIZE_; ++i){

		subspace[2][i] = subspace[1][i] + subspace[0][i] + carry;

		if(carry) {
			carry =!(subspace[2][i]>subspace[1][i] && subspace[2][i]>subspace[0][i]);
		}else{
			carry = (subspace[2][i]<subspace[1][i]||subspace[2][i]<subspace[0][i]);
		}
	}

	if(carry == 0 ){
		for(int i = 0; i <  modlen; i++)
			subspace[0][i] = mod[i];
		for (int i = modlen; i < _WORDSIZE_; i++)
			subspace[0][i] = 0;
		for (int i=0; i<_WORDSIZE_; ++i){
			hold = subspace[2][i] + subspace[0][i] + carry;

			if(carry)
				carry = !(hold > subspace[0][i] && hold > subspace[2][i]);
			else
				carry = (hold < subspace[0][i] || hold < subspace[2][i]);
			subspace[2][i] = hold;
		}
	}

	for(int i = 0; i < _WORDSIZE_; i++)
		final[i] = subspace[2][i];
	//modredu(final, subspace[2], mod, _WORDSIZE_, modlen);
}
