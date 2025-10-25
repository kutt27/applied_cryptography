## Caesar Cipher - Algorithm
---
1. **Mapping letter to numbers**: We assign numerical value to each letter. Eg, A is 1, B is 2, and so on.
2. **Selecting a key**: Key is determined by selecting a number between 1 and 25. This input will determine the key value.
3. **Encryption:** To encrypt a letter, we add the numerical value of letter to the key value and then it's modulo by 26. The result is the numerical value of the encrypted letter.
4. **Decryption:** To decrypt a letter, subtract  the key from the encrypted letter's numerical value, again mod 26.
---
[] Change of plans: After 6 month of not taking it continously, switched to C from C++ for more expressive implementation.
---


## Monoalphabetic Cipher - Algorithm
---
Code status: --Incomplete
1. **Key Generation:**  
   Create a substitution key by mapping each letter of the plaintext alphabet (A–Z) to a unique ciphertext letter (A–Z).  
   - Ensure each mapping is one-to-one (no duplicates).  
   - This creates 26 unique substitution pairs.
2. **Encipher (Encryption):**  
   For each letter in the plaintext, replace it with its corresponding ciphertext letter according to the key.
3. **Decipher (Decryption):**  
   Use the inverse of the substitution key to map each ciphertext letter back to its original plaintext letter.

