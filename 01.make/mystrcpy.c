#include<string.h>

void mystrcpy(char *dest, char *src) {
	while(*src) {
		*dest++ = *src++;
	}
	*dest = *src;
}

