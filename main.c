#include "environment.c"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> // archaic, need to remove this reference
#include <unistd.h>
#include <sys/wait.h>

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

		int childTerm = fork();

		if (childTerm == 0) {
			// child path - exec() command
			// basically we need to "tokenize" the incoming command by stripping out the spaces and passing that to execvp
//			execvp(
		
		} else {
			// parent path - just go back to the thingy
			int status;
			pid_t child_pid = wait(&status); // freeze until child calls exit()
			//continue;
		}
		
	}

	return 0;

}
