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

	/*inLeftR[index] = ((iChannel0LeftIn<<8)>>0)>>8;
	inRightR[index] = ((iChannel0RightIn<<8)>>0)>>8;
	inLeftI[index] = inRightI[index] = 0;*/


	//outLeft[index] = ((outLeft[index]<<8)>>0)>>8;
	iChannel0LeftIn = ((iChannel0LeftIn<<8)>>0)>>8;
	procInLeft[index] = iChannel0LeftIn;
	procOutLeft[index] = iChannel0LeftOut = floor((double) ((1 - alpha) * iChannel0LeftIn) + (double) (alpha * fsk_samples[index]));
	//outRight[index] = ((outRight[index]<<8)>>0)>>8;
	iChannel0RightIn = ((iChannel0RightIn<<8)>>0)>>8;
	procInRight[index] = iChannel0RightIn;
	procOutRight[index] = iChannel0RightOut = floor((double) ((1 - alpha) * iChannel0RightIn) + (double) (alpha * fsk_samples[index]));

	/*struct SignalComparator *sample;
	sample->original_left = outLeft[index];
	sample->original_right = outRight[index];
	sample->watermark_left = floor((double)(iChannel0LeftOut - (double) (1 - alpha) * outLeft[index]));
	sample->watermark_right = floor((double)(iChannel0RightOut - (double) (1 - alpha) * outRight[index]));
	sample->sof_value = 0;

	if((*pDMA11_IRQ_STATUS & DMA_RUN) == 0) {
		if(eof_flag) {
			struct SignalComparator init = {0};
			for(int i = 0; i <= 3; i++) {
				memcpy(&tx_buffer[i], init, sizeof(struct SignalComparator));
				tx_buffer[i]->sof_value = ((0xA + i) >> 4) | (0xA + i);
			}
		}
		memcpy(tx_buffer[index + 3], &sample, sizeof(struct SignalComparator));

		if(index == 511) {
		*pDMA11_START_ADDR = (void*)tx_buffer;
		*pDMA11_X_COUNT = 517;
		*pDMA11_X_MODIFY = 1;
		ssync();

		*pDMA11_CONFIG = DMAEN | FLOW_STOP | WDSIZE_8 | DI_EN;
		ssync();
	}
	}*/

	index = (index + 1) % 512;
}
