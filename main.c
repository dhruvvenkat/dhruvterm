#include "environment.c"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> // archaic, need to remove this reference
#include <unistd.h>
#include <sys/wait.h>

#define MAX_TOKENS 20

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

		char *token = strtok(command, " ");
		char *tokens[MAX_TOKENS];
		int numTokens = 0;

		while (token != NULL && numTokens < MAX_TOKENS - 1) {
			tokens[numTokens] = token;
			numTokens++;

			token = strtok(NULL, " \t\n");
		}

		tokens[numTokens] = NULL; // set the last value to NULL for execvp

		pid_t childCmd = fork();

		if (childCmd < 0) {
			fprintf(stderr, "fork failed\n");
			exit(1);
		} else if (childCmd == 0) {
			// child path - exec() command
			// basically we need to "tokenize" the incoming command by stripping out the spaces and passing that to execvp
			
			
			execvp(tokens[0], tokens);
			perror("execvp");

			exit(1);
		} else {
			// parent path - just go back to the top
			pid_t child_pid = wait(NULL); // freeze until child calls exit()
			//continue;
		}
		
	}

	return 0;

}
