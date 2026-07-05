#include "Talkthrough.h"
#include <math.h>
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



void Process_Data(void)
{

	iChannel0LeftIn = ((iChannel0LeftIn<<8)>>0)>>8;
	procInLeft[index] = iChannel0LeftIn;
	procOutLeft[index] = iChannel0LeftOut = floor((double) ((1 - alpha) * iChannel0LeftIn) + (double) (alpha * fsk_samples[index]));
	iChannel0RightIn = ((iChannel0RightIn<<8)>>0)>>8;
	procInRight[index] = iChannel0RightIn;
	procOutRight[index] = iChannel0RightOut = floor((double) ((1 - alpha) * iChannel0RightIn) + (double) (alpha * fsk_samples[index]));
	index = (index + 1) % 512;
}
