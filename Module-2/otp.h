#ifndef OTP_H
#define OTP_H

#include <stddef.h> // for size_t
#include <stdint.h> // for uint8_t

// Function prototypes

/**
 * @brief Generates a cryptographically secure random key of a specified length.
 * * @param length The desired length of the key in bytes.
 * @return uint8_t* A pointer to the newly allocated key buffer. NULL on failure.
 * The caller is responsible for freeing this memory.
 */
uint8_t* generate_otp_key(size_t length);

/**
 * @brief Performs bitwise XOR operation between two byte arrays (data and key).
 * * @param data The input data (plaintext or ciphertext).
 * @param key The key to use for XORing.
 * @param length The length of both the data and the key (must be equal).
 * @return uint8_t* A pointer to the newly allocated ciphertext/plaintext buffer.
 * The caller is responsible for freeing this memory.
 */
uint8_t* xor_bytes(const uint8_t* data, const uint8_t* key, size_t length);

#endif