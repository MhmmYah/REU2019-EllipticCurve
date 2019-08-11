#include "../modlib/modlib.h"

#ifndef _CURVELIB_
#define _CURVELIB_
    struct CurvePoint{
        u32bits* x;
        u32bits* y;
        int xsize;
        int ysize;
    };

#endif

//Curve addition
void eadd(struct CurvePoint *final, struct CurvePoint *ina, struct CurvePoint *inb, u32bits* mod, int modlen);

//Curve doubling
void eCurveDouble(struct CurvePoint* result, struct CurvePoint inP, u32bits a[], int lenA, u32bits modulus[], int modlen);

//Scalar Multiplication
void bmult(struct CurvePoint *final, struct CurvePoint *ina, u32bits a[], u32bits secret[], u32bits mod[], int alen, int slen, int modlen);