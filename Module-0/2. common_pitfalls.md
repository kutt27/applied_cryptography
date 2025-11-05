Common pitfalls in cryptography include rolling your own crypto, side-channel attacks, and misuse of cryptographic primitives, each posing significant risks to security.

### Rolling Your Own Crypto
- Designing custom cryptographic algorithms or protocols without deep expertise often results in vulnerabilities because cryptography is highly complex.
- Experts recommend using well-established, peer-reviewed cryptographic libraries and standards rather than inventing new algorithms or ad hoc methods.
- Mistakes in implementation or design can lead to weak encryption, making the system vulnerable to attacks.

### Side-Channel Attacks
- Side-channel attacks exploit indirect information leaked during cryptographic operations, such as timing, power consumption, electromagnetic emissions, or acoustic signals.
- These attacks can reveal secret keys or sensitive data without breaking the cryptographic algorithm mathematically.
- Common countermeasures include shielding hardware, using constant-time algorithms, masking intermediate values, adding noise/random delays, and offloading crypto operations to secure hardware (HSMs).
- Regular testing and auditing, secure hardware design, software techniques like randomization, and isolation of sensitive processes help mitigate side-channel risks.

[Practical side-channel attack](https://www.youtube.com/watch?v=2-zQp26nbY8)

### Misuse of Cryptographic Primitives
- Inappropriate or incorrect use of cryptographic components (e.g., using weak keys, improper key management, wrong modes of encryption, or reusing nonces) can weaken overall security.
- Even strong algorithms fail if implemented improperly or used in the wrong context, such as encrypting without authentication or neglecting secure key storage.
- Developers must ensure correct usage patterns following best cryptographic practices and standards.

In summary, secure cryptographic systems require using vetted libraries, understanding side-channel risks with proper protections, and correctly applying cryptographic primitives. Neglecting these leads to exploitable vulnerabilities despite using strong algorithms.
