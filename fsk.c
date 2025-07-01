#include "Talkthrough.h"
#include <math.h>

int fsk_samples[32];
int bit;

void fsk(int f1, int f2)
{
	int freq;

		if(bit == 0) {
			freq = f1;
		}
		else {
			freq = f2;
		}

		for(int i = 0; i < 32; i++) {
			fsk_samples[i] = sin(2 * acos(-1) * freq * i / 512);
		}
}
