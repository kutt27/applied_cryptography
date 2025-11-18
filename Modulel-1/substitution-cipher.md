## Substitution cipher

A substitution cipher is an encryption method where each unit of plaintext (usually single letters) is replaced with another letter or symbol according to a fixed system or key. The receiver decrypts it by reversing this substitution process. This is a broad category that includes many types of ciphers.

The Caesar cipher is a specific type of substitution cipher where each letter in the plaintext is shifted by a fixed number of positions down or up the alphabet. For example, shifting by 3 means 'A' becomes 'D', 'B' becomes 'E', etc. It uses a fixed key which is the shift amount.

The key difference is that the Caesar cipher is a simple substitution cipher with a uniform shift, whereas general substitution ciphers can use any arbitrary mappings for each letter. So, while all Caesar ciphers are substitution ciphers, not all substitution ciphers are Caesar ciphers.

In substitution ciphers, there are two major types:

- Monoalphabetic substitution cipher: a fixed substitution alphabet is used for the entire message. Each letter in the plaintext maps to one consistent letter in the ciphertext. For example, if 'A' maps to 'D' at the start, it maps to 'D' throughout the message.

- Polyalphabetic substitution cipher: multiple substitution alphabets are used at different positions in the message. For instance, the substitution used for the first letter is different from that of the second letter, often cycling through a keyword's alphabets. This increases security by making frequency analysis attacks more difficult.

To summarize the concepts:

| Feature                 | Caesar Cipher             | Substitution Cipher                    |
|-------------------------|--------------------------|--------------------------------------|
| Type                    | Simple substitution      | General substitution (simple or polyalphabetic)  |
| Key                     | Fixed shift number       | Arbitrary mapping per letter          |
| Alphabet mapping        | Uniform shift across text| Can be fixed (monoalphabetic) or multiple alphabets (polyalphabetic) |
| Example                 | Shift by 3 (A→D)         | Any letter-to-letter mapping, like A→Q, B→Z, etc. |
| Security                | Weaker, easily breakable | Can be stronger, especially polyalphabetic |

With this understanding, one can implement a substitution cipher in C by specifying a key (substitution alphabet) and then performing encryption by replacing each letter from the plaintext according to this key, and decryption by reversing this mapping.

Encryption and decryption of a substitution cipher essentially rely on replacing letters according to a defined key mapping.

### Encryption Logic:
- You start with a key: a string representing the substitution alphabet (e.g., "QWERTYUIOPASDFGHJKLZXCVBNM").
- For each letter in the plaintext:
  - If it is an alphabetical character, find its position in the standard alphabet (A-Z or a-z).
  - Replace it with the letter from the key at that same position.
  - Non-alphabetical characters remain unchanged.
- The result is the ciphertext.

### Decryption Logic:
- You reverse this process:
  - Create a reverse mapping from the key back to the standard alphabet.
  - For each letter in the ciphertext, find it in the key string and replace it with the corresponding letter in the standard alphabet.
  - Non-alphabetical characters remain unchanged.

### Example:
- Suppose the key is "QWERTYUIOPASDFGHJKLZXCVBNM"
- Plaintext: "HELLO"
- Encryption:
  - H (7th index) → key = 'I'
  - E (4th) → 'T'
  - L (11th) → 'S'
  - L (11th) → 'S'
  - O (14th) → 'G'
- Ciphertext: "ITSSG"

- Decryption reverses this mapping to get back the original plaintext.

### Implementation Notes:
- Case can be preserved by checking if a character is uppercase or lowercase.
- Non-alphabetic characters (spaces, punctuation) usually remain unchanged.
- You can implement this efficiently using arrays or dictionaries for mapping letters.
- Polyalphabetic substitution varies the key along the plaintext, but the basic encryption/decryption idea remains the same.
