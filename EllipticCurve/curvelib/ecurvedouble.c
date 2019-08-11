#include "ecurvelib.h"

void multiplyThree(u32bits result[], u32bits ina[], int reslen, int len,
                   u32bits* modulus, int modlen) {
	int i, carry[2];
	carry[0] = 0;
	for (i = 0; i < reslen; i++) {
		carry[1] = (ina[i] & _ONEMASK_) / _ONEMASK_;
		result[i] = ina[i] << 1;
		result[i] += carry[0];
		carry[0] = carry[1];
	}
	if (carry[0]) {
		result[i] = 1;
		reslen++;
		result[i + 1] = 0;
	}
	else{
		result[i] = 0;
	}
    anadd(result, result, ina, 0, reslen, len, modulus, modlen);
}

void shift(u32bits result[], u32bits ina[], int lenRes, u32bits* modulus, int modlen) {
    int i, carry[2];
    carry[0] = 0;
    for(i = 0; i < lenRes; i++) {
        carry[1] = (ina[i] & _ONEMASK_) / _ONEMASK_;
        result[i] = ina[i] << 1;
        result[i] += carry[0];
        carry[0] = carry[1];
    }
    if(carry[0]) {
        result[i] = 1;
		lenRes++;
		result[i+1] = 0;
    }
	else{
		result[i] = 0;
	}
    modredu(result, result, modulus, lenRes, modlen);
}

void eCurveDouble(struct CurvePoint* result, struct CurvePoint inP, u32bits a[], int lenA, u32bits modulus[], int modlen){
    //s is slope
    //inverse is used to hold 2yp and 2xp
    u32bits s[_WORDSIZE_];
	u32bits inverse[_WORDSIZE_];
	u32bits hold[_WORDSIZE_];
	u32bits xval[_WORDSIZE_];
	u32bits yval[_WORDSIZE_];
	int  i;
    
    //equation to find slope
    shift(inverse, inP.y, inP.ysize, modulus, modlen);
    modinv(inverse, inverse, modulus, modlen, modlen);
    quickmul(s, inP.x, inP.x, inP.xsize, inP.xsize, modulus, modlen);
    multiplyThree(hold, s, modlen, modlen, modulus, modlen);
    anadd(s, a, hold, 0, lenA, modlen, modulus, modlen);
    quickmul(s, s, inverse, modlen, modlen, modulus, modlen);
    
    //equation to find xr
    quickmul(xval, s, s, modlen, modlen, modulus, modlen);
    shift(inverse, inP.x, inP.xsize, modulus, modlen);
    mysub(xval, xval, inverse, modlen, modlen, modulus, modlen);
    
    //equation to find yr
    mysub(yval, inP.x, xval, modlen, modlen, modulus, modlen);
    quickmul(yval, s, yval, modlen, modlen, modulus, modlen);
    mysub(yval, yval, inP.y, modlen, modlen, modulus, modlen);

	for (i = 0; i < modlen; i++){
		(*result).x[i] = xval[i];
		(*result).y[i] = yval[i];
	}
}