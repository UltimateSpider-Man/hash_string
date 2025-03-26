#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 128   // Max length for each dictionary word
#define MAX_DICT_SIZE   50000000 // Adjust to the maximum expected dictionary size

// Hash function (unchanged from your original)
unsigned int hash_string(const char* str) {
    const char* v1 = str;
    unsigned int v2 = 0;
    int v3;

    if (*str) {
        do {
            if (isalpha(*v1))
                v3 = tolower(*v1);
            else
                v3 = *v1;
            v2 = v3 + 33 * v2;
        } while (*++v1);
    }
    return v2;
}

// Reverse hash using dictionary lookup
const char* reverse_hash(unsigned int hash_to_find, char** dictionary, int dict_size) {
    for (int i = 0; i < dict_size; ++i) {
        if (dictionary[i] == NULL) {
            continue;
        }
        if (hash_string(dictionary[i]) == hash_to_find) {
            return dictionary[i];
        }
    }
    return NULL; // Not found
}

// Load dictionary from external file into memory
// Returns the number of words loaded, or -1 on error
int load_dictionary(const char* file_path, char** dictionary, int max_dict_size) {
    FILE* file = fopen(file_path, "r");
    if (!file) {
        perror("Failed to open dictionary file");
        return -1;
    }

    int count = 0;
    char buffer[MAX_WORD_LENGTH];

    while (count < max_dict_size && fgets(buffer, sizeof(buffer), file)) {
        // Remove trailing newline characters
        buffer[strcspn(buffer, "\r\n")] = '\0';

        // Allocate memory to store the new word
        dictionary[count] = (char*)malloc(strlen(buffer) + 1);
        if (!dictionary[count]) {
            fprintf(stderr, "Error: malloc failed for dictionary entry.\n");
            fclose(file);
            return -1;
        }
        strcpy(dictionary[count], buffer);

        count++;
    }

    fclose(file);
    return count;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <dictionary_file> <hex_hash>\n", argv[0]);
        printf("Example: %s words.txt 0x891DF25\n", argv[0]);
        return 1;
    }

    // Allocate array of pointers to char* for dictionary
    char** dictionary = (char**)calloc(MAX_DICT_SIZE, sizeof(char*));
    if (!dictionary) {
        fprintf(stderr, "Error: calloc failed for dictionary.\n");
        return 1;
    }

    // Load words from the specified dictionary file
    int dict_size = load_dictionary(argv[1], dictionary, MAX_DICT_SIZE);
    if (dict_size < 0) {
        // Loading dictionary failed
        free(dictionary);
        return 1;
    }

    // Convert the second argument from hex to an unsigned int
    // e.g., 0x891DF25
    unsigned int hash_input = (unsigned int)strtoul(argv[2], NULL, 16);

    // Perform reverse lookup
    const char* result = reverse_hash(hash_input, dictionary, dict_size);
    if (result) {
        printf("String found: %s\n", result);
    }
    else {
        printf("String not found.\n");
    }

    // Clean up
    for (int i = 0; i < dict_size; ++i) {
        free(dictionary[i]);
    }
    free(dictionary);

    return 0;
}
