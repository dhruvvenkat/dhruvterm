#include <stdio.h>

void clearScreen() {
	printf("\033[1;1H\033[2J");
	fflush(stdout);
}
