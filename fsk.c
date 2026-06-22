#include "Talkthrough.h"
#include <math.h>
#include <string.h>

int text_index;

void fsk(uint8_t state, int f1, int f2)
{
	int freq;
	if(state)
		freq = f1;
	else
		freq = f2;
	for(int i = 0; i < 512; i++) {
		proc_fsk_samples[i] = (int) 1600000 * sin(2 * acos(-1) * freq * i / 512);
	}

}
