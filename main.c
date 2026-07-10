#include "environment.c"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> // archaic, need to remove this reference

int main() {
	char promptChar = '>';
	char *exitCommand = "exit";

	initialize(); // clear screen + print title ascii art
	while (true) {
		printf("%c ", promptChar);

		char command[10];
		scanf("%s", command);

		if (strcmp(command, exitCommand) == 0) {
			return EXIT_SUCCESS;
		}

		printf("command was: %s\n\n", command);
	}

	return 0;

}
