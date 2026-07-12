#include "environment.c"
#include "tokenizer.c"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h> // archaic, need to remove this reference
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <readline/history.h>
#include <readline/readline.h>

void signalHandler(int sig) {
	printf("interrupt handled: %d", sig);

	exit(sig);
}

int main() {
	char command[1024];
	char *exitCommand = "exit";

	signal(SIGINT, signalHandler);

	initialize(); // clear screen + print title ascii art
	while (true) {
		printf("> ");

		fgets(command, sizeof(command), stdin);
		if (command == NULL) {
			continue;
		}

		command[strcspn(command, "\n")] = '\0'; // replace the newline character with a null terminator so that we can compare against hard-coded keywords (e.g. exit)

		if (strcmp(command, exitCommand) == 0) {
			return EXIT_SUCCESS;
		}

		char *tokens[MAX_TOKENS];
		
		int numTokens = tokenize(command, tokens, MAX_TOKENS);
		if (numTokens < 0) {
			fprintf(stderr, "tokenization failed");
			return -1;
		}

//		for (int i = 0; tokens[i] != NULL; i++) {
//			printf("token %d: %s\n", i, tokens[i]);
//		}
		
		// handling cd separately
		if (tokens[0] != NULL && strcmp(tokens[0], "cd") == 0) {
			if (tokens[1] == NULL) {
				fprintf(stderr, "cd: missing arguments\n");
			} else if (chdir(tokens[1]) != 0)  {
				perror("cd");
			}

			continue;
		}

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
