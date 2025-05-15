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

int inputBuff1R[512][2];
int inputBuff2R[512][2];
int inputBuff1I[512][2];
int inputBuff2I[512][2];

int index;
int *inPointerR;
int *inPointerI;

int outputBuff1R[512][2];
int outputBuff2R[512][2];
int outputBuff1I[512][2];
int outputBuff2I[512][2];

int *outPointerR;
int *outPointerI;

void switchBuffs(int *p, int a[512][], int b[512][]) {
  if(*p == a[511][1]) {
	  *p = b;
  }
  else *p = a;
}

void Process_Data(void)
{

	&inPointerR = ((iChannel0LeftIn<<8)>>0)>>8;
	inPointerR++;
	&inPointerR = ((iChannel0RightIn<<8)>>0)>>8;
	index = (index + 1) % 512;

	//FFT(1,9, *);

	if(index == 511) {
		switchBuffs(inPointerR, inputBuff1R, inputBuff2R);
		switchBuffs(inPointerI, inputBuff1I, inputBuff2I);
		switchBuffs(outPointerR, outputBuff1R, outputBuff2R);
		switchBuffs(outPointerI, outputBuff1I, outputBuff2I);
	}

	iChannel0LeftOut = ((iChannel0LeftIn<<8)>>0)>>8;
	iChannel0RightOut = ((iChannel0RightIn<<8)>>0)>>8;
}
