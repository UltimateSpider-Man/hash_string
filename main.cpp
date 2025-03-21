#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "main.h"


// Your provided hash function
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
const char* reverse_hash(unsigned int hash_to_find, const char* dictionary[], int dict_size) {
    for (int i = 0; i < dict_size; ++i) {
        if (hash_string(dictionary[i]) == hash_to_find)
            return dictionary[i];
    }
    return NULL; // Not found
}

int main(int argc, char* argv[]) {
    // Sample dictionary; expand as needed


    int dict_size = sizeof(dictionary) / sizeof(dictionary[0]);
    unsigned int hash_input;

    if (argc != 2) {
        printf("Usage: %s <hash_code>\n", argv[0]);
        printf("Example: %s 143691877\n", argv[0]);
        return 1;
    }

    hash_input = (unsigned int)strtoul(argv[1], NULL, 10);

    const char* result = reverse_hash(hash_input, dictionary, dict_size);
    if (result)
        printf("String found: %s\n", result);
    else
        printf("String not found.\n");

    return 0;
}



