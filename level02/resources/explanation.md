# Level02: Network Packet Analysis

## Description
This level focuses on network packet analysis and demonstrates the risks of unencrypted network traffic. The challenge provides a packet capture (PCAP) file that contains network traffic data, which when analyzed reveals login credentials.

## Steps to Exploit

1. **Initial Enumeration:**
   - Found `level02.pcap` file in home directory
   - PCAP file contains captured network traffic data

2. **Packet Analysis:**
   - Used `strings` command to examine readable content:
   ```bash
   strings level02.pcap
   ```
   - Found a login attempt containing "Password: Nf&Nat"
   - Further analysis revealed the actual password was being transmitted character by character
   - Each keystroke was captured in the network traffic

3. **Password Reconstruction:**
   - Analyzing the captured keystrokes:
     - 'f' (press)
     - 't' (press)
     - '_' (press)
     - 'w' (press)
     - 'a' (press)
     - 'N' (press)
     - 'D' (press)
     - 'R' (press)
     - 'e' (press)
     - 'L' (press)
     - '0' (press)
     - 'L' (press)
   - Reconstructed password: `ft_waNDReL0L`

4. **Privilege Escalation:**
   - Used reconstructed password to switch to flag02 user:
   ```bash
   su flag02
   ```
   - Retrieved flag using `getflag` command

## Impact
This vulnerability demonstrates:
- Risks of unencrypted network communications
- Importance of using secure protocols for data transmission
- How packet analysis can reveal sensitive information

## Mitigation
1. Use encrypted protocols (SSH, HTTPS) for all sensitive communications
2. Implement network segmentation
3. Regular network traffic monitoring
4. Use secure password transmission methods

## Tools Used
- `strings`: Basic analysis of readable content in binary files
- Basic Linux commands for file examination
