#include <stdio.h>
#include <stdlib.h> // For malloc, free
#include <string.h> // For strlen, strcpy, etc.
#include <stdbool.h> // For bool type

// --- Helper Functions (From PS1 or to be implemented) ---

/**
 * @brief Determines the byte length of a UTF-8 character based on its first byte.
 * @param byte The first byte of a potential UTF-8 character.
 * @return The number of bytes this UTF-8 character occupies (1-4), or 0 if invalid.
 */
int get_utf8_char_len(unsigned char byte) {
    if ((byte & 0x80) == 0x00) { // 0xxxxxxx
        return 1;
    } else if ((byte & 0xE0) == 0xC0) { // 110xxxxx
        return 2;
    } else if ((byte & 0xF0) == 0xE0) { // 1110xxxx
        return 3;
    } else if ((byte & 0xF8) == 0xF0) { // 11110xxx
        return 4;
    }
    return 0; // Invalid start byte
}

/**
 * @brief Extracts the Unicode codepoint value from a UTF-8 byte sequence.
 * @param utf8_bytes Pointer to the start of the UTF-8 character.
 * @param len The byte length of the UTF-8 character (from get_utf8_char_len).
 * @return The unsigned int Unicode codepoint value.
 */
unsigned int get_codepoint(const unsigned char *utf8_bytes, int len) {
    unsigned int codepoint = 0;
    if (len == 1) {
        codepoint = utf8_bytes[0];
    } else if (len == 2) {
        codepoint = ((utf8_bytes[0] & 0x1F) << 6) | (utf8_bytes[1] & 0x3F);
    } else if (len == 3) {
        codepoint = ((utf8_bytes[0] & 0x0F) << 12) |
                    ((utf8_bytes[1] & 0x3F) << 6) |
                    (utf8_bytes[2] & 0x3F);
    } else if (len == 4) {
        codepoint = ((utf8_bytes[0] & 0x07) << 18) |
                    ((utf8_bytes[1] & 0x3F) << 12) |
                    ((utf8_bytes[2] & 0x3F) << 6) |
                    (utf8_bytes[3] & 0x3F);
    }
    return codepoint;
}

/**
 * @brief Converts a Unicode codepoint value back to its UTF-8 byte sequence.
 * @param codepoint The unsigned int Unicode codepoint.
 * @param buffer A buffer (at least 5 bytes) to store the UTF-8 sequence (plus null terminator).
 * @return The number of bytes written to the buffer, or 0 if invalid codepoint.
 */
int codepoint_to_utf8(unsigned int codepoint, unsigned char *buffer) {
    if (codepoint <= 0x7F) { // 1-byte (ASCII)
        buffer[0] = (unsigned char)codepoint;
        return 1;
    } else if (codepoint <= 0x7FF) { // 2-bytes
        buffer[0] = 0xC0 | (unsigned char)(codepoint >> 6);
        buffer[1] = 0x80 | (unsigned char)(codepoint & 0x3F);
        return 2;
    } else if (codepoint <= 0xFFFF) { // 3-bytes
        buffer[0] = 0xE0 | (unsigned char)(codepoint >> 12);
        buffer[1] = 0x80 | (unsigned char)((codepoint >> 6) & 0x3F);
        buffer[2] = 0x80 | (unsigned char)(codepoint & 0x3F);
        return 3;
    } else if (codepoint <= 0x10FFFF) { // 4-bytes
        buffer[0] = 0xF0 | (unsigned char)(codepoint >> 18);
        buffer[1] = 0x80 | (unsigned char)((codepoint >> 12) & 0x3F);
        buffer[2] = 0x80 | (unsigned char)((codepoint >> 6) & 0x3F);
        buffer[3] = 0x80 | (unsigned char)(codepoint & 0x3F);
        return 4;
    }
    return 0; // Invalid codepoint
}

// --- Specific Lab Task Functions ---

/**
 * @brief Checks if a UTF-8 string is valid ASCII.
 * @param input_str The input string.
 * @return True if all bytes are <= 127, false otherwise.
 */
bool is_valid_ascii(const char *input_str) {
    const unsigned char *ptr = (const unsigned char *)input_str;
    while (*ptr != '\0') {
        if (*ptr > 127) {
            return false;
        }
        ptr++;
    }
    return true;
}

/**
 * @brief Converts ASCII lowercase characters to uppercase.
 * Modifies a copy of the input string.
 * @param input_str The original input string.
 * @return A dynamically allocated string with ASCII chars uppercased.
 * Caller is responsible for freeing this memory.
 */
char* to_uppercase_ascii_copy(const char *input_str) {
    // Allocate memory for the new string (plus null terminator)
    char *upper_str = (char *)malloc(strlen(input_str) + 1);
    if (upper_str == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    strcpy(upper_str, input_str); // Copy original string

    for (int i = 0; upper_str[i] != '\0'; i++) {
        // Only modify if it's an ASCII lowercase letter
        if (upper_str[i] >= 'a' && upper_str[i] <= 'z') {
            upper_str[i] = upper_str[i] - 32; // Convert to uppercase (ASCII magic)
        }
    }
    return upper_str;
}

/**
 * @brief Counts the number of UTF-8 codepoints in a string.
 * @param input_str The input string.
 * @return The number of UTF-8 codepoints.
 */
int count_utf8_codepoints(const char *input_str) {
    int count = 0;
    const unsigned char *ptr = (const unsigned char *)input_str;
    while (*ptr != '\0') {
        int len = get_utf8_char_len(*ptr);
        if (len == 0) {
            // Handle invalid UTF-8 (for this lab, might skip or assume valid)
            // For robustness, you'd advance by 1 byte and report error
            ptr++;
        } else {
            count++;
            ptr += len;
        }
    }
    return count;
}

/**
 * @brief Prints the decimal representation and byte length of each codepoint.
 * @param input_str The input string.
 */
void print_codepoint_details(const char *input_str) {
    const unsigned char *ptr = (const unsigned char *)input_str;
    printf("Decimal representations of each codepoint: ");
    while (*ptr != '\0') {
        int len = get_utf8_char_len(*ptr);
        if (len == 0) {
            // Invalid byte, skip or report
            ptr++;
            continue;
        }
        unsigned int codepoint = get_codepoint(ptr, len);
        printf("%u ", codepoint);
        ptr += len;
    }
    printf("\n");

    ptr = (const unsigned char *)input_str; // Reset pointer
    printf("Bytes used to represent each codepoint: ");
    while (*ptr != '\0') {
        int len = get_utf8_char_len(*ptr);
        if (len == 0) {
            ptr++;
            continue;
        }
        printf("%d ", len);
        ptr += len;
    }
    printf("\n");
}

/**
 * @brief Extracts the substring containing only the first N codepoints.
 * @param input_str The input string.
 * @param num_codepoints The number of codepoints to extract.
 * @return A dynamically allocated string containing the substring.
 * Caller is responsible for freeing this memory.
 */
char* get_first_n_codepoints_substring(const char *input_str, int num_codepoints) {
    const unsigned char *ptr = (const unsigned char *)input_str;
    int current_codepoint_count = 0;
    size_t bytes_to_copy = 0;

    while (*ptr != '\0' && current_codepoint_count < num_codepoints) {
        int len = get_utf8_char_len(*ptr);
        if (len == 0) {
            // Invalid byte, skip
            ptr++;
            bytes_to_copy++; // Count even invalid bytes to avoid infinite loop on malformed
            continue;
        }
        bytes_to_copy += len;
        ptr += len;
        current_codepoint_count++;
    }

    char *substring = (char *)malloc(bytes_to_copy + 1);
    if (substring == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }
    strncpy(substring, input_str, bytes_to_copy);
    substring[bytes_to_copy] = '\0'; // Null-terminate the new string

    return substring;
}

/**
 * @brief Checks if a codepoint is an animal emoji based on defined ranges.
 * @param codepoint The Unicode codepoint value.
 * @return True if it's an animal emoji, false otherwise.
 */
bool is_animal_emoji(unsigned int codepoint) {
    // 🐀 (U+1F400) to 🐿️ (U+1F43F)
    // 🦀 (U+1F980) to 🦮 (U+1F9AE)
    return (codepoint >= 0x1F400 && codepoint <= 0x1F43F) ||
           (codepoint >= 0x1F980 && codepoint <= 0x1F9AE);
}

/**
 * @brief Finds and prints all animal emojis in the input string.
 * @param input_str The input string.
 */
void print_animal_emojis(const char *input_str) {
    const unsigned char *ptr = (const unsigned char *)input_str;
    printf("Animal emojis in the input string: ");
    bool found_any = false;

    while (*ptr != '\0') {
        int len = get_utf8_char_len(*ptr);
        if (len == 0) {
            ptr++;
            continue;
        }
        unsigned int codepoint = get_codepoint(ptr, len);

        if (is_animal_emoji(codepoint)) {
            // Print the UTF-8 bytes directly
            for (int i = 0; i < len; i++) {
                printf("%c", ptr[i]);
            }
            printf(" "); // Space between emojis
            found_any = true;
        }
        ptr += len;
    }
    if (!found_any) {
        printf("None");
    }
    printf("\n");
}

/**
 * @brief Finds the codepoint at a specific 0-indexed position, increments it, and prints the new character.
 * @param input_str The input string.
 * @param index The 0-indexed position of the codepoint to modify.
 */
void print_incremented_codepoint_at_index(const char *input_str, int index) {
    const unsigned char *ptr = (const unsigned char *)input_str;
    int current_codepoint_index = 0;
    unsigned int target_codepoint = 0;
    int target_codepoint_len = 0;

    // Find the codepoint at the target index
    while (*ptr != '\0') {
        int len = get_utf8_char_len(*ptr);
        if (len == 0) {
            ptr++;
            continue;
        }

        if (current_codepoint_index == index) {
            target_codepoint = get_codepoint(ptr, len);
            target_codepoint_len = len;
            break; // Found it
        }
        ptr += len;
        current_codepoint_index++;
    }

    printf("Codepoint with value one higher at index %d: ", index);
    if (target_codepoint_len == 0) {
        printf("Codepoint at index %d not found or invalid.\n", index);
        return;
    }

    target_codepoint++; // Increment the codepoint value

    unsigned char buffer[5]; // Max 4 bytes + null terminator
    int new_len = codepoint_to_utf8(target_codepoint, buffer);
    if (new_len > 0) {
        buffer[new_len] = '\0';
        printf("%s\n", buffer);
    } else {
        printf("Could not convert incremented codepoint to UTF-8.\n");
    }
}


// --- Main Program ---
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s \"<UTF-8 string>\"\n", argv[0]);
        return 1;
    }

    const char *input_string = argv[1];

    printf("Input string: %s\n", input_string);

    // 1. Whether the string is valid ASCII
    printf("Is valid ASCII: %s\n", is_valid_ascii(input_string) ? "Yes" : "No");

    // 2. The same input string, but any ASCII lowercase characters a-z are changed to their uppercase versions.
    char *upper_case_str = to_uppercase_ascii_copy(input_string);
    if (upper_case_str) {
        printf("Uppercase ASCII string: %s\n", upper_case_str);
        free(upper_case_str); // Free dynamically allocated memory
    }

    printf("Valid ASCII: ");
    if (validASCII == 1) { printf(" true\n"); } else { printf(" false\n");}

    // 3. Length of the whole string in bytes.
    printf("Length of the whole string in bytes: %zu\n", strlen(input_string));

    // 4. The number of UTF-8 codepoints represented by the string.
    printf("Number of UTF-8 codepoints: %d\n", count_utf8_codepoints(input_string));

    // 5. Decimal representations of each codepoint.
    // 6. How many bytes are used to represent each codepoint.
    print_codepoint_details(input_string);

    // 7. Substring of the input string containing only the first 6 codepoints.
    char *first_6_codepoints_sub = get_first_n_codepoints_substring(input_string, 6);
    if (first_6_codepoints_sub) {
        printf("First 6 codepoints substring: %s\n", first_6_codepoints_sub);
        free(first_6_codepoints_sub); // Free dynamically allocated memory
    }

    // 8. All animal emojis in the input string.
    print_animal_emojis(input_string);

    // 9. The code point with value one higher at index 3.
    print_incremented_codepoint_at_index(input_string, 3); // Index 3 for the 4th codepoint

    return 0;

    // implement the UTF-8 analyzer here
}
