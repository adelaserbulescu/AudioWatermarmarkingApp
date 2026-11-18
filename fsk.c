#include "Talkthrough.h"
#include <math.h>
#include <string.h>

int text_index;

void fsk(int f1, int f2)
{
	int freq =0;
	for(int i = 0; i < 16; i++) {
		if(proc_enc[i] == 0) {
			freq = f1;
		}
		else {
			freq = f2;
		}

		for(int j = 0; j < 32; j++) {
			proc_fsk_samples[i * 32 + j] = (int) 160000 * sin(2 * acos(-1) * freq * (i * 32 + j) / 512);
		}
	}

}


int *dec;
int *symbol_real, *symbol_imag;

void fsk_demod(int f1, int f2)
{
	symbol_real = (int*) malloc(sizeof(proc_fsk_samples));
	symbol_imag = (int*) malloc(sizeof(proc_fsk_samples));
	dec = (int*) malloc(sizeof(enc));
	for(int i = 0; i < 16; i++) {
		long long mag_f1 = 0, mag_f2 = 0;
		for(int j = 32 * i; j < (i + 1) * 32; j++)
			symbol_real[j] = proc_fsk_samples[j];
		FFT(1, 5, symbol_real, symbol_imag);
		mag_f1 = sqrt((symbol_real[f1] * symbol_real[f1]) + (symbol_imag[f1] * symbol_imag[f1]));
		mag_f2 = sqrt((symbol_real[f2] * symbol_real[f2]) + (symbol_imag[f2] * symbol_imag[f2]));
		dec[i] = (mag_f2 > mag_f1) ? 1 : 0;
		memset(symbol_real, 0, sizeof(proc_fsk_samples));
		memset(symbol_imag, 0, sizeof(proc_fsk_samples));

	}

}
