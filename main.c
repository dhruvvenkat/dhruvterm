#include "environment.c"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h> // archaic, need to remove this reference
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_TOKENS 20

void signalHandler(int sig) {
	printf("interrupt handled: %d", sig);

	exit(sig);
}

int main() {
	char promptChar = '>';
	char *exitCommand = "exit";

	signal(SIGINT, signalHandler);
//	signal(SIGCHLD, signalHandler);

	initialize(); // clear screen + print title ascii art
	while (true) {
		printf("%c ", promptChar);

		char command[1024]; // give 1mb of space for a command (should be enough)
		while (fgets(command, sizeof(command), stdin) == NULL) {
			break;
		}

		command[strcspn(command, "\n")] = '\0'; // replace the newline character with a null terminator so that we can compare against hard-coded keywords (e.g. exit)
//		printf("command: %s", command); 

		if (strcmp(command, exitCommand) == 0) {
			return EXIT_SUCCESS;
		}

		char *token = strtok(command, " \t\n");
		char *tokens[MAX_TOKENS];
		int numTokens = 0;

		while (token != NULL && numTokens < MAX_TOKENS - 1) {
			tokens[numTokens] = token;
			numTokens++;

			token = strtok(NULL, " \t\n");
		}

		tokens[numTokens] = NULL; // set the last value to NULL for execvp
		
//		for (int i = 0; tokens[i] != NULL; i++) {
//			printf("token %d: %s\n", i, tokens[i]);
//		}

		pid_t childCmd = fork();

		if (childCmd < 0) {
			fprintf(stderr, "fork failed\n");
			_exit(1);
		} else if (childCmd == 0) {
			execvp(tokens[0], tokens);
			perror("execvp"); // only return from execvp if it failed
			_exit(127); // note: linux child processes should always use _exit() instead of C exit()
		} else {
			// parent path - just go back to the top
			int status;
			waitpid(childCmd, &status, 0); // freeze until child calls exit()
			//continue;
		}
		
	}

	return 0;

}
