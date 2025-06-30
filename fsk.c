#include "Talkthrough.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*long long getNoOfBits(void)
{
	FILE *code = fopen("encoded_message.txt", "r");
    if(!code) {
    	perror("Failed to open file");
		exit(1);
	}

    long long n = 0;
    int ch;
    while((ch = fgetc(code)) != EOF) {
    	if(ch == '1' || ch == '0')
    		n++;
    	else if(ch != ' ') {
    		perror("Unsupported file format");
    		exit(1);
    	}
    }
    fclose(code);
    return n;
}*/

FILE *wave;

void fsk(int f1, int f2, int res)
{
	//long long wave_size = res * getNoOfBits();
	FILE *code = fopen("encoded_message.txt", "r");
	if(!code) {
		perror("Failed to open file");
		exit(1);
	}

	wave = fopen("wave.txt", "w");
	if(!wave) {
		perror("Failed to open file");
		exit(1);
	}
	int bit;
	int freq;
	while((bit = fgetc(code)) != EOF) {
		if(bit != ' ' && bit != '0' && bit != '1') {
			perror("Unsupported file format");
			exit(1);
		}
		if(bit == ' ') {
			continue;
		}

		if(bit == 0) {
			freq = f1;
		}
		else {
			freq = f2;
		}

		for(int i = 0; i < res; i++) {
			int fsk = sin(2 * acos(-1) * freq * i / res);
			fprintf(wave, "%d ", fsk);
		}
	}
	fclose(code);
	fclose(wave);
}
