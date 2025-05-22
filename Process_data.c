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

int *inBuffer1LeftR;
int *inBuffer1LeftI;
int *inBuffer2LeftR;
int *inBuffer2LeftI;

int *inBuffer1RightR;
int *inBuffer1RightI;
int *inBuffer2RightR;
int *inBuffer2RightI;

int *outBuffer1LeftR;
int *outBuffer1LeftI;
int *outBuffer2LeftR;
int *outBuffer2LeftI;

int *outBuffer1RightR;
int *outBuffer1RightI;
int *outBuffer2RightR;
int *outBuffer2RightI;

int index;

void swap(int *a, int *b) {
  int *temp;
  temp = a;
  a = b;
  b = temp;
}

void switchBuffs() {
	swap(inBuffer1LeftR, inBuffer2LeftR);
	swap(inBuffer1LeftI, inBuffer2LeftI);
	swap(inBuffer1RightR, inBuffer2RightR);
	swap(inBuffer1RightI, inBuffer2RightI);
	swap(outBuffer1LeftR, outBuffer2LeftR);
	swap(outBuffer1LeftI, outBuffer2LeftI);
	swap(outBuffer1RightR, outBuffer2RightR);
	swap(outBuffer1RightI, outBuffer2RightI);
}

void Process_Data(void)
{

	inBuffer1LeftR[index] = ((iChannel0LeftIn<<8)>>0)>>8;
	inBuffer1RightR[index] = ((iChannel0RightIn<<8)>>0)>>8;
	index = (index + 1) % 512;

	if(index == 511) {
		switchBuffs();
		fft(1, 9, inBuffer2LeftR, inBuffer2LeftI);
		fft(1, 9, inBuffer2RightR, inBuffer2RightI);
		outBuffer2LeftR = inBuffer2LeftR;
		outBuffer2LeftI = inBuffer2LeftI;
		outBuffer2RightR = inBuffer2RightR;
		outBuffer2RightI = inBuffer2RightI;
		fft(0, 9, outBuffer2LeftR, outBuffer2LeftI);
		fft(0, 9, outBuffer2RightR, outBuffer2RightI);
	}

	iChannel0LeftOut = ((outBuffer1LeftR<<8)>>0)>>8;
	iChannel0RightOut = ((outBuffer1RightR<<8)>>0)>>8;
}
