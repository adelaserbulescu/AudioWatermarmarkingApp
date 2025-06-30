#include "Talkthrough.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void codeMessage() {
	FILE *input = fopen("message.txt", "r");
	if (!input) {
	        //perror("Failed to open input file");
	        exit(1);
	}
	FILE *output = fopen("encoded_message.txt", "w");
	if (!output) {
	        //perror("Failed to open output file");
	        exit(1);
	}
	int ch;
	while ((ch = fgetc(input)) != EOF) {
		if(!(ch >=32 && ch <= 127)) {
	            continue;
		}
		for(int k = 7; k >= 0; k--) {
			int bit = (ch & (1 << k)) ? '1' : '0'; // Extract each bit from the character
			fputc(bit, output);
		}

		fputc(' ', output); // Print a space after each byte

	}
	fclose(input);
	fclose(output);
}
