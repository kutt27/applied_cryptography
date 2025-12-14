## Module 2: Information Theory & Perfect Secrecy

### 1\. Information Theory Fundamentals

To mathematically formalize security, we need tools to measure the uncertainty and information content within messages and keys. This is where Claude Shannon's Information Theory comes in.

#### A. Shannon Entropy ($H(X)$)

Shannon entropy is the measure of the **uncertainty** or **randomness** of a random variable $X$. In cryptography, this measures how much information an attacker *gains* by learning the value of $X$ (or, conversely, how much uncertainty they *lose*).

  * **Theory:** The higher the entropy, the less predictable the variable is. For a key, high entropy means a stronger, harder-to-guess key.
  * **Math:** For a discrete random variable $X$ with alphabet $\mathcal{X}$ and probability mass function $P(x)$, the entropy $H(X)$ is defined as:

$$H(X) = - \sum_{x \in \mathcal{X}} P(x) \log_2 P(x)$$

  * **Units:** Bits (since we use $\log_2$).
  * **Maximum Entropy:** For an alphabet of size $N$, the maximum entropy is $\log_2 N$. This is achieved when all outcomes are equally likely (a uniform distribution).

#### B. Conditional Entropy ($H(X|Y)$)

Conditional entropy measures the remaining uncertainty about $X$ *given* that the value of $Y$ is known.

  * **Theory:** In a cryptographic context, $H(\text{Key}|\text{Ciphertext})$ is the uncertainty about the key *after* seeing the ciphertext.
  * **Math:**
    $$H(X|Y) = - \sum_{y \in \mathcal{Y}} P(y) \sum_{x \in \mathcal{X}} P(x|y) \log_2 P(x|y)$$

#### C. Mutual Information ($I(X;Y)$)

Mutual information measures the **amount of information** that one random variable contains about another. It quantifies the *reduction in uncertainty* about $X$ achieved by knowing $Y$.

  * **Theory:** In security, we want the mutual information between the Ciphertext ($C$) and the Plaintext ($P$) to be **zero**, i.e., $I(P; C) = 0$. This means seeing the ciphertext gives the attacker *zero* information about the original plaintext.
  * **Math:**
    $$I(X;Y) = H(X) - H(X|Y)$$

If $I(X;Y) = 0$, then $H(X|Y) = H(X)$. This means the uncertainty of $X$ given $Y$ is the same as the uncertainty of $X$ initially. Knowing $Y$ tells you nothing about $X$.

-----

### 2\. Perfect Secrecy

The formal mathematical definition of "secure" (specifically, perfectly secure) is rooted in the concept of Mutual Information.

#### A. Formal Definition of Perfect Secrecy

A cryptosystem provides **perfect secrecy** if the probability distribution of the plaintext $P$ is independent of the resulting ciphertext $C$.

Mathematically, this means:
$$P(P=p | C=c) = P(P=p)$$
for all possible plaintexts $p$ and ciphertexts $c$.

  * **In English:** Seeing the ciphertext $C$ does not change the attacker's prior belief about what the plaintext $P$ is. The a posteriori probability equals the a priori probability.
  * **In Information Theory Terms:** The mutual information between the plaintext $P$ and the ciphertext $C$ must be zero:
    $$I(P; C) = 0$$

#### B. Proof of One-Time Pad (OTP) Security

The One-Time Pad (Vernam Cipher) is the **only known practical cryptosystem** that achieves perfect secrecy.

  * **The System:**

      * **Plaintext space ($\mathcal{P}$):** Binary strings of length $L$.
      * **Ciphertext space ($\mathcal{C}$):** Binary strings of length $L$.
      * **Key space ($\mathcal{K}$):** Binary strings of length $L$, **chosen uniformly at random** from $\mathcal{K}$.
      * **Encryption:** $C = P \oplus K$ (bitwise XOR)
      * **Decryption:** $P = C \oplus K$

  * **The Proof (Demonstrating $P(P=p | C=c) = P(P=p)$):**

<!-- end list -->

1.  We need to find $P(P=p | C=c)$. Using Bayes' theorem:
    $$P(P=p | C=c) = \frac{P(C=c | P=p) \cdot P(P=p)}{P(C=c)}$$

2.  Focus on the numerator: $P(C=c | P=p)$.
    Since $C = P \oplus K$, the event $C=c$ given $P=p$ only occurs if the key $K$ satisfies $c = p \oplus K$, which means $K = p \oplus c$.
    Because the key $K$ is chosen **uniformly at random** from a space of size $|\mathcal{K}| = 2^L$, the probability of selecting any specific key is $1/2^L$.
    Therefore:
    $$P(C=c | P=p) = P(K = p \oplus c) = \frac{1}{|\mathcal{K}|}$$

3.  Now, calculate the denominator: $P(C=c)$.
    $$P(C=c) = \sum_{p' \in \mathcal{P}} P(C=c | P=p') \cdot P(P=p')$$
    Substitute the result from step 2:
    $$P(C=c) = \sum_{p' \in \mathcal{P}} \frac{1}{|\mathcal{K}|} \cdot P(P=p') = \frac{1}{|\mathcal{K}|} \sum_{p' \in \mathcal{P}} P(P=p')$$
    Since the sum of probabilities for all possible plaintexts is 1 ($\sum P(P=p') = 1$):
    $$P(C=c) = \frac{1}{|\mathcal{K}|}$$

4.  Substitute the numerator and denominator back into the Bayes' equation (step 1):
    $$P(P=p | C=c) = \frac{P(C=c | P=p) \cdot P(P=p)}{P(C=c)} = \frac{\frac{1}{|\mathcal{K}|} \cdot P(P=p)}{\frac{1}{|\mathcal{K}|}}$$

5.  **The Result:**
    $$P(P=p | C=c) = P(P=p)$$
    Perfect secrecy is proven. An attacker observing $C=c$ learns nothing new about $P$.

-----

### 3\. Limitations of Perfect Secrecy

While mathematically perfect, the OTP has severe practical drawbacks, which explains why we rely on weaker, but more practical, cryptosystems (like AES).

#### A. Key Size Constraint

  * **The Rule:** A necessary condition for a cryptosystem to achieve perfect secrecy is that the size of the key space must be **at least as large** as the size of the plaintext space:
    $$|\mathcal{K}| \ge |\mathcal{P}|$$
  * **Implication for OTP:** The key must be as long as the message itself ($|K| = |P|$). This means for every gigabyte of data you want to encrypt, you need a unique, pre-shared, gigabyte-long key. This is the **key distribution problem**.

#### B. Key Reuse (The Fatal Flaw)

  * **The Rule:** The "One-Time" in OTP is non-negotiable. The key $K$ must only be used **once**.
  * **The Insecurity:** If you encrypt two distinct plaintexts, $P_1$ and $P_2$, with the same key $K$:
    $$C_1 = P_1 \oplus K$$
    $$C_2 = P_2 \oplus K$$
    An attacker who intercepts both ciphertexts $C_1$ and $C_2$ can easily break the system:
    $$C_1 \oplus C_2 = (P_1 \oplus K) \oplus (P_2 \oplus K)$$
    Since $K \oplus K = 0$:
    $$C_1 \oplus C_2 = P_1 \oplus P_2$$
    While the attacker hasn't recovered $P_1$ or $P_2$ directly, they have recovered $P_1 \oplus P_2$. This **removes the perfect secrecy** because an attacker has reduced the uncertainty about the plaintexts. For natural language text, analyzing $P_1 \oplus P_2$ through frequency analysis (since spaces and common letters appear frequently) allows for a relatively easy recovery of both messages.

-----

### 4\. Deliverable: Simulate OTP and Demonstrate Insecurity

1. Header File: otp.h

    This file declares the necessary functions and includes standard libraries. [code](otp.h)

2. Source File: otp.c

    This file contains the function definitions and the main program to demonstrate OTP security and key reuse insecurity.

    Crucial Note on CSPRNG: In a real-world C application, you would use a platform-specific library for secure randomness (e.g., arc4random_buf on BSD/macOS, or the Windows CryptoAPI). For cross-platform simplicity, this code uses getrandom (Linux) or falls back to arc4random_buf if available.

    [Code](otp.c)

3. How to Compile and Run (Linux/macOS)

    1. Save the code above as otp.h and otp.c.
    2. Compile using a standard C compiler (like GCC or Clang):
    ```sh
    # For Linux (using getrandom):
    clang otp.c -o otp

    # If using the insecure fallback, the compiler will show a warning, 
    # but the executable will still be created.
    ```

4. Run the executables:

    ```sh
    ./otp
    ```

Output:

```text
--- OTP Simulation: Perfect Secrecy vs. Key Reuse ---

P1 (Length 34): Meet me at the secure location 01.
P2 (Length 34): Meet me at the secure location 02.

1. Key Generation (K1) successful.
2. P1 Encrypted to C1 (One-Time Use)
   Decrypted P1 check: Meet me at the secure location 01.

--- DEMONSTRATION OF INSECURITY (Key Reuse) ---
3. P2 Encrypted to C2 (Key K1 REUSED)
4. Attacker computes C1 XOR C2 = P1 XOR P2:
   P1 XOR P2 Result: 

EXPLANATION OF INSECURITY:
> The attacker has recovered P1 XOR P2. If the plaintexts (P1 and P2) are English text, 
> the attacker can use frequency analysis and crib-dragging (searching for patterns like ' the ') 
> in the XOR result to fully recover both P1 and P2. 
> Perfect secrecy is lost because the uncertainty about P1/P2 has been severely reduced.
```