#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

char Plain[26];
char Cipher[26];

void shuffle(char *array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void PutCharInVec() {
    printf("Plain:\n");
    for (int i = 0; i < 26; i++) {
        Plain[i] = 'a' + i;
        printf("%c\t", Plain[i]);
    }
    printf("\n");

    // Initialize Cipher as uppercase alphabet
    for (int i = 0; i < 26; i++) {
        Cipher[i] = 'A' + i;
    }

    // Generate perfect random permutation of Cipher
    srand((unsigned int)time(NULL));
    shuffle(Cipher, 26);

    printf("Cipher:\n");
    for (int i = 0; i < 26; i++) {
        printf("%c\t", Cipher[i]);
    }
    printf("\n");
}

char Monoalphabetic(char c) {
    if (islower(c)) { // Encryption
        for (int i = 0; i < 26; i++) {
            if (Plain[i] == c)
                return Cipher[i];
        }
    } else if (isupper(c)) { // Decryption
        for (int i = 0; i < 26; i++) {
            if (Cipher[i] == c)
                return Plain[i];
        }
    }
    return c; // Return unchanged if not alphabetic
}

int main() {
    char input[256];
    char output[256];
    int choice = 0;

    PutCharInVec();

    while (choice != 2) {
        printf("\nPress 1: Encryption/Decryption; Press 2: Quit: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            return 1;
        }
        getchar(); // Consume newline

        if (choice == 1) {
            printf("\nNOTE: Use LOWERCASE for encryption and UPPERCASE for decryption\n");
            printf("Enter text (alphabets only): ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0';

            // Input validation
            for (int i = 0; i < (int)strlen(input); i++) {
                if (!isalpha(input[i])) {
                    printf("Error: Only alphabets allowed!\n");
                    return 1;
                }
            }

            // Perform encryption/decryption
            for (int i = 0; i < (int)strlen(input); i++) {
                output[i] = Monoalphabetic(input[i]);
            }
            output[strlen(input)] = '\0';

            printf("Output: %s\n", output);
        } else if (choice != 2) {
            printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}
