#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "modlib/modlib.h"
#define _LARGEST_ 12
#define _TRIALSIZE_ 1

int main(){
	u32bits final[2*_LARGEST_], ina[_LARGEST_], inb[_LARGEST_], mod[_LARGEST_];
	srand(time(0));
	clock_t start, stop;
	double timed;
	int compareresult;

	for(int i = 0; i  < 2*_LARGEST_; i++)
		final[i] = 0;

	for(int i = 0; i<_LARGEST_; i++){

/*/For random numbers ina and inb
		ina[i] =((rand()&0xFF)<<24) + ((rand()&0xFF)<<16) + 
				((rand()&0xFF)<<8) + (rand()&0xFF);
		inb[i] =((rand()&0xFF)<<24) + ((rand()&0xFF)<<16) + 
				((rand()&0xFF)<<8) + (rand()&0xFF);
		mod[i] =((rand()&0xFF)<<24) + ((rand()&0xFF)<<16) + 
				((rand()&0xFF)<<8) + (rand()&0xFF);
// */
/*/For addition checking. Need 
		ina[i] =((rand()&0xFF)<<24) + ((rand()&0xFF)<<16) + 
				((rand()&0xFF)<<8) + (rand()&0xFF);
		inb[i] =~ina[i];
// */
//For defined values
		ina[i] =0xFFFFFFFF;	//32 1's
		inb[i] =0xFFFFFFFF;	//32 1's
		mod[i] =0;	//32 1's
// */
	}
//----anadd--------------------------------------------------

	anadd(final, ina, inb, 1, _LARGEST_, _LARGEST_, mod, _LARGEST_);
	
	for(int i=2*_LARGEST_-1; i>=0; --i){
		printf("%x, ", final[i]);
	}
	printf("ANDADD \n");
	
	start = clock();
	for(int i = 0; i < _TRIALSIZE_; ++i){
		anadd(final, ina, inb, 1, _LARGEST_, _LARGEST_, mod, _LARGEST_);
	}
	stop = clock();

	timed = (double)(stop-start)/(((double)_TRIALSIZE_));
	printf("ClockCycle: %f. \n", timed);

//-----myadd-------------------------------------------------

	myadd(final, ina, inb, 1, _LARGEST_, _LARGEST_, mod, _LARGEST_);

	for(int i=2*_LARGEST_-1; i>=0; --i){
		printf("%x, ", final[i]);
	}
	printf("MYADD \n");

	start = clock();
	for(int i = 0; i < _TRIALSIZE_; ++i){
		myadd(final, ina, inb, 1, _LARGEST_, _LARGEST_, mod, _LARGEST_);
	}

	stop = clock();
	timed = (double)(stop-start)/((double)_TRIALSIZE_);
	printf("ClockCycle: %f. \n", timed);

//------mysub------------------------------------------------

	mysub(final, ina, inb, _LARGEST_, _LARGEST_, mod, _LARGEST_);

	for(int i=2*_LARGEST_-1; i>=0; --i){
		printf("%x, ", final[i]);
	}
	printf("MYSUB \n");

	start = clock();
	for(int i = 0; i < _TRIALSIZE_; ++i){
		mysub(final, ina, inb, _LARGEST_, _LARGEST_, mod, _LARGEST_);
	}
	stop = clock();
	timed = (double)(stop-start)/((double)_TRIALSIZE_);
	printf("ClockCycle: %f. \n", timed);

//-----fumul-------------------------------------------------

	fumul(final, ina, inb, _LARGEST_, _LARGEST_, mod, _LARGEST_);

	for(int i=2*_LARGEST_-1; i>=0; --i){
		printf("%x, ", final[i]);
	}
	printf("FUMUL \n");

	start = clock();
	for(int i = 0; i < _TRIALSIZE_; ++i){
		fumul(final, ina, inb, _LARGEST_, _LARGEST_, mod, _LARGEST_);
	}
	stop = clock();
	timed = (double)(stop-start)/((double)_TRIALSIZE_);
	printf("ClockCycle: %f. \n", timed);

//-----mamul-------------------------------------------------

	mamul(final, ina, inb, _LARGEST_, _LARGEST_, mod, _LARGEST_);

	for(int i=2*_LARGEST_-1; i>=0; --i){
		printf("%x, ", final[i]);
	}
	printf("MAMUL \n");

	start = clock();
	for(int i = 0; i < _TRIALSIZE_; ++i){
		mamul(final, ina, inb,  _LARGEST_, _LARGEST_, mod, _LARGEST_);
	}
	stop = clock();
	timed = (double)(stop-start)/((double)_TRIALSIZE_);
	printf("ClockCycle: %f. \n", timed);

//-----damul-------------------------------------------------

	damul(final, ina, inb, _LARGEST_, _LARGEST_);

	for(int i=2*_LARGEST_-1; i>=0; --i){
		printf("%x, ", final[i]);
	}
	printf("DAMUL \n");

	start = clock();
	for(int i = 0; i < _TRIALSIZE_; ++i){
		damul(final, ina, inb, _LARGEST_, _LARGEST_);
	}
	stop = clock();
	timed = (double)(stop-start)/((double)_TRIALSIZE_);
	printf("ClockCycle: %f. \n", timed);

//----Cleanmult---------------------------------------------

	cleanMult(final, ina, inb, _LARGEST_, _LARGEST_, mod, _LARGEST_);

	for(int i=2*_LARGEST_-1; i>=0; --i){
		printf("%x, ", final[i]);
	}
	printf("CleanMult \n");

	start = clock();
	for(int i = 0; i < _TRIALSIZE_; ++i){
		cleanMult(final, ina, inb, _LARGEST_, _LARGEST_, mod, _LARGEST_);
	}
	stop = clock();
	timed = (double)(stop-start)/((double)_TRIALSIZE_);
	printf("ClockCycle: %f. \n", timed);

//----quickmult---------------------------------------------

	quickmul(final, ina, inb, _LARGEST_, _LARGEST_, mod, _LARGEST_);

	for(int i=2*_LARGEST_-1; i>=0; --i){
		printf("%x, ", final[i]);
	}
	printf("quickmul \n");

	start = clock();
	for(int i = 0; i < _TRIALSIZE_; ++i){
		quickmul(final, ina, inb, _LARGEST_, _LARGEST_, mod, _LARGEST_);
	}
	stop = clock();
	timed = (double)(stop-start)/((double)_TRIALSIZE_);
	printf("ClockCycle: %f. \n", timed);
//-----compare-------------------------------------------------

	compareresult = compare(ina, inb, _LARGEST_, _LARGEST_);

	printf("COMPARE Answer = %d. \n", compareresult);

	start = clock();
	for(int i = 0; i < _TRIALSIZE_; ++i){
		compareresult = compare(ina, inb, _LARGEST_, _LARGEST_);
	}
	stop = clock();
	timed = (double)(stop-start)/((double)_TRIALSIZE_);
	printf("ClockCycle: %f. \n", timed);
// */
}