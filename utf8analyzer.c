#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: utf8analyzer \"<UTF-8 encoded string>\"\n");
        return 1;
    }

    // implement the UTF-8 analyzer here
    int i = 0;
    int validASCII = 1; 
    while (*argv[i] != 0) {
        if ((*argv[i] & 0b10000000) != 0) { validASCII = 0; }
    }

    printf("Valid ASCII: ");
    if (validASCII == 1) { printf(" true\n"); } else { printf(" false\n");}

    char inString[] = "";
    str_len = strlen(inString);

    printf("Length in bytes: %u\n", str_len);


}
