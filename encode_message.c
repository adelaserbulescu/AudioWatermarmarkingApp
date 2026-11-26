#include "Talkthrough.h"
#include <string.h>

void encodeMessage(void) {

	for(int i = 0; i < 2; i++) {
		for(int k = 7; k >= 0; k--) {
			int bit = ((proc_string[i]) & (1 << k)) ? 1 : 0; // Extract each bit from the character
			proc_enc[i * 8 + 7 - k] = bit;
		}
	}
}


char *decoded_string;

void decodeMessage() {
	if (!dec) return;                                  // fsk_demod failed
	decoded_string = (char*) calloc(3, 1);
	if (!decoded_string) return;
	for (int i = 0; i < 2; i++) {
		char c = 0;
	    for (int b = 0; b < 8; b++)
	    	c = (c << 1) | (dec[i * 8 + b] & 1);// MSB first, like encodeMessage

	    decoded_string[i] = c;
	}
}

char* recovered_text;
static int rec_len = 0;
void recoverText(void)
{
    if (recovered_text == NULL) {                      // allocate once
        recovered_text = (char*) calloc(len + 3, 1);
        if (!recovered_text) return;
    }
    if (rec_len >= len) return;
    fsk_demod(64, 192);
    decodeMessage();
    if (decoded_string && dec) {
        memcpy(recovered_text + rec_len, decoded_string, 2);
        rec_len += 2;
    }
    free(decoded_string); decoded_string = NULL;
    free(dec);            dec = NULL;
}
