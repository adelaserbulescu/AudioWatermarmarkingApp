#ifndef  __Talkthrough_DEFINED
	#define __Talkthrough_DEFINED

//--------------------------------------------------------------------------//
// Header files																//
//--------------------------------------------------------------------------//
#include <sys\exception.h>
#include <cdefBF537.h>

//--------------------------------------------------------------------------//
// Symbolic constants														//
//--------------------------------------------------------------------------//
// names for registers in AD1854/AD187 converters
#define INTERNAL_ADC_L0			0
#define INTERNAL_ADC_R0			1
#define INTERNAL_DAC_L0			0
#define INTERNAL_DAC_R0			1


#define delay 0xf00

// SPORT0 word length
#define SLEN_24	0x0017

// DMA flow mode
#define FLOW_1					0x1000


//--------------------------------------------------------------------------//
// Global variables															//
//--------------------------------------------------------------------------//
extern int iChannel0LeftIn;
extern int iChannel0RightIn;
extern int iChannel0LeftOut;
extern int iChannel0RightOut;

extern int iRxBuffer1[];
extern int iTxBuffer1[];

extern int *inBuffer1LeftR;
extern int *inBuffer1LeftI;
extern int *inBuffer2LeftR;
extern int *inBuffer2LeftI;

extern int *inBuffer1RightR;
extern int *inBuffer1RightI;
extern int *inBuffer2RightR;
extern int *inBuffer2RightI;

extern int *outBuffer1LeftR;
extern int *outBuffer1LeftI;
extern int *outBuffer2LeftR;
extern int *outBuffer2LeftI;

extern int *outBuffer1RightR;
extern int *outBuffer1RightI;
extern int *outBuffer2RightR;
extern int *outBuffer2RightI;

extern int *inLeftR;
extern int *inLeftI;
extern int *inRightR;
extern int *inRightI;
extern int *outLeftR;
extern int *outLeftI;
extern int *outRightR;
extern int *outRightI;

extern int index;
extern int flag;


//--------------------------------------------------------------------------//
// Prototypes																//
//--------------------------------------------------------------------------//
// in file Initialize.c
void Init_Flags(void);
void Audio_Reset(void);
void Init_Sport0(void);
void Init_DMA(void);
void Init_Interrupts(void);
void Enable_DMA_Sport0(void);

// in file Process_data.c
void Process_Data(void);

// in file ISRs.c
EX_INTERRUPT_HANDLER(Sport0_RX_ISR);

//FFT
void FFT(short int dir, long m, int *x, int *y);

//Initialize Buffer Pointers
void initBuffers(void);

#endif //__Talkthrough_DEFINED
