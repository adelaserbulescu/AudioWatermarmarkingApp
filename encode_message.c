#include "Talkthrough.h"

int byte[8];
void encodeMessage(void) {

		for(int k = 7; k >= 0; k--) {
			int bit = (ch & (1 << k)) ? 1 : 0; // Extract each bit from the character
			byte[7 - k] = bit;
		}
}
