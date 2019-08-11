#include "modlib.h"

static u32bits addspace[3][_WORDSIZE_];
static u32bits hold;
static int finalsize;
void anadd(u32bits final[], u32bits ina[], u32bits inb[], u32bits c, int alen, int blen, u32bits mod[], int modlen){

	for(int i = 0; i < alen; ++i){
		addspace[1][i] = ina[i];
	}
	for(int i = alen; i < _WORDSIZE_; ++i){
		addspace[1][i] = 0;
	}

	for(int i = 0; i < blen; ++i){
		addspace[0][i] = inb[i];
	}
	for(int i = blen; i < _WORDSIZE_; ++i){
		addspace[0][i] = 0;
	}

	for (int i=0; i<_WORDSIZE_; ++i){

		addspace[2][i] = addspace[1][i] + addspace[0][i] + c;

		if(c) {
			c =!(addspace[2][i]>addspace[1][i] && addspace[2][i]>addspace[0][i]);
		}else{
			c = (addspace[2][i]<addspace[1][i]||addspace[2][i]<addspace[0][i]);
		}
	}

	for(int i = 0; i<_WORDSIZE_; i++)
		final[i] = addspace[2][i];
	//modredu(final, addspace[2], mod, _WORDSIZE_, modlen);
}

void myadd(u32bits final[], u32bits ina[], u32bits inb[], u32bits c, int alen, int blen, u32bits mod[], int modlen){
	for(int i = 0; i < alen; ++i){
		addspace[1][i] = ina[i];
	}
	for(int i = alen; i < _WORDSIZE_; ++i){
		addspace[1][i] = 0;
	}

	for(int i = 0; i < blen; ++i){
		addspace[0][i] = inb[i];
	}
	for(int i = blen; i < _WORDSIZE_; ++i){
		addspace[0][i] = 0;
	}

	for(int i = 0; i < _WORDSIZE_; ++i){
		addspace[2][i] = (c&_LOWMASK_) + (addspace[1][i]&_LOWMASK_) + (addspace[0][i]&_LOWMASK_);
		hold = ((addspace[2][i]>>_SHIFT_)&_LOWMASK_) + ((addspace[1][i]>>_SHIFT_)&_LOWMASK_) + ((addspace[0][i]>>_SHIFT_)&_LOWMASK_) +((c>>_SHIFT_)&_LOWMASK_);
		addspace[2][i] = (addspace[2][i]&_LOWMASK_) + (hold<<_SHIFT_);
		c = (hold>>_SHIFT_)&_LOWMASK_;
	}
	
	for(int i = 0; i<_WORDSIZE_; i++)
		final[i] = addspace[2][i];
	//modredu(final, addspace[2], mod, _WORDSIZE_, modlen);
}

//Deprecated:
//
//u32bits myadd(u32bits final[], u32bits ina[], u32bits inb[], u32bits c, int alen, int blen){
//	addspace[3][0] = c;
//
//	for (int i = 0; i<_WORDSIZE_; ++i){
//		addspace[0][2*i] = ina[i]&0xFFFF;
//		addspace[0][2*i+1] = (ina[i]>>16)&0xFFFF;
//		addspace[1][2*i] = inb[i]&0xFFFF;
//		addspace[1][2*i+1] = (inb[i]>>16)&0xFFFF;
//	}
//	
//	for (int i = 0; i< 2*_WORDSIZE_; ++i){
//		addspace[2][i] = addspace[0][i] + addspace[1][i] + addspace[3][0];
//		addspace[3][0] = (addspace[2][i]>>16)&0xFFFF;
//	}
//
//	for (int i = 0; i<_WORDSIZE_; i++){
//		final[i] = (addspace[2][2*i]&0xFFFF) + (addspace[2][2*i+1]<<16);
//	}
//
//	return addspace[3][0];
//}