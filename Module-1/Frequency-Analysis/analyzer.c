#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <float.h> // For DBL_MAX

// Maximum size for the cipher text buffer (adjust as needed)
#define MAX_TEXT_SIZE 10000
// Expected IC for standard English plaintext
#define ENGLISH_IC 0.0667
// Minimum text length required for reliable frequency analysis
#define MIN_TEXT_LENGTH 50
// Minimum sub-text length for key letter recovery
#define MIN_SUBTEXT_LENGTH 5

// Standard English letter frequencies (multiplied by 1000 for integer arithmetic, 
// but we'll stick to doubles for calculation precision)
const double ENGLISH_FREQUENCIES[26] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, // A-G
    6.094, 6.966, 0.153, 0.772, 4.025, 2.406, 6.749, // H-N
    7.507, 1.929, 0.095, 5.987, 6.327, 9.056, 2.758, // O-U
    0.978, 2.360, 0.150, 1.974, 0.074  // V-Z
};

// --- Function Prototypes ---
char* read_file_to_buffer(const char *filename);
char* cleanup_text(char *raw_text);
void free_text_buffer(char *text);
// --- Function Prototypes (Updated) ---
double calculate_ic(const char *text);
int find_key_length(const char *ciphertext, int max_key_len); // Returns best key length
// --- NEW Function Prototypes ---
double calculate_chi_squared(const int *observed_freq, size_t N, int shift);
char find_key_letter(const char *sub_text); 
int find_key(const char *ciphertext, int key_len, char *key_buffer); // NEW
// --- NEW Function Prototype ---
char* vigenere_decrypt(const char *ciphertext, const char *key);

// --- Main Program Update ---
int main() {
    const char *filename = "ciphertext.txt";
    char *raw_ciphertext = NULL;
    char *cleaned_ciphertext = NULL;
    char *plaintext = NULL; // New variable for decrypted text

    printf("--- Vigenere Frequency Analyzer ---\n");
    
    // 1. Read and Clean the text
    raw_ciphertext = read_file_to_buffer(filename);

    if (raw_ciphertext == NULL) {
        return 1;
    }

    cleaned_ciphertext = cleanup_text(raw_ciphertext);

    size_t N = strlen(cleaned_ciphertext);
    printf("Cleaned ciphertext length: %zu characters\n", N);

    // Validate minimum text length
    if (N < MIN_TEXT_LENGTH) {
        printf("\nError: Ciphertext too short for reliable analysis.\n");
        printf("       Minimum required: %d characters, got: %zu characters.\n",
               MIN_TEXT_LENGTH, N);
        printf("       Please provide a longer ciphertext.\n");
        free_text_buffer(cleaned_ciphertext);
        return 1;
    }

    // ----------------------------------------------------
    // --- Phase 2: Key Length Determination ---
    // ----------------------------------------------------
    printf("\n--- Phase 2: Key Length Determination ---\n");
    int best_key_len = find_key_length(cleaned_ciphertext, 15);

    if (best_key_len < 1) {
        printf("Error: Could not determine key length.\n");
        free_text_buffer(cleaned_ciphertext);
        return 1;
    }

    printf("\n*** Auto-detected Key Length (L) = %d (highest average IC) ***\n", best_key_len);

    // ----------------------------------------------------
    // --- Phase 3: Key Recovery ---
    // ----------------------------------------------------
    char key_buffer[best_key_len + 1];
    printf("\n--- Phase 3: Key Recovery ---\n");
    
    if (find_key(cleaned_ciphertext, best_key_len, key_buffer)) {
        printf("Recovered Key: **%s**\n", key_buffer);
        
        // ----------------------------------------------------
        // --- Phase 4: Decryption (Step 6) ---
        // ----------------------------------------------------
        plaintext = vigenere_decrypt(cleaned_ciphertext, key_buffer);
        
        if (plaintext != NULL) {
            printf("\n--- Phase 4: Decryption Result ---\n");
            printf("Plaintext: %s\n", plaintext);
            free(plaintext); // Free memory allocated by vigenere_decrypt
        }
        
    } else {
        printf("Error: Key recovery failed. Decryption skipped.\n");
    }

    // 3. Clean up memory
    free_text_buffer(cleaned_ciphertext); 

    printf("\n--- Vigenere Analyzer Process Complete! ---\n");

    return 0;
}

// --- Function Definitions ---

/**
 * @brief Reads the entire content of a file into a dynamically allocated buffer.
 * @param filename The path to the file.
 * @return A pointer to the buffer containing the file content, or NULL on failure.
 */
char* read_file_to_buffer(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return NULL;
    }

    // Allocate an initial buffer. We'll read up to MAX_TEXT_SIZE.
    char *buffer = (char *)malloc(MAX_TEXT_SIZE + 1);
    if (buffer == NULL) {
        perror("Error allocating memory");
        fclose(fp);
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, MAX_TEXT_SIZE, fp);
    buffer[bytes_read] = '\0'; // Null-terminate the string

    if (bytes_read == MAX_TEXT_SIZE) {
        printf("Warning: Input file may be truncated as it exceeded MAX_TEXT_SIZE (%d).\n", 
               MAX_TEXT_SIZE);
    }

    fclose(fp);
    return buffer;
}

/**
 * @brief Cleans the text: converts to uppercase and removes all non-alphabetic characters.
 * @param raw_text The text buffer (will be modified in-place).
 * @return A pointer to the cleaned text (same buffer).
 */
char* cleanup_text(char *raw_text) {
    if (raw_text == NULL) {
        return NULL;
    }
    
    int read_idx = 0;
    int write_idx = 0;
    
    while (raw_text[read_idx] != '\0') {
        // Convert the character to uppercase
        char upper_char = toupper((unsigned char)raw_text[read_idx]);
        
        // Check if the character is an alphabet letter
        if (isalpha((unsigned char)upper_char)) {
            // If it's a letter, write it to the current write position
            raw_text[write_idx] = upper_char;
            write_idx++;
        }
        
        // Always advance the read pointer
        read_idx++;
    }
    
    // Null-terminate the new, shorter string
    raw_text[write_idx] = '\0';
    
    return raw_text;
}

/**
 * @brief Frees the memory allocated for the text buffer.
 * @param text The pointer to the buffer.
 */
void free_text_buffer(char *text) {
    if (text != NULL) {
        free(text);
    }
}

/**
 * @brief Calculates the Index of Coincidence (IC) for a given text.
 * @param text The cleaned, uppercase text string.
 * @return The calculated IC value as a double.
 */
double calculate_ic(const char *text) {
    size_t N = strlen(text);
    
    // Check for minimum length required for N(N-1)
    if (N < 2) {
        return 0.0;
    }

    // Array to store the frequency count of letters A-Z (0-25)
    int freq[26] = {0};
    
    // 1. Count letter frequencies
    for (size_t i = 0; i < N; i++) {
        char c = text[i];
        // Since the text is cleaned (uppercase, alpha only), 
        // we can safely assume 'A' <= c <= 'Z'
        if (c >= 'A' && c <= 'Z') {
            freq[c - 'A']++;
        }
    }

    // 2. Calculate the numerator: Sum of f_i * (f_i - 1)
    long long numerator = 0;
    for (int i = 0; i < 26; i++) {
        long long f_i = freq[i];
        numerator += f_i * (f_i - 1);
    }
    
    // 3. Calculate the denominator: N * (N - 1)
    // Use double for precision in the final division
    double denominator = (double)N * (N - 1);

    // 4. Return the IC value
    return (double)numerator / denominator;
}

// --- NEW Function Definition ---

/**
 * @brief Attempts to determine the Vigenere key length using the Index of Coincidence.
 * @param ciphertext The cleaned, uppercase cipher text.
 * @param max_key_len The maximum key length to test.
 * @return The most probable key length (with highest average IC), or -1 on error.
 */
int find_key_length(const char *ciphertext, int max_key_len) {
    size_t N = strlen(ciphertext);

    // Check if the text is long enough to test the maximum key length
    if (N / 2 < (size_t)max_key_len) {
        max_key_len = (int)N / 2;
        if (max_key_len < 2) {
             printf("Error: Ciphertext too short for meaningful analysis.\n");
             return -1;
        }
    }

    // Arrays to store IC values for each key length
    double ic_values[max_key_len + 1];
    int best_key_len = 2;  // Start at 2 (L=1 is not a real Vigenere cipher)
    double best_ic = 0.0;

    printf("| Key Length (L) | Average IC |\n");
    printf("|----------------|------------|\n");

    // L = potential key length
    for (int L = 1; L <= max_key_len; L++) {
        double total_ic = 0.0;

        // Check L=1 separately: IC of the whole text
        if (L == 1) {
            // L=1 means no key; it should be close to the random IC (0.038) if it's Vigenere
            total_ic = calculate_ic(ciphertext);
            ic_values[L] = total_ic;
            printf("| %14d | %10.4f |\n", L, total_ic);
            continue;
        }

        // Divide the ciphertext into L sub-texts (columns)
        for (int i = 0; i < L; i++) {
            // Estimate the maximum possible size for the sub-text
            size_t sub_text_max_len = (N / L) + 2;

            // Dynamically allocate buffer for sub-text
            char *sub_text_buffer = (char *)malloc(sub_text_max_len);
            if (sub_text_buffer == NULL) {
                perror("Memory allocation failed");
                return -1;
            }
            int sub_text_idx = 0;

            // Extract the sub-text (characters at indices i, i+L, i+2L, ...)
            for (size_t j = i; j < N; j += L) {
                if ((size_t)sub_text_idx < sub_text_max_len - 1) {
                    sub_text_buffer[sub_text_idx++] = ciphertext[j];
                }
            }
            sub_text_buffer[sub_text_idx] = '\0'; // Null-terminate

            // Calculate IC for this sub-text and add it to the total
            if (sub_text_idx >= 2) {
                total_ic += calculate_ic(sub_text_buffer);
            }

            free(sub_text_buffer);
        }

        // Calculate the average IC
        double avg_ic = total_ic / L;
        ic_values[L] = avg_ic;

        // Track the best key length (highest IC, excluding L=1)
        if (avg_ic > best_ic) {
            best_ic = avg_ic;
            best_key_len = L;
        }

        printf("| %14d | %10.4f |%s\n", L, avg_ic,
               (L == best_key_len) ? " <-- current best" : "");
    }

    // Check if the best key length might be a multiple of a smaller valid key length
    // If a smaller divisor has similar IC (within 5%), prefer the shorter key
    for (int divisor = 2; divisor < best_key_len; divisor++) {
        if (best_key_len % divisor == 0) {
            double divisor_ic = ic_values[divisor];
            // If the divisor's IC is at least 95% of the best IC and above English threshold
            if (divisor_ic >= best_ic * 0.95 && divisor_ic >= 0.055) {
                printf("\nNote: Key length %d is a multiple of %d (IC: %.4f vs %.4f)\n",
                       best_key_len, divisor, best_ic, divisor_ic);
                printf("      Preferring shorter key length: %d\n", divisor);
                best_key_len = divisor;
                break;
            }
        }
    }

    return best_key_len;
}

/**
 * @brief Calculates the Chi-Squared statistic for a given frequency distribution 
 * after a potential decryption shift.
 * * @param observed_freq The raw frequency count (0-25) of the sub-text.
 * @param N Total length of the sub-text.
 * @param shift The assumed key shift (0=A, 1=B, ..., 25=Z).
 * @return The calculated Chi-Squared value.
 */
double calculate_chi_squared(const int *observed_freq, size_t N, int shift) {
    double chi_squared = 0.0;
    
    for (int i = 0; i < 26; i++) {
        // P_i is the plaintext letter index (0=A, 25=Z)
        // Decrypted Index = (Cipher Index - Shift + 26) mod 26
        int plaintext_idx = (i - shift + 26) % 26;
        
        // O_p: Observed count for plaintext letter P_i after shifting
        int O_p = observed_freq[i]; 
        
        // E_p: Expected count for plaintext letter P_i
        // Expected frequency is ENGLISH_FREQUENCIES[plaintext_idx] / 100 * N
        double E_p = ENGLISH_FREQUENCIES[plaintext_idx] / 100.0 * N;
        
        // Chi-Squared Formula: Sum of (Observed - Expected)^2 / Expected
        if (E_p > 0.0) {
            double diff = O_p - E_p;
            chi_squared += (diff * diff) / E_p;
        }
    }
    
    return chi_squared;
}

/**
 * @brief Finds the most probable key letter (shift) for a single sub-text 
 * using Chi-Squared analysis.
 * * @param sub_text The string containing one of the sub-texts (columns).
 * @return The most probable key letter (A-Z).
 */
char find_key_letter(const char *sub_text) {
    size_t N = strlen(sub_text);
    if (N < MIN_SUBTEXT_LENGTH) {
        printf("    Warning: Sub-text too short (%zu chars) for reliable analysis.\n", N);
        return '?'; // Too short for reliable analysis
    }

    // 1. Get raw frequency count of the sub-text
    int freq[26] = {0};
    for (size_t i = 0; i < N; i++) {
        freq[sub_text[i] - 'A']++;
    }

    // 2. Test all 26 possible shifts (0='A' to 25='Z')
    double min_chi_squared = DBL_MAX;
    int best_shift = 0;

    for (int shift = 0; shift < 26; shift++) {
        double current_chi_squared = calculate_chi_squared(freq, N, shift);
        
        if (current_chi_squared < min_chi_squared) {
            min_chi_squared = current_chi_squared;
            best_shift = shift;
        }
    }
    
    // Convert the best shift (0-25) back to a character ('A'-'Z')
    return (char)('A' + best_shift);
}

/**
 * @brief Divides the ciphertext and calls find_key_letter for each sub-text.
 * * @param ciphertext The cleaned cipher text.
 * @param key_len The determined key length (L).
 * @param key_buffer A buffer to store the resulting key (must be key_len + 1 size).
 * @return 1 on success, 0 on failure.
 */
int find_key(const char *ciphertext, int key_len, char *key_buffer) {
    size_t N = strlen(ciphertext);
    if (key_len <= 0 || (size_t)key_len > N) return 0;

    // Check if sub-texts will be long enough
    size_t expected_subtext_len = N / key_len;
    if (expected_subtext_len < MIN_SUBTEXT_LENGTH) {
        printf("Warning: With key length %d, sub-texts will only have ~%zu chars.\n",
               key_len, expected_subtext_len);
        printf("         Results may be unreliable. Consider shorter key length or longer ciphertext.\n");
    }
    
    printf("Recovering key letters...\n");

    // Loop through each sub-text/column (i = 0 to key_len - 1)
    for (int i = 0; i < key_len; i++) {
        // Estimate the maximum possible size for the sub-text
        size_t sub_text_max_len = (N / key_len) + 1; 
        
        // Dynamically allocate buffer for sub-text
        char *sub_text_buffer = (char *)malloc(sub_text_max_len);
        if (sub_text_buffer == NULL) {
            perror("Memory allocation failed for sub-text");
            return 0;
        }
        
        int sub_text_idx = 0;
        
        // Extract the sub-text (characters at indices i, i+L, i+2L, ...)
        for (size_t j = i; j < N; j += key_len) {
            if (sub_text_idx < sub_text_max_len - 1) {
                sub_text_buffer[sub_text_idx++] = ciphertext[j];
            }
        }
        sub_text_buffer[sub_text_idx] = '\0'; // Null-terminate

        // Find the key letter for this column
        char key_letter = find_key_letter(sub_text_buffer);
        key_buffer[i] = key_letter;
        
        printf("  Column %d key letter: %c\n", i + 1, key_letter);

        free(sub_text_buffer); // Clean up memory
    }
    
    key_buffer[key_len] = '\0'; // Null-terminate the full key
    return 1;
}

/**
 * @brief Decrypts the Vigenere ciphertext using the recovered key.
 * @param ciphertext The cleaned, uppercase cipher text.
 * @param key The Vigenere key (uppercase string).
 * @return A dynamically allocated string containing the plaintext, or NULL on error.
 */
char* vigenere_decrypt(const char *ciphertext, const char *key) {
    size_t N = strlen(ciphertext);
    size_t key_len = strlen(key);

    if (key_len == 0) {
        printf("Error: Key is empty.\n");
        return NULL;
    }

    // Allocate memory for the plaintext (same length as ciphertext + null terminator)
    char *plaintext = (char *)malloc(N + 1);
    if (plaintext == NULL) {
        perror("Memory allocation failed for plaintext");
        return NULL;
    }

    // Perform decryption
    for (size_t i = 0; i < N; i++) {
        // C_i: Ciphertext letter index (0-25)
        int C_i = ciphertext[i] - 'A';
        
        // K_i: Key letter index (0-25) based on modulo key length
        int K_i = key[i % key_len] - 'A';
        
        // Decryption calculation: (C_i - K_i + 26) mod 26
        // Adding 26 ensures the result is not negative before modulo
        int P_i = (C_i - K_i + 26) % 26;
        
        // Convert the index back to an uppercase character
        plaintext[i] = (char)('A' + P_i);
    }

    plaintext[N] = '\0'; // Null-terminate the plaintext string
    
    return plaintext;
}