#include "Talkthrough.h"
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

/*void reload_byte(void)
{
	for(int i = 0; i < 8; i++)
		byte[i] = 0;
	encodeMessage();
}

void reload_fsk(void)
{
	for(int i = 0; i < 32; i++)
		fsk_samples[i] = 0;
	fsk(50, 2000);
}*/

void Process_Data(void)
{

	inLeftR[index] = ((iChannel0LeftIn<<8)>>0)>>8;
	inRightR[index] = ((iChannel0RightIn<<8)>>0)>>8;
	inLeftI[index] = inRightI[index] = 0;


	iChannel0LeftOut = ((outLeft[index]<<8)>>0)>>8;
	//iChannel0LeftOut = (1 - alpha) * iChannel0LeftOut + alpha * fsk_samples[index % 32];
	iChannel0RightOut = ((outRight[index]<<8)>>0)>>8;
	//iChannel0RightOut = (1 - alpha) * iChannel0RightOut + alpha * fsk_samples[index % 32];

	/*if(index % 32 == 31) {
		bit = byte[(index + 1) % 8];
		reload_fsk();
	}

	if(index % 256 == 255) {
		ch = text[(index + 1) % len];
		reload_byte();
	}*/


	index = (index + 1) % 512;
}
