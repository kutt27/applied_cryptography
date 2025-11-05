Caesar Cipher

The Caesar Cipher is one of the simplest and oldest encryption techniques, famously used by Julius Caesar to protect his military communications. It is a type of substitution cipher where each letter in the plaintext is shifted by a fixed number of positions down the alphabet.

### Encryption Mechanism:
- You select a shift value $ n $ (also called the key), between 1 and 25.
- Each letter in the plaintext is replaced by the letter that is $ n $ positions to the right in the alphabet.
- If the shift goes past 'Z', it wraps around back to 'A' (modular arithmetic on 26 letters).
- For example, with $ n = 3 $, $ A \to D $, $ B \to E $, $ C \to F $, ..., $ Y \to B $, $ Z \to C $.

Mathematically, the encryption for a letter represented as a number $ x $ from 0 (A) to 25 (Z) is:
$$
E_n(x) = (x + n) \mod 26
$$

### Decryption Mechanism:
- To decrypt the ciphertext, the receiver shifts each letter back by the same number $ n $ to restore the original message:
$$
D_n(x) = (x - n) \mod 26
$$

### Example:
Encrypting "HELLO" with a shift of 3:
- H (7) becomes K (10)
- E (4) becomes H (7)
- L (11) becomes O (14)
- L (11) becomes O (14)
- O (14) becomes R (17)

Result: "KHOOR"

To decrypt, each letter in "KHOOR" is shifted back by 3 to get "HELLO".

### Properties:
- The Caesar cipher only shifts letters; non-alphabetical characters remain unchanged.
- It is a symmetric key algorithm as the same key $ n $ is used for both encryption and decryption.
- Because the key space is small (only 25 possible shifts), it is vulnerable to brute-force attacks.
- It provides minimal security and is mainly used for educational purposes to introduce cryptography concepts.

This technique exemplifies the basics of substitution ciphers and modular arithmetic in cryptography, making it foundational to both classical and modern cryptography learning.

Code: [Link](ceaser_cipher.c)

If you take a look at the code, and the ouput, you can see the issue their. Theirs a pattern in the output. Where 'o' is repeated twice in the output, so we can scope down the guess to few constraints and it's a simple brute force to get the desired input correctly without finding the key. Pretty basic encryption.