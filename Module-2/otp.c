#include "otp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Platform-Specific CSPRNG Setup ---

#if defined(__linux__)
    #include <sys/random.h>
    #define CSPRNG_FUNC(buf, len) getrandom(buf, len, 0)
#elif defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__)
    #include <stdlib.h> // for arc4random_buf
    #define CSPRNG_FUNC(buf, len) (arc4random_buf(buf, len) == 0 ? (ssize_t)len : -1)
#else
    // Fallback for other systems: You MUST replace this in a real app.
    // Use of standard rand() is NOT cryptographically secure!
    #warning "No secure random function defined. Using insecure fallback."
    #include <time.h>
    #define CSPRNG_FUNC(buf, len) (srand(time(NULL)), rand(), -1) 
    // The -1 return simulates an error to flag the insecure usage.
#endif

// --- Function Definitions ---

uint8_t* generate_otp_key(size_t length) {
    uint8_t* key = (uint8_t*)malloc(length);
    if (!key) {
        perror("Failed to allocate key memory");
        return NULL;
    }

    if (CSPRNG_FUNC(key, length) < 0) {
        fprintf(stderr, "ERROR: Secure random number generation failed. Check CSPRNG_FUNC definition.\n");
        free(key);
        return NULL;
    }

    return key;
}

uint8_t* xor_bytes(const uint8_t* data, const uint8_t* key, size_t length) {
    // Allocate memory for the result (ciphertext or decrypted plaintext)
    uint8_t* result = (uint8_t*)malloc(length);
    if (!result) {
        perror("Failed to allocate result memory");
        return NULL;
    }

    // Perform the bitwise XOR operation
    for (size_t i = 0; i < length; i++) {
        result[i] = data[i] ^ key[i];
    }

    return result;
}

// --- Demonstration Main Function ---

int main() {
    // Plaintexts
    const char* P1_str = "Meet me at the secure location 01.";
    // const char* P2_str = "The second message has different words.";
    const char* P2_str = "Meet me at the secure location 02.";

    // Convert string plaintexts to byte arrays and get length
    const uint8_t* P1 = (const uint8_t*)P1_str;
    const size_t len = strlen(P1_str);
    const uint8_t* P2 = (const uint8_t*)P2_str;

    // Check if P2 is the same length as P1 (a requirement for OTP reuse demonstration)
    if (strlen(P2_str) != len) {
        fprintf(stderr, "Error: Plaintext lengths must be equal for this demonstration.\n");
        return 1;
    }
    
    printf("--- OTP Simulation: Perfect Secrecy vs. Key Reuse ---\n\n");
    printf("P1 (Length %zu): %s\n", len, P1_str);
    printf("P2 (Length %zu): %s\n\n", len, P2_str);

    // 1. Generate Secure Key (K1)
    uint8_t* K1 = generate_otp_key(len);
    if (!K1) return 1;

    printf("1. Key Generation (K1) successful.\n");
    // NOTE: NEVER print the key in a real application!

    // 2. Encryption (C1 = P1 XOR K1) - Secure
    uint8_t* C1 = xor_bytes(P1, K1, len);
    if (!C1) { free(K1); return 1; }

    // 3. Decryption check (P1' = C1 XOR K1) - Verification
    uint8_t* P1_decrypted = xor_bytes(C1, K1, len);
    if (!P1_decrypted) { free(K1); free(C1); return 1; }
    
    printf("2. P1 Encrypted to C1 (One-Time Use)\n");
    printf("   Decrypted P1 check: %s\n\n", P1_decrypted);
    free(P1_decrypted); // Clean up temporary decryption result

    // 4. Insecurity Demonstration: Key Reuse
    printf("--- DEMONSTRATION OF INSECURITY (Key Reuse) ---\n");
    
    // Reuse K1 to encrypt P2 (C2 = P2 XOR K1)
    uint8_t* C2 = xor_bytes(P2, K1, len);
    if (!C2) { free(K1); free(C1); return 1; }
    
    printf("3. P2 Encrypted to C2 (Key K1 REUSED)\n");

    // Attacker's step: C1 XOR C2 = (P1 XOR K1) XOR (P2 XOR K1) = P1 XOR P2
    uint8_t* P1_XOR_P2 = xor_bytes(C1, C2, len);
    if (!P1_XOR_P2) { free(K1); free(C1); free(C2); return 1; }
    
    printf("4. Attacker computes C1 XOR C2 = P1 XOR P2:\n");
    
    // Print the result (P1 XOR P2). 
    // Note: The result is a byte array and may not be a valid UTF-8 string, 
    // but C will try to print it as a string for demonstration purposes.
    printf("   P1 XOR P2 Result: %s\n", P1_XOR_P2);
    
    printf("\nEXPLANATION OF INSECURITY:\n");
    printf("> The attacker has recovered P1 XOR P2. If the plaintexts (P1 and P2) are English text, \n");
    printf("> the attacker can use frequency analysis and crib-dragging (searching for patterns like ' the ') \n");
    printf("> in the XOR result to fully recover both P1 and P2. \n");
    printf("> Perfect secrecy is lost because the uncertainty about P1/P2 has been severely reduced.\n");

    // 5. Cleanup
    free(K1);
    free(C1);
    free(C2);
    free(P1_XOR_P2);
    
    return 0;
}