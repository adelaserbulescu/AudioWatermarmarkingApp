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

/*extern int Buffer1LeftR[512];
extern int Buffer1LeftI[512];
extern int Buffer2LeftR[512];
extern int Buffer2LeftI[512];

extern int Buffer1RightR[512];
extern int Buffer1RightI[512];
extern int Buffer2RightR[512];
extern int Buffer2RightI[512];

extern int *inLeftR;
extern int *inLeftI;
extern int *inRightR;
extern int *inRightI;
extern int *procLeftR;
extern int *procLeftI;
extern int *procRightR;
extern int *procRightI;

extern int outLeft1[512];
extern int outLeft2[512];
extern int outRight1[512];
extern int outRight2[512];
extern int *outLeft;
extern int *outRight;
extern int *procOutLeft;
extern int *procOutRight;*/

extern  int *outLeft;
extern  int *outRight;
extern  int *inLeft;
extern  int *inRight;

extern int index;
extern int flag;

extern double alpha;
extern char text[];
extern int len;

extern char *string;
extern char *proc_string;
extern char string1[2];
extern char string2[2];

extern int *enc;
extern int *proc_enc;
extern int enc1[16];
extern int enc2[16];

extern int *fsk_samples;
extern int *proc_fsk_samples;
extern int fsk_samples1[512];
extern int fsk_samples2[512];

extern int text_index;

extern int eof_flag;
extern volatile char rx_buffer[8];
extern volatile int rx_index;
extern volatile int uart_isr_count;
extern  int *procOutLeft;
extern  int *procOutRight;
extern  int *procInLeft;
extern  int *procInRight;
extern  int outLeft1[512];
extern  int outRight1[512];
extern  int inLeft1[512];
extern  int inRight1[512];
extern  int outLeft2[512];
extern  int outRight2[512];
extern  int inLeft2[512];
extern  int inRight2[512];


extern struct SignalComparator {
	int sof_value;
	int original_left;
	int original_right;
	int watermark;

}tx_buffer[516];


extern volatile int frame_state;

extern volatile int struct_size;
extern volatile int offset_left;
extern volatile int offset_right;
extern volatile int offset_watermark;
extern volatile int offset_sof;
extern volatile int fill_tx_called;



//--------------------------------------------------------------------------//
// Prototypes																//
//--------------------------------------------------------------------------//
// in file Initialize.c
void Init_Flags(void);
void Audio_Reset(void);
void Init_Sport0(void);
void Init_DMA(void);
void initUART(void);
void Init_Interrupts(void);
void Enable_DMA_Sport0(void);
void initTIM0(void);
void delayTIM0(void);

// in file Process_data.c
void Process_Data(void);

// in file ISRs.c
EX_INTERRUPT_HANDLER(Sport0_RX_ISR);
EX_INTERRUPT_HANDLER(UART1_RX_ISR);
EX_INTERRUPT_HANDLER(TIM0_ISR);

//FFT
void FFT(short int dir, long m, int *x, int *y);

//Initialize Buffer Pointers
void initBuffers(void);

void getText(void);

void procFirstTwoChars(void);

void encodeMessage(void);

void fsk(int f1, int f2);

void fillTX(void);

void readRX(char n);

#endif //__Talkthrough_DEFINED
