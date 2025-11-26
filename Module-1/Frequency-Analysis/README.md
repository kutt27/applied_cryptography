## Frequency Analysis: Vigenère Cipher Cracking

Cracking the Vigenère cipher relies on transforming a polyalphabetic substitution (which resists simple frequency counting) back into a series of monoalphabetic ciphers. This transformation is achieved by correctly guessing the **key length** and then using **statistical analysis** on the resulting sub-texts.

---

### 1. The Vigenère Challenge: Polyalphabetic Substitution

You can refer more about Vigenère cipher in [here](../vignere-cipher.md)

The Vigenère cipher uses a key word to encrypt text. Each letter of the key determines a different Caesar shift for the corresponding letter of the plaintext.

* **Problem:** If the key is *length L*, every $L^{th}$ letter is encrypted using the **same** shift.
* **Effect:** This scrambles the overall letter frequencies, making simple frequency analysis (like that used for Caesar or Atbash ciphers) useless. If the key is long enough, the resulting ciphertext frequency distribution approaches a **uniform distribution** (all letters appear roughly $1/26^{th}$ of the time), mimicking random noise.

---

## 2. Phase 2: Determining the Key Length (The Kasiski Examination)

The core idea for Vigenère breaking is to find the periodicity of the cipher - its key length ($L$).

### Index of Coincidence (IC)

The Index of Coincidence is a statistical measure of how often two randomly chosen letters from a text will be the same.

#### A. The Formula
The IC is calculated as:
$$IC = \frac{\sum_{i=A}^{Z} f_i (f_i - 1)}{N(N-1)}$$
Where:
* $f_i$ is the frequency count of the $i$-th letter (A-Z).
* $N$ is the total length of the text.

#### B. Expected Values (The Metric)
The power of IC comes from its expected values for different languages/distributions:

| Distribution | Expected IC ($\approx$) | Meaning |
| :--- | :--- | :--- |
| **English Plaintext** | **0.0667** | High coincidence; reflects the non-uniform frequency of English (E, T, A are common). |
| **Random Text** | **0.0385** | Low coincidence; approaches $1/26$ probability for each letter. |
| **Vigenère Ciphertext (Whole Text)** | $\approx 0.038$ | Since the shifts are mixed, the whole text looks random. |

#### C. The Kasiski Method
To find the key length $L$:

1.  **Assume a Key Length ($L$):** Iterate $L = 2, 3, 4, \dots$
2.  **Separate Sub-texts (Columns):** For a key length $L$, the ciphertext is separated into $L$ sub-texts. The first sub-text consists of characters at indices $0, L, 2L, \dots$, the second at $1, L+1, 2L+1, \dots$, and so on.
3.  **Monophabetic Check:** If $L$ is the *correct* key length, each of the $L$ sub-texts was created using a **single, constant Caesar shift** (i.e., a monoalphabetic cipher).
4.  **Calculate Average IC:** The IC for each of these $L$ sub-texts should be **high** (close to **$0.0667$**), since they are simply shifted English plaintext. The IC values are then averaged.
5.  **Conclusion:** The value of $L$ that yields an average IC closest to **$0.0667$** is the most probable key length.

---

## 3. Phase 3: Recovering the Key (Chi-Squared Test)

Once the key length $L$ is known, we have $L$ independent sub-texts, each waiting to be broken by a standard frequency test. The $\chi^2$ (Chi-Squared) statistic is the most robust way to do this.

### The Chi-Squared ($\chi^2$) Statistic

The $\chi^2$ test measures the difference between an **Observed** frequency distribution (from your shifted sub-text) and an **Expected** frequency distribution (the standard English language distribution).

#### A. The Formula
$$\chi^2 = \sum_{i=0}^{25} \frac{(O_i - E_i)^2}{E_i}$$
Where:
* $O_i$ is the **Observed** count of a letter *after* a hypothetical decryption shift.
* $E_i$ is the **Expected** count of that letter in an English text of the same length (calculated using English letter frequencies).

#### B. The Process
For a single sub-text (column):

1.  **Count Frequencies:** Get the raw letter counts of the sub-text.
2.  **Test 26 Shifts:** Iterate through every possible shift (key letter $K$, from A to Z).
3.  **Calculate $\chi^2$ for each shift:** For an assumed shift $K$:
    * Shift the observed frequency distribution *back* by $K$.
    * Compare the resulting distribution to the fixed English frequency distribution using the $\chi^2$ formula.
4.  **Identify the Best Shift:** The shift $K$ that results in the **smallest $\chi^2$ value** is the most likely correct key letter for that sub-text. A smaller $\chi^2$ means the observed data is a **better fit** for the expected English model.

By repeating this process $L$ times (once for each sub-text), you recover all $L$ letters of the Vigenère key.

---

## 4. Phase 4: Decryption

Once the key $K$ is known, the final step is straightforward Vigenère decryption:

$$P_i = (C_i - K_{i \pmod{L}} + 26) \pmod{26}$$

This step is a direct application of the known formula, requiring no further analysis, as the key has already been statistically recovered.

---

## 5. Usage

### Compilation
```bash
gcc -o analyzer analyzer.c -lm
```

### Running
```bash
./analyzer
```

The analyzer reads from `ciphertext.txt` in the same directory. For best results:
- Use ciphertext of at least **100+ characters**
- The text should be Vigenère-encrypted English plaintext
- After running, check the Average IC table to identify the most likely key length (value closest to 0.0667)
- Update the `assumed_key_len` variable in `main()` if needed based on IC results

### Example Output
```
--- Phase 2: Key Length Determination ---
| Key Length (L) | Average IC |
|----------------|------------|
|              1 |     0.0421 |
|              2 |     0.0438 |
|              3 |     0.0455 |
|              4 |     0.0442 |
|              5 |     0.0661 |  <-- Closest to 0.0667, likely key length
...
Recovered Key: **LEMON**
Plaintext: ATTACKATDAWN...
```

> **Note:** The analyzer requires sufficient text length for accurate statistical analysis. Short ciphertexts (< 50 characters) may produce unreliable results.