# Level01: Exposed credential in /etc/passwd — local account compromise

## Description
This level demonstrates an information exposure vulnerability where a credential-like value is present in the password field of /etc/passwd for user `flag01`. The exposed value was weak/reversible and was cracked externally with John the Ripper to obtain the real password, allowing an attacker to switch user and run getflag to retrieve the token.

## Steps to Exploit

1. **Enumerate users**
   - Inspect `/etc/passwd` to look for misconfigured accounts:
   ```bash
   cat /etc/passwd
   ```
   - Found an entry for `flag01` with the second field set to `42hDRfypTqqnw` (this should not be a plain or reversible password).


2. **Crack the exposed value**
   - Copied the exposed string into a file and used John the Ripper (outside the target machine) to recover the actual password:
   ```bash
   echo 42hDRfypTqqnw > passwd
   john --show ../../passwd
   ```
   - John revealed the plaintext password: `abcdefg`.

3. **Gain account access and read the flag**
   - Use the cracked password to switch to `flag01` and run getflag:
   ```bash
   su flag01
   # enter: abcdefg
   getflag
   ```
   - Retrieved token:
   ```
   f2av5il02puano7naaf6adaaf
   ```

## Impact
- Local privilege/account compromise: any user who can read `/etc/passwd` (or other world-readable files containing credential data) can attempt to crack exposed credentials and impersonate accounts.
- Exposes sensitive tokens when privileged or special-purpose accounts are accessed.

## Mitigation
1. Do not store passwords or password-equivalent values in `/etc/passwd`. 
2. Ensure password hashes are salted and use modern hashing algorithms (sha256).
3. Restrict access to sensitive files and audit their contents regularly.
4. Enforce strong password policies to prevent trivial cracking.
