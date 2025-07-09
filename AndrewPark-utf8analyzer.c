#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

void is_valid_ascii(char str[]) {
    int i = 0;
    int isValid = 1; 
    while(str[i] != 0) {
        if (str[i] > 127) {isValid = 0; break;}
	i += 1;
    }

    printf("Valid ASCCI: ");
    if (isValid == 1) { printf("true\n"); } else { printf("false\n");}
    
    return; 
}

void uppercase(char str[]) {
    int i = 0; 
    char temp[strlen(str)]; 
    while (str[i] != 0) {
	temp[i] = str[i];
        if (str[i] >= 97 && str[i] <= 122) {
	     temp[i] -= 32;
	}
	i += 1;
    }

    printf("Uppercased ASCII: %s\n", temp);
}

void length_in_bytes(char str[]) {
    printf("Length in bytes: %d\n", (int) (strlen(str)));
}

void length_codepoints(char str[]) {
    int count = 0;
    int i = 0;
    while (str[i] != 0) {
        if ((str[i] & 0b10000000) == 0) { count += 1; i += 1; }
	if ((str[i] & 0b11100000) == 0b11000000) { count += 1; i += 2; }
	if ((str[i] & 0b11110000) == 0b11100000) { count += 1; i += 3; }
	if ((str[i] & 0b11111000) == 0b11110000) { count += 1; i += 4; }
    }

    printf("Number of code points: %d\n", count); 
}

void decimal_codepoints(char str[]) {
    int i = 0;
    printf("Code points as decimal numbers: ");
    while (str[i] != 0) {
        if ((str[i] & 0b10000000) == 0) {
	    printf("%u ", str[i]);
	}
        if ((str[i] & 0b11100000) == 0b11000000) {
	    printf("%u ", ((str[i] & 0b11111) << 6) + (str[i+1] & 0b111111));
	}
        if ((str[i] & 0b11110000) == 0b11100000) {
	    printf("%u ", ((str[i] & 0b1111) << 12) + ((str[i + 1] & 0b111111) << 6) + (str[i + 2] & 0b111111)); 
	}
        if ((str[i] & 0b11111000) == 0b11110000) {
            printf("%u ", ((str[i] & 0b111) << 18) + ((str[i + 1] & 0b111111) << 12) + ((str[i + 2] & 0b111111) << 6) + (str[i+3] & 0b111111)); 
	}
	i+= 1;
    }

    printf("\n");
}

void bytes_per_codepoint(char str[]) {
    int i = 0;
    printf("Bytes per code point: ");
    while (str[i] != 0) {
        if ((str[i] & 0b10000000) == 0) {
            printf("1 ");
        }
        if ((str[i] & 0b11100000) == 0b11000000) {
            printf("2 ");
        }
        if ((str[i] & 0b11110000) == 0b11100000) {
            printf("3 ");
        }
        if ((str[i] & 0b11111000) == 0b11110000) {
            printf("4 ");
        }
        i+= 1;
    }

    printf("\n");
}

void six_substring(char str[]) {
    int i = 0;
    char temp[50]; 
    int tempCount = 0;

    while(str[i] != 0 && tempCount <= 6) {
         if ((str[i] & 0b10000000) == 0) {
            temp[i] = str[i];
	    tempCount += 1;
        }
        if ((str[i] & 0b11100000) == 0b11000000) {
            temp[i] = str[i];
	    temp[i+1] = str[i+1];
	    tempCount += 1;
        }
        if ((str[i] & 0b11110000) == 0b11100000) {
            temp[i] = str[i];
	    temp[i+1] = str[i + 1];
	    temp[i+ 2] = str[i + 2];
	    tempCount += 1;
        }
        if ((str[i] & 0b11111000) == 0b11110000) {
            temp[i] = str[i];
            temp[i+1] = str[i + 1];
            temp[i+ 2] = str[i + 2];
	    temp[i + 3] = str[ i + 3];
	    printf("here: %s", temp);
            tempCount += 1;
        }
        i+= 1;
    }
    temp[tempCount + 1] = '\0'; 

    printf("Substring of the first 6 code points: %s\n", temp); 
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: utf8analyzer \"<UTF-8 encoded string>\"\n");
        return 1;
    }
    

    char *inputStr = argv[1];
    // implement the UTF-8 analyzer here

    is_valid_ascii(inputStr);
    uppercase(inputStr);
    length_in_bytes(inputStr);
    length_codepoints(inputStr);
    decimal_codepoints(inputStr);
    bytes_per_codepoint(inputStr);
    six_substring(inputStr);
}
