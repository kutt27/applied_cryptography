## Threat Model - Password Manager App

### Define the Scope
- **Assets**: user passwords, tokens, encryption keys, master password, and metadata like usernames/URLs.
- **Boundaries**: Server/Cloud storage. The passwords will be managed on a external server.
- **Features**: Password storage, auto-fill, password generation, biometric login, sharing, emergency access, and authenticater otp management.

### Decompose the System
- **Client-side**: Mobile/Desktop app, browser extension.
- **Server-side**: Cloud storage, authentication servers, APIs.
- **Data Flow**: 
	Client(retrieve, store) <---[API Responses]---> Server(scan url, auto-fill, save password, retrive, store)
- **Dependencies**: No external dependencies.

### Identify Threats
**STRIDE** (Spoofing, Tampering, Repudiation, Information Disclosure, Denial of Service, Elevation of Privilege) to brainstorm threats:

| Category               | Example Threats                                                                                     |
|------------------------|-----------------------------------------------------------------------------------------------------|
| **Spoofing**           | Fake login prompts, phishing attacks, session hijacking.                                            |
| **Tampering**          | Modifying stored passwords, altering encryption keys, MITM attacks during sync.                     |
| **Repudiation**        | User denies performing an action (e.g., sharing a password).                                       |
| **Information Disclosure** | Leaking passwords due to weak encryption, logging sensitive data, or insecure backups.          |
| **Denial of Service**  | Flooding sync requests, crashing the app, or locking users out.                                    |
| **Elevation of Privilege** | Exploiting vulnerabilities to gain admin access or decrypt other users' data.                   |

### Analyze and Prioritize Risks
- **Likelihood**: Phishing attacks are common, session hijacking, greedy approach are uncommon.
- **Impact**: Huge monetory issues, high threat, reputation damage, etc.
- **Risk Score**: // leaving it for blank for now

### Mitigation Strategies
For each high-priority threat, define countermeasures:

| Threat                          | Mitigation Strategy                                                                                 |
|---------------------------------|-----------------------------------------------------------------------------------------------------|
| Weak encryption                  | Use AES-256 or Argon2 for key derivation, and ensure keys never leave the device.                  |
| Phishing attacks                 | Educate users, use FIDO2/U2F for multi-factor authentication, and warn about suspicious logins.    |
| MITM during sync                 | Use TLS 1.3, certificate pinning, and end-to-end encryption.                                        |
| Local storage vulnerabilities    | Secure enclaves, device-specific encryption, and OS-level protections (e.g., iOS Keychain).       |
| Brute force on master password  | Enforce strong master passwords, rate-limiting, and account lockout.                                |

### Validate and Iterate
- **Testing**: Conduct penetration testing, code audits, and red team exercises.
- **User Feedback**: Monitor for unusual activity or user reports of suspicious behavior.
- **Update**: Regularly revisit the threat model as the app evolves (e.g., new features, OS updates, or emerging threats).

### Document Everything
- Create a **threat model report** with:
  - System diagrams.
  - List of threats and mitigations.
  - Risk assessment matrix.
  - Action items for developers/security teams.
