/*****************************************************************************
**																			**
**	 Name: 	BF537 C Talkthrough I2S										**
**																			**
******************************************************************************
Analog Devices, Inc.  All rights reserved.

File Name:	Main.c

Date Modified:	12/14/04		Rev 1.0
		01/11/05		Rev 1.1 Updated to work on Rev 1.1 of the ADSP-BF537 EZ-Kit

Hardware:	ADSP-BF537 EZ-KIT Board Rev 1.1
			Users of ADSP-BF537 EZ-KIT Board Rev 1.0 must change the SPORT to
			generate internal clock and frame sync

Purpose:	This program sets up the ADSP-BF537 to reset the ADC and DAC.
			The data to/from the DAC/ADC are transfered over SPORT0 in I2S mode.

Program Parameters:


******************************************************************************/
#include "Talkthrough.h"
#include <sysreg.h>
#include <ccblkfn.h>


/*****************************************************************************

Variables

Description:	The variables ChannelxLeftIn and ChannelxRightIn contain
		the data coming from the codec ADC (AD1871).  The (processed)
		playback data are written into the variables
		ChannelxLeftOut and ChannelxRightOut respectively, which
		are then sent back to the DAC (AD1854) in the SPORT0 ISR.

******************************************************************************/
// left input data from AD1871
int iChannel0LeftIn, iChannel1LeftIn;
// right input data from AD1871
int iChannel0RightIn, iChannel1RightIn;
// left ouput data for AD1854
int iChannel0LeftOut, iChannel1LeftOut;
// right ouput data for AD1854
int iChannel0RightOut, iChannel1RightOut;
// SPORT0 DMA transmit buffer
int iTxBuffer1[2];
// SPORT0 DMA receive buffer
int iRxBuffer1[2];

int flag;

int Buffer1LeftR[512];
int Buffer1LeftI[512];
int Buffer2LeftR[512];
int Buffer2LeftI[512];

int Buffer1RightR[512];
int Buffer1RightI[512];
int Buffer2RightR[512];
int Buffer2RightI[512];

int *inLeftR;
int *inLeftI;
int *inRightR;
int *inRightI;
int *procLeftR;
int *procLeftI;
int *procRightR;
int *procRightI;

int outLeft1[512];
int outLeft2[512];
int outRight1[512];
int outRight2[512];
int *outLeft;
int *outRight;
int *procOutLeft;
int *procOutRight;

int *fsk_samples;
int *proc_fsk_samples;
int fsk_samples1[512];
int fsk_samples2[512];
int alpha;

void copy(void)
{
	for(int i = 0; i < 512; i++){
		procOutLeft[i] = procLeftR[i];
		procOutRight[i] = procRightR[i];
	}

	return;
}
//--------------------------------------------------------------------------//
// Function:	main														//
//																			//
// Description:	After calling a few initalization routines, main() just 	//
//				waits in a loop forever.  The code to process the incoming  //
//				data can be placed in the function Process_Data() in the 	//
//				file "Process_Data.c".										//
//--------------------------------------------------------------------------//
void main(void)
{
	Init_Flags();
	Audio_Reset();
	Init_Sport0();
	Init_DMA();
	Init_Interrupts();
	Enable_DMA_Sport0();
	initUART();

	procLeftR = inLeftR = Buffer1LeftR;
	procLeftI = inLeftI = Buffer1LeftI;
	procRightR = inRightR = Buffer1RightR;
	procRightI = inRightI = Buffer1RightI;
	outLeft = outLeft1;
	outRight = outRight1;

	proc_fsk_samples = fsk_samples = fsk_samples1;
	alpha = 0.1;


	flag = 1;

	while(1){
		if(index == 511) {
			if(flag == 1) {
				inLeftR = Buffer1LeftR;
				inLeftI = Buffer1LeftI;
				inRightR = Buffer1RightR;
				inRightI = Buffer1RightI;

				procLeftR = Buffer2LeftR;
				procLeftI = Buffer2LeftI;
				procRightR = Buffer2RightR;
				procRightI = Buffer2RightI;

				outLeft = outLeft1;
				outRight = outRight1;
				procOutLeft = outLeft2;
				procOutRight = outRight2;

				fsk_samples = fsk_samples1;
				proc_fsk_samples = fsk_samples2;
			}

			else {
				inLeftR = Buffer2LeftR;
				inLeftI = Buffer2LeftI;
				inRightR = Buffer2RightR;
				inRightI = Buffer2RightI;

				procLeftR = Buffer1LeftR;
				procLeftI = Buffer1LeftI;
				procRightR = Buffer1RightR;
				procRightI = Buffer1RightI;

				outLeft = outLeft2;
				outRight = outRight2;
				procOutLeft = outLeft1;
				procOutRight = outRight1;

				fsk_samples = fsk_samples2;
				proc_fsk_samples = fsk_samples1;
			}

			output();
			FFT(1, 1, procLeftR, procLeftI);
			FFT(1, 1, procRightR, procRightI);
			FFT(-1, 1, procLeftR, procLeftI);
			FFT(-1, 1, procRightR, procRightI);

			copy();
			flag = -flag;
		}
	}
}
