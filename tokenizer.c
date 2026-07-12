#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TOKENS 64

// tracks whether we can keep space-delimiting tokens or if we need to treat a quote-wrapped string as one token
enum cursorState {
	NORMAL,
	INSIDE_DOUBLE_QUOTES,
	INSIDE_SINGLE_QUOTES
};

int tokenize(char *command, char *tokens[], int max_tokens) {
	int count = 0;
	char *p = command;
	enum cursorState cursor = NORMAL; // default state


    while (*p != '\0') {
	// bypass consecutive whitespace characters
        while (isspace((unsigned char)*p)) {
            p++;
        }

        if (*p == '\0') { // EOL
            break;
        }

        if (count >= max_tokens - 1) {
            fprintf(stderr, "too many tokens\n");
            return -1;
        }

        if (*p == '"') {
            // quoted token starts after opening quote
            p++;
            tokens[count++] = p;

            while (*p != '\0' && *p != '"') {
                p++;
            }

            if (*p == '"') {
                *p = '\0';
                p++;
            } else {
                fprintf(stderr, "unterminated quote\n");
                return -1;
            }
        } else {
            // normal token
            tokens[count++] = p;

            while (*p != '\0' && !isspace((unsigned char)*p)) {
                p++;
            }

            if (*p != '\0') {
                *p = '\0';
                p++;
            }
        }

	tokens[count] = NULL; // last token is always NULL for execvp
    }
}
