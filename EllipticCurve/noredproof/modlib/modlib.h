#include "intbase.h"

//Add prototype definitions
void anadd(u32bits final[], u32bits ina[], u32bits inb[], u32bits c, int alen, int blen, u32bits mod[], int modlen);
void myadd(u32bits final[], u32bits ina[], u32bits inb[], u32bits c, int alen, int blen, u32bits mod[], int modlen);

//Subtract prototype definitions
void mysub(u32bits final[], u32bits ina[], u32bits inb[], int alength, int blength, u32bits mod[], int modlen);

//Compare Prototype definitions
int compare(u32bits ina[], u32bits inb[], int alength, int blength);

//Modreduce prototype definitions
short compar( register u32bits *r1, register u32bits *r2, int precision);
void modredu( u32bits *result, u32bits * A, u32bits *modulus ,int length, int n);
int intSub(  register u32bits *r1, register u32bits *r2,  int precision);

//Modinverse prototype definitions
void modinv(u32bits final[], u32bits input[], u32bits modnum[], int insize, int modsize);

//Multiply prototype definitions
void fumul(u32bits final[], u32bits ina[], u32bits inb[], int alen, int blen, u32bits mod[], int modlen);
void mamul(u32bits final[], u32bits ina[], u32bits inb[], int alen, int blen, u32bits mod[], int modlen);
void damul(unsigned int *c, unsigned int *a, unsigned int *b, int sizeA, int sizeB);
u32bits smaladd(u32bits final[], u32bits a, u32bits b, u32bits c);
void cleanMult(u32bits result[], u32bits intA[], u32bits intB[], int intASize, int intBSize, u32bits* modulus, int modlen);
void quickmul(u32bits final[], u32bits ina[], u32bits inb[], int alen, int blen, u32bits *modulus, int modlen);