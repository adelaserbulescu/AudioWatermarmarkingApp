#include "Talkthrough.h"
#include <math.h>
#include <string.h>

int text_index;

void fsk(int f1, int f2)
{
	int freq;
	for(int i = 0; i < 16; i++) {
		if(proc_enc[i] == 0) {
			freq = f1;
		}
		else {
			freq = f2;
		}

		for(int j = 0; j < 32; j++) {
			proc_fsk_samples[i * 32 + j] = (int) 1600000 * sin(2 * acos(-1) * freq * (i * 32 + j) / 512);
		}
	}

}
