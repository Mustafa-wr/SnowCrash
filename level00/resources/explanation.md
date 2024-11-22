# Level00: File Permission Enumeration

## Description
This level demonstrates the importance of proper file permission management and the risks of storing encoded credentials in readable files. The challenge involves finding files owned by a specific user and decoding their contents to obtain credentials.

## Steps to Exploit

1. **Enumerate Files:**
   - Search for files owned by user `flag00` using:
   ```bash
   find / -user flag00
   ```
   - Found two identical files:
     - `/usr/sbin/john`
     - `/rofs/usr/sbin/john`

2. **Analyze File Contents:**
   - Both files contained the encoded string: `cdiiddwpgswtgt`
   - Analysis suggested ROT15 encoding

3. **Decode the String:**
   - Used [CacheSleuth MultiDecoder](https://www.cachesleuth.com/multidecoder/)
   - ROT15 decoding revealed: `nottoohardhere`

4. **Privilege Escalation:**
   - Used decoded string as password for user `flag00`
   - Retrieved flag using the `getflag` command

## Impact
This vulnerability demonstrates several security concerns:
- Improper storage of encoded credentials in readable files
- Weak encoding method (ROT15)
- Insufficient file permission controls

## Mitigation
1. Never store credentials, even encoded ones, in readable files
2. Use strong encryption rather than simple encoding
3. Implement proper file permission controls
4. Regular security audits to identify exposed sensitive files
