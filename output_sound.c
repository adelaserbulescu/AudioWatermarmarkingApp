#include "Talkthrough.h"
#include <math.h>

volatile uint8_t temp;

void output(void) {
	int f = 0;
	if(temp == '0') {
		f = 50;
	}
	else if(temp == '1') {
		f = 1000;
	}

	for(int i = 0; i < 512; i++) {
		proc_fsk_samples[i] = (int) 1600000 * sin(2 * acos(-1) * f * i / 512);
	}
}
