#include "Talkthrough.h"
#include <string.h>

void encodeMessage(void) {

	for(int i = 0; i < 2; i++) {
		for(int k = 7; k >= 0; k--) {
			int bit = ((proc_string[i]) & (1 << k)) ? 1 : 0; // Extract each bit from the character
			proc_enc[i * 7 + 7 - k] = bit;
		}
	}
}


char *decoded_string;

void decodeMessage() {
	decoded_string = (char*)malloc(sizeof(string));
	for(int i = 0; i < 2; i++) {
		int shift = 0;
		for(int k = 7 * (i + 1); k >= 7 * i; k--) {
		    decoded_string[i] +=  dec[i] << shift;
		    shift++;
		}
	}
}


char *recovered_text;

void recoverText() {
	recovered_text = (char*) malloc(sizeof((char*)text));
	if(strlen(recovered_text) == strlen(text))
		return;
	else {
		fsk_demod(50, 2000);
		decodeMessage();
		strncpy(recovered_text + strlen(recovered_text), decoded_string, 2);
	}
}
