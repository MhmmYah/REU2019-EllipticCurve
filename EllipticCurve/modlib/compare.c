#include "modlib.h"

int compare(u32bits ina[], u32bits inb[], int alength, int blength){
	
	if(alength > blength)
		return 1;
	else if (alength < blength)
		return -1;
	else{
		--alength;

		while(ina[alength] == inb[alength] && alength>=0)
			--alength;
		if(alength<0)
			return 0;
		else if(inb[alength]>ina[alength])
			return -1; //a is smaller
		else
			return 1; //b is smaller
	}

}