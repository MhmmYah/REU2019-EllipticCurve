#include "modlib.h"

#define SMALADD(F,A,B,C){\
	wkspace[0][0] = A&0xFFFF;\
	wkspace[0][1] = (A>>16)&0xFFFF;\
	wkspace[1][0] = B&0xFFFF;\
	wkspace[1][1] = (B>>16)&0xFFFF;\
	wkspace[2][0] = C&0xFFFF;\
	wkspace[2][1] = (C>>16)&0xFFFF;\
	wkspace[3][0] = wkspace[0][0] + wkspace[1][0] + wkspace[2][0];\
	wkspace[3][1] = wkspace[0][1] + wkspace[1][1] + wkspace[2][1] + ((wkspace[3][0]>>16)&0xFFFF);\
	F = (wkspace[3][0]&0xFFFF) + (wkspace[3][1]<<16);\
	wkspace[5][0] = (wkspace[3][1]>>16)&0xFFFF;\
}

#define MULTIPLY(A, B) {                \
    if ((A) == 0 || (B) == 0) {         \
        lower32 = 0;                    \
        upper32 = 0;                    \
    } else {                            \
        lowA = (A) & 0xffff;                \
        highA = ((A) >> 16) & 0xffff;       \
        lowB = (B) & 0xffff;                \
        highB = ((B) >> 16) & 0xffff;       \
        tempLower = lowA * lowB;            \
        upper32 = highA * highB;            \
        lowAhighB = lowA * highB;           \
        highAlowB = highA * lowB;           \
        middle32 = lowAhighB + highAlowB;   \
        if((unsigned int)middle32 < (unsigned int)lowAhighB)     \
            upper32 += 0x10000;                          \
        upperMiddle = (middle32 >> 16) & 0xffff;         \
        lowerMiddle = middle32 << 16;       \
        upper32 += upperMiddle;             \
        lower32 = tempLower + lowerMiddle;  \
        if (((unsigned)lower32 < (unsigned)tempLower) || ((unsigned)lower32 < (unsigned)lowerMiddle)) \
            upper32++;                      \
    }                                       \
}

static u32bits wkspace[11][2*_WORDSIZE_];
static int finalsize;

void mamul(u32bits final[], u32bits ina[], u32bits inb[], int alen, int blen, u32bits mod[], int modlen){
	wkspace[4][0] = 0;				//for forward carry
	wkspace[4][1] = 0;				//for forward outcarry
	wkspace[5][0] = 0;				//for backward carry
	wkspace[5][1] = 0;				//for backward outcarry

	for (int i = 0; i<alen; ++i){
		wkspace[6][2*i] = ina[i]&0xFFFF;
		wkspace[6][2*i+1] = (ina[i]>>16)&0xFFFF;
		wkspace[8][2*i] = 0;
		wkspace[8][2*i+1] = 0;
	}
	for (int i = alen; i<_WORDSIZE_; ++i){
		wkspace[6][2*i] = 0;
		wkspace[6][2*i+1] = 0;
		wkspace[8][2*i] = 0;
		wkspace[8][2*i+1] = 0;
	}
	for (int i = 0; i<blen; ++i){
		wkspace[7][2*i] = inb[i]&0xFFFF;
		wkspace[7][2*i+1] = (inb[i]>>16)&0xFFFF;
		wkspace[9][2*i] = 0;
		wkspace[9][2*i+1] = 0;
	}
	for (int i = blen; i<_WORDSIZE_; ++i){
		wkspace[7][2*i] = 0;
		wkspace[7][2*i+1] = 0;
		wkspace[9][2*i] = 0;
		wkspace[9][2*i+1] = 0;
	}

	for(int i=0; i<2*_WORDSIZE_; ++i){
		for(int j=0; j<=i; ++j){
			SMALADD(wkspace[8][i], wkspace[6][j]*wkspace[7][i-j], wkspace[8][i], wkspace[4][1]);
			wkspace[4][0] = wkspace[4][0] + wkspace[5][0];
			wkspace[4][1] = 0;
		}

		wkspace[4][1] = wkspace[4][0]<<16;

		wkspace[4][0] = 0;	
	}

	for(int i=1; i<2*_WORDSIZE_; ++i){
		for(int j=(2*_WORDSIZE_)-1; j>=i ; --j){
			SMALADD(wkspace[9][i-1], wkspace[6][j]*wkspace[7][(2*_WORDSIZE_)-1-j+1], wkspace[9][i-1], wkspace[4][1]);
			wkspace[4][0] = wkspace[4][0] + wkspace[5][0];
			wkspace[4][1] = 0;
		}
		wkspace[4][1] = wkspace[4][0]<<16;
		wkspace[4][0] = 0;

	

	}

	for(int i = 0; i<_WORDSIZE_; ++i){
		SMALADD(wkspace[10][i], wkspace[8][2*i], wkspace[8][2*i+1]<<16, wkspace[4][0]);
		wkspace[4][0] = wkspace[5][0] + (wkspace[8][2*i+1]>>16)&0xFFFF;
	}
	
	for(int i=0; i<_WORDSIZE_; ++i){
		SMALADD(wkspace[10][i+_WORDSIZE_], wkspace[9][2*i], wkspace[9][2*i+1]<<16, wkspace[4][0]);
		wkspace[4][0] = wkspace[5][0] + (wkspace[9][2*i+1]>>16)&0xFFFF;
	}

    modredu(final, wkspace[10], mod, alen+blen, modlen);
}

void fumul(u32bits final[], u32bits ina[], u32bits inb[], int alen, int blen, u32bits mod[], int modlen){
	wkspace[4][0] = 0;				//for forward carry
	wkspace[4][1] = 0;				//for forward outcarry
	wkspace[5][0] = 0;				//for backward carry
	wkspace[5][1] = 0;				//for backward outcarry

	for (int i = 0; i<_WORDSIZE_; ++i){
		wkspace[6][2*i] = ina[i]&0xFFFF;
		wkspace[6][2*i+1] = (ina[i]>>16)&0xFFFF;
		wkspace[7][2*i] = inb[i]&0xFFFF;
		wkspace[7][2*i+1] = (inb[i]>>16)&0xFFFF;
		wkspace[8][2*i] = 0;
		wkspace[8][2*i+1] = 0;
		wkspace[9][2*i] = 0;
		wkspace[9][2*i+1] = 0;
	}

	for(int i=0; i<2*_WORDSIZE_; ++i){
		for(int j=0; j<=i; ++j){

			wkspace[4][0] = 
				wkspace[4][0] + smaladd(
					&wkspace[8][i], wkspace[6][j]*wkspace[7][i-j], wkspace[8][i], wkspace[4][1]);
			wkspace[4][1] = 0;
		}

		wkspace[4][1] = wkspace[4][0]<<16;

		wkspace[4][0] = 0;	
	}

	for(int i=1; i<2*_WORDSIZE_; ++i){
		for(int j=(2*_WORDSIZE_)-1; j>=i ; --j){
			wkspace[4][0] = 
				wkspace[4][0] + smaladd(
					&wkspace[9][i-1], wkspace[6][j]*wkspace[7][(2*_WORDSIZE_)-1-j+1], wkspace[9][i-1], wkspace[4][1]);
			wkspace[4][1] = 0;
		}
		wkspace[4][1] = wkspace[4][0]<<16;
		wkspace[4][0] = 0;

	

	}

	for(int i = 0; i<_WORDSIZE_; ++i){
		wkspace[4][0] = smaladd(
			&wkspace[10][i], wkspace[8][2*i], wkspace[8][2*i+1]<<16, wkspace[4][0]);
		wkspace[4][0] = wkspace[4][0] + (wkspace[8][2*i+1]>>16)&0xFFFF;
	}
	
	for(int i=0; i<_WORDSIZE_; ++i){
		wkspace[4][0] = smaladd(
			&wkspace[10][i+_WORDSIZE_], wkspace[9][2*i], wkspace[9][2*i+1]<<16, wkspace[4][0]);
		wkspace[4][0] = wkspace[4][0] + (wkspace[9][2*i+1]>>16)&0xFFFF;
	}

	modredu(final, wkspace[10], mod, alen+blen, modlen);
}

void damul(unsigned int *c, unsigned int *a, unsigned int *b, int sizeA, int sizeB)
{
    //variable declarations (note: it may be useful to make some of these variables static)
    static unsigned int lowA, highA, lowB, highB, lowAhighB, highAlowB;                    //used in single 32 bit calc
    static unsigned int lower32, tempLower, middle32, upperMiddle, lowerMiddle, upper32;    //used in single 32 bit calc
    int i, j, carryIn, carryOut;
 
    for (i = 0; i < sizeA + sizeB; i++)
        c[i] = 0;                        //clearing out the answer array
 
    for (i = 0; i < sizeA; i++) {
        carryOut = 0;
        for ( j = 0; j < sizeB; j++) {
            carryIn = carryOut;
            MULTIPLY(a[i], b[j]);
            c[i + j] += lower32;
            if ((unsigned)c[i + j] < (unsigned)lower32)
                upper32++;
            c[i + j + 1] += upper32 + carryIn;
            carryOut = ((unsigned)c[i+j+1] < (unsigned)upper32) ? 1 : 0;
        } //end of inner for loop, which cycles through b's blocks and multiplies against a
		 while (carryOut) {
            c[i+j+1]++;
            carryOut = !c[i+j+1];
            j++;
        } //end of while loop
    } //end of outer for loop, which cycles through a's groupings
    return;
} //end of elementary multiplier function

u32bits smaladd(u32bits final[], u32bits a, u32bits b, u32bits c){

	wkspace[0][0] = a&0xFFFF;				//a0
	wkspace[0][1] = (a>>16)&0xFFFF;			//a1
	wkspace[1][0] = b&0xFFFF;				//b0
	wkspace[1][1] = (b>>16)&0xFFFF;			//b1
	wkspace[2][0] = c&0xFFFF;				//c0
	wkspace[2][1] = (c>>16)&0xFFFF;			//c1
	wkspace[3][0] = wkspace[0][0] + wkspace[1][0] + wkspace[2][0]; //a0 + b0 + c0
	wkspace[3][1] = wkspace[0][1] + wkspace[1][1] + wkspace[2][1] + ((wkspace[3][0]>>16)&0xFFFF); 

	final[0] = (wkspace[3][0]&0xFFFF) + (wkspace[3][1]<<16);
	return (wkspace[3][1]>>16)&0xFFFF;

}


void cleanMult(u32bits result[], u32bits intA[], u32bits intB[], int intASize, int intBSize, u32bits* modulus, int modlen) {
    u32bits carryOut[3];
	u32bits othertemp[2*_WORDSIZE_];
    u32bits holder[4];
    u32bits work[3];
    u32bits hold[2];
    u32bits temp[3*_WORDSIZE_];
    for(int i = 0; i < intASize+intBSize; i++) {
         othertemp[i] = 0;
        temp[i] = 0;
    }
    carryOut[2] = 0;
    for(int i = 0; i < intASize; i++) {
        for(int j = 0; j < intBSize; j++) {
            //Seperate the two numbers into halves
            holder[0] = (intA[i] & 0xFFFF0000) >> 16;
            holder[1] = (intA[i] & 0xFFFF);
            holder[2] = (intB[j] & 0xFFFF0000) >> 16;
            holder[3] = (intB[j] & 0xFFFF);
            
            //Get upper, lower, and middle respectively
            work[0] = holder[0]*holder[2];
            work[1] = holder[1]*holder[3];
            work[2] = ((holder[0]*holder[3]) + (holder[1]*holder[2]));
            
            //lower 32 bits of the product
            hold[0] = work[1] + ((work[2] & 0xFFFF)<<16);
            
            carryOut[0] = (work[2] < (holder[0]*holder[3])) ||
            (work[2] < (holder[1]*holder[2])) ? 1 : 0;
            carryOut[1] = ((hold[0] < work[1]) ||
                           (hold[0] < ((work[2] << 2) & 0xFFFF))) ? 1 : 0;
            
            //upper 32 bits of the product
            hold[1] = work[0] + ((work[2]& 0xFFFF0000)>>16) + carryOut[1] +
            (carryOut[0] << 16);
            
            //            stored in temp;
            temp[i+j+1] += (( othertemp[i+j]+hold[0] < hold[0])||
                            ( othertemp[i+j]+hold[0] <  othertemp[i+j])) ? 1 : 0;
             othertemp[i+j] += hold[0];
            temp[i+j+2] += ((( othertemp[i+j+1] + hold[1]) <  othertemp[i+j+1])
                            || (( othertemp[i+j+1] + hold[1]) < hold[1]))
            ? 1 : 0;
             othertemp[i+j+1] += hold[1];
        }
    }
    for(int i = 0; i < intASize+intBSize; i++) {
         othertemp[i] += carryOut[2];
        carryOut[2] = (( othertemp[i]+temp[i] <  othertemp[i]) ||
                        othertemp[i]+temp[i] < temp[i]) ? 1 : 0;
         othertemp[i] =  othertemp[i] + temp[i];
    }
//    printf("This number is: ");
//    for(int x = intASize+intBSize-1; x  >= 0; x--) {
//        printf("%08x/", result[x]);
//    }
//    printf("\n\n");
modredu(result,  othertemp, modulus, intASize+intBSize-1, modlen);
}

void quickmul(u32bits final[], u32bits ina[], u32bits inb[], int alen, int blen, u32bits *modulus, int modlen){
	
	u64bits hold;
	u32bits temp, carry, *ptr;

	for(int i = 0; i < alen+blen; i++)
		wkspace[0][i] = 0;

	for(int i = 0;  i< alen; i++){
		for(int j = 0; j < blen; j++){
			ptr = &wkspace[0][i+j];
			hold = (u64bits)ina[i] * (u64bits)inb[j];
			temp = *ptr + (hold&_FULLMASK_);
			carry = (temp < *ptr || temp < (hold&_FULLMASK_));
			*ptr = temp;
			++ptr;
			temp = *ptr + ((hold>>32)&_FULLMASK_) + carry;
			if(carry)
				carry = !(temp > *ptr && temp > ((hold>>32)&_FULLMASK_));
			else
				carry = (temp < *ptr || temp < ((hold>>32)&_FULLMASK_));
			*ptr = temp;
			while(carry){
				++ptr;
				temp = *ptr + 1;
				carry = !(temp > *ptr);
				*ptr = temp;
			}
		}
	}
	//for(int i = 0; i  < alen+blen; i++)
	//	final[i] = wkspace[0][i];
   modredu(final, wkspace[0], modulus, alen + blen, modlen);
}
