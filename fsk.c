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
int b1, b2;

void fsk_demod(int f1, int f2)
{
	int b1 = f1 / 16, b2 = f2 / 16;
	if (b1 < 0 || b1 > 31 || b2 < 0 || b2 > 31) return;
    symbol_real = (int*) calloc(32, sizeof(int));
    symbol_imag = (int*) calloc(32, sizeof(int));
    dec         = (int*) calloc(16, sizeof(int));
    if (!symbol_real || !symbol_imag || !dec) {
        free(symbol_real); free(symbol_imag); free(dec);
        symbol_real = symbol_imag = dec = NULL;
        return;
    }
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 32; j++) {
            symbol_real[j] = proc_fsk_samples[i * 32 + j];
            symbol_imag[j] = 0;
        }
        FFT(1, 5, symbol_real, symbol_imag);        // 32-point
        long long r1 = symbol_real[b1], i1 = symbol_imag[b1];
        long long r2 = symbol_real[b2], i2 = symbol_imag[b2];
        long long m1 = r1*r1 + i1*i1;
        long long m2 = r2*r2 + i2*i2;
        dec[i] = (m2 > m1) ? 1 : 0;
    }
    free(symbol_real);
    free(symbol_imag);
}
