## Caesar Cipher - Algorithm
---
1. **Mapping letter to numbers**: We assign numerical value to each letter. Eg, A is 1, B is 2, and so on.
2. **Selecting a key**: Key is determined by selecting a number between 1 and 25. This input will determine the key value.
3. **Encryption:** To encrypt a letter, we add the numerical value of letter to the key value and then it's modulo by 26. The result is the numerical value of the encrypted letter.
4. **Decryption:** To decrypt a letter, subtract  the key from the encrypted letter's numerical value, again mod 26.

