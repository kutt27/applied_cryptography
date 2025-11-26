## Introduction and Historical of Classical Cryptography

**Theory**:
Cryptography, the art and science of securing information, has ancient origins tracing back to early civilizations like the Egyptians who used hieroglyphic substitution ciphers and the Romans with Caesar cipher (a simple letter shift). These classical ciphers were originally developed to protect military and diplomatic communications. Over centuries, more complex polyalphabetic ciphers like the Vigenère cipher emerged to improve security against early attacks such as frequency analysis, which exploits statistical patterns in languages.

Classical cryptography relied mainly on symmetric key methods, where the same secret key is used for both encryption and decryption. While innovative for their times, these methods had critical limitations, including vulnerability to frequency analysis and brute force attacks due to short key lengths and predictable structure. Secure key distribution was also a major challenge, as the secrecy of the key was paramount to maintaining confidentiality.

In the early 20th century, electromechanical machines like the Enigma provided more complex encryption but were still eventually broken, marking a critical junction in cryptographic history. The rise of computing power post-World War II enabled the development of modern cryptography, which uses rigorous mathematical foundations and asymmetric key techniques (public-private key cryptography) to provide stronger security guarantees. Modern cryptography underpins today's digital security for online banking, communications, and data privacy.

**Basic Terminology**:
- Encryption: The process of converting plaintext into ciphertext to conceal information.
- Decryption: The process of converting ciphertext back into plaintext.
- Symmetric Cryptography: Uses a shared secret key for both encryption and decryption.
- Asymmetric Cryptography: Uses a pair of keys (public and private) for encryption and decryption.

Frequency Analysis

Theory: Statistical attack on substitution and Caesar ciphers; concept of letter frequency and common cryptanalysis techniques.

Code: Build a frequency analyzer (for letters) in Python as it’s simpler and more efficient for text processing.

Hands-on: Use frequency analyzer to break substitution and Caesar ciphers.

One-Time Pad and Perfect Secrecy

Theory: Concept of one-time pad, why it provides perfect secrecy, limitations.

Code: Implement a basic one-time pad encryption and decryption in C.

Hands-on: Simulate one-time pad usage; understand randomness and key management.

Entropy and Randomness in Cryptography

Theory: Introduction to entropy as a measure of uncertainty, relevance of randomness in encryption.

Hands-on: Write notes on entropy and randomness, ideally with code snippets demonstrating entropy calculation or random number generation in C.