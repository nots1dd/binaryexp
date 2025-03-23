# **Restricted Shell Escape - Exploitation Guide**  

## **Overview**  
This program implements a "restricted shell" that limits user commands to a predefined set. The purpose of such a system is to prevent unauthorized execution of arbitrary commands. However, due to improper handling of environment variables and command execution, it can be exploited to escape restrictions and execute arbitrary code.  

## **Problem Description**  
The restricted shell allows users to execute a predefined set of commands:  

```
ls, pwd, lsblk, whoami, clear, export, run, pushd, dirs
```

The `run` command is designed to execute the contents of a file specified in the `GIVE_NAME` environment variable. However, due to improper validation, this feature can be abused to execute arbitrary commands.  

## **Root Cause of the Vulnerability**  
1. **Improper Environment Variable Handling**  
   - The program allows users to set `GIVE_NAME` using `export GIVE_NAME=<filename>`, but does not properly restrict its content.  
   - When `run` is executed, it retrieves the value of `GIVE_NAME` and appends it to `cat <value>`.  
   - If `GIVE_NAME` contains a filename that includes shell metacharacters, it can lead to command injection.  

2. **Unvalidated Input to `system()`**  
   - The program constructs a shell command using `std::string cat_command = "cat " + std::string(cmd);` and passes it to `system()`.  
   - Since `system()` executes the command in a shell, any special characters within `GIVE_NAME` can alter execution flow.

> [!NOTE]
> 
> One niche thing to note is that the code tries to unset the `GIVE_NAME` env var at the beginning of the main func.
> 
> So if you try to set the env like so:
> 
> ```sh 
> export GIVE_NAME="flag.txt"
> ./env_var # will NOT work
> ```
> 
> This will **NOT** work as the environment variable is unset for the lifetime of the binary!
> 

## **Exploitation**  
### **1. Command Injection via `GIVE_NAME`**  
Since `system("cat " + GIVE_NAME)` is executed without sanitization, an attacker can exploit it by setting `GIVE_NAME` to a command such as:  

```sh
export GIVE_NAME="flag.txt; id"
run
```

**Explanation:**  
- The shell interprets `cat flag.txt; id` as two separate commands:  
  1. `cat flag.txt` (prints the file contents)  
  2. `id` (executes an arbitrary system command to print user identity)  

### **2. Arbitrary Code Execution**  
A more dangerous variation allows arbitrary command execution:  

```sh
export GIVE_NAME="flag.txt; bash -i"
run
```

**Effect:**  
- The command will first print `flag.txt`, then spawn an interactive bash shell, effectively escaping the restricted environment.  

### **3. Gaining Persistent Access**  
An attacker could modify system files or create a backdoor:  

```sh
export GIVE_NAME="flag.txt; echo 'malicious_code' > /tmp/backdoor.sh; chmod +x /tmp/backdoor.sh"
run
```

This creates an executable script that can later be triggered by the attacker.  

## **Mitigation**  
To prevent this vulnerability, the following security measures should be implemented:  

1. **Avoid `system()` Calls**  
   - Instead of constructing commands dynamically, use safer alternatives like `execve()`, which does not invoke a shell.  

2. **Strictly Validate Input**  
   - Ensure that `GIVE_NAME` only contains a valid filename without special characters.  

3. **Sanitize Environment Variables**  
   - Remove or escape shell metacharacters before using environment variables in commands.  

4. **Use a Sandbox or Restricted Execution Environment**  
   - Running the restricted shell in a controlled environment can limit the impact of any exploit attempts.  

## **Conclusion**  
This vulnerability demonstrates the dangers of improper command execution in a restricted shell. By leveraging command injection via environment variables, an attacker can easily bypass restrictions and execute arbitrary commands. Proper input validation and avoiding unsafe functions like `system()` are crucial in preventing such exploits.
