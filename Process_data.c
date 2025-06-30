#include "Talkthrough.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
//--------------------------------------------------------------------------//
// Function:	Process_Data()												//
//																			//
// Description: This function is called from inside the SPORT0 ISR every 	//
//				time a complete audio frame has been received. The new 		//
//				input samples can be found in the variables iChannel0LeftIn,//
//				iChannel0RightIn, iChannel1LeftIn and iChannel1RightIn 		//
//				respectively. The processed	data should be stored in 		//
//				iChannel0LeftOut, iChannel0RightOut, iChannel1LeftOut,		//
//				iChannel1RightOut, iChannel2LeftOut and	iChannel2RightOut	//
//				respectively.												//
//--------------------------------------------------------------------------//


int index = 0;
int current_wave_sample;
int val;

void Process_Data(void)
{

	inLeftR[index] = ((iChannel0LeftIn<<8)>>0)>>8;
	inRightR[index] = ((iChannel0RightIn<<8)>>0)>>8;
	inLeftI[index] = inRightI[index] = 0;

	/*current_wave_sample = 0;
	while((val = fgetc(wave)) != ' ' || (val = fgetc(wave)) != EOF) {
		if(!isdigit(val)) {
			//perror("Unsupported file format");
			exit(1);
		}
		current_wave_sample = current_wave_sample * 10 + val - '0';

	}*/

	iChannel0LeftOut = ((outLeft[index]<<8)>>0)>>8;
	//iChannel0LeftOut = (1 - alpha) * iChannel0LeftOut + alpha * current_wave_sample;
	iChannel0RightOut = ((outRight[index]<<8)>>0)>>8;
	//iChannel0RightOut = (1 - alpha) * iChannel0RightOut + alpha * current_wave_sample;

	index = (index + 1) % 512;
	/*if(feof(wave)) {
		rewind(wave);
    }*/
}
