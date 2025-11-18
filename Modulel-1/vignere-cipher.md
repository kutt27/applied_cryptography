## Vigenère cipher

The Vigenère cipher is a polyalphabetic substitution cipher, which means it uses multiple Caesar ciphers based on a keyword to encrypt the plaintext. Unlike the Caesar cipher, which uses a fixed shift for all letters, the Vigenère cipher varies the shift depending on the corresponding letter of the repeating keyword, making it much harder to break.

### Theory:
- The plaintext letters are encrypted by shifting them according to the letters of the keyword.
- Each letter of the keyword determines a Caesar shift (A means shift by 0, B by 1, C by 2, etc.).
- The keyword is repeated as needed to match the length of the plaintext.
- This creates a series of Caesar shifts rather than a single constant shift.
- Decryption uses the inverse shifts corresponding to the keyword letters.
- Because of varying shifts, the cipher resists simple frequency analysis that breaks Caesar or monoalphabetic substitution ciphers.
- The primary weakness lies in the repeating nature of the keyword; once the key length is known, the cipher can be broken by treating the ciphertext as interwoven Caesar ciphers.

### Hands-on Understanding:
- Encrypting "HELLO" with key "KEY":
  - 'K' = shift by 10, 'E' = shift by 4, 'Y' = shift by 24.
  - H + K shift = R, E + E shift = I, L + Y shift = J, L + K shift = V, O + E shift = S.
  - Ciphertext = "RIJVS".
- This polyalphabetic approach increases key space exponentially and increases security against brute force.
- Yet it is breakable with enough ciphertext using statistical methods (Kasiski examination, Friedman test).

### Summary Comparison with Caesar Cipher:

| Aspect                  | Caesar Cipher                   | Vigenère Cipher                          |
|-------------------------|---------------------------------|-----------------------------------------|
| Cipher Type             | Monoalphabetic substitution     | Polyalphabetic substitution             |
| Key                     | Single integer (shift)          | Keyword (string)                         |
| Shift for letters       | Constant for whole text         | Varies per letter via keyword letter    |
| Security                | Weak, easily broken             | Stronger, but vulnerable if key repeats |
| Resistance to frequency analysis | Low                  | Higher                                  |
| Complexity              | Simple                         | Moderate                                |
| Key Space               | 26 possible keys (English)      | Very large (length and characters vary) |

[Code Implementation](vigenère.c)
