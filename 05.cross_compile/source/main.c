#include <stdio.h>

extern void sub1();
extern void sub2();

int main() {
	printf("Hello main()\n");
	sub1();
	sub2();
	return 0;
}
