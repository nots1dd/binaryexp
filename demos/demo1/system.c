#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/****************************************
 * Vulnerable system call:
 * This program uses the `system()` function to execute shell commands.
 * It reads user input using `scanf()` and uses `system()` to execute 
 * the `echo` command with the input string.
 * 
 * The input is unsafely formatted directly into the `system()` call 
 * via `sprintf()`, allowing a format string vulnerability that can 
 * be exploited for command injection.
 ****************************************/

int main() {
    char user_input[100];
    char command[120];

    // Prompt the user for input
    printf("Hello!\nSend your message to the terminal!!\n");
    printf("Enter your message: ");
    fflush(stdout);

    /****************************************
     * Vulnerable Input:
     * The program reads user input using `scanf("%[^\n]s")`. This takes inputs till \n.
     * However, the vulnerability exists in how this input 
     * is later used in the `sprintf()` function without proper sanitization.
     ****************************************/
    scanf("%[^\n]s", user_input);
    /****************************************************
     *
     * @USING FGETS 
     *
     * It is much safer to use fgets in terms of avoiding
     * buffer overflows, but in this case, it doesnt
     * matter how you take the input in as the vulnerability
     * is in `system()` and the logic in the code rather
     * than just the input
     *
     ****************************************************/
    // fgets(user_input, sizeof(user_input), stdin);

    // user_input[strcspn(user_input, "\n")] = 0;

    /****************************************
     * Vulnerable sprintf call:
     * Here, the input is inserted into the `command` string using `sprintf()`. 
     * The user input is directly formatted into the string without specifying 
     * a format specifier like `%s`. This causes a **format string vulnerability**.
     * 
     * If a user provides input like `%s` or `%x`, the program will interpret it 
     * as a format specifier, potentially causing the program to crash or expose 
     * sensitive information. Worse, the user can inject malicious shell commands.
     ****************************************/
    /*
     * $ALTERNATIVE 
     *
     * Not specifying the format can lead to injections of specifiers like %x, %p 
     *
     * That can read your memory, pointer data that might provide the attacker with 
     *
     * sensitive information
     *
     * sprintf(command, "echo ", user_input)
     */
    sprintf(command, "echo %s", user_input);

    /****************************************
     * Dangerous system() execution:
     * The `command` string is passed to `system()`, which executes it as a shell 
     * command. Since the user controls part of this string, they can inject 
     * arbitrary commands into the shell, leading to **command injection**.
     ****************************************/
    system(command);

    return 0;
}

/****************************************
 * Exploitation Explanation:
 *
 * 1. **Format String Vulnerability**:
 *    The key vulnerability lies in how `sprintf()` is used:
 *    ```
 *    sprintf(command, "echo ", user_input);
 *    ```
 *    The user-controlled input (`user_input`) is inserted into the `command` 
 *    string without a proper format specifier like `%s`. If the user provides 
 *    format specifiers (like `%s`, `%x`, etc.), they can potentially read memory 
 *    or cause undefined behavior.
 *
 * 2. **Command Injection**:
 *    Even worse, the `command` string is passed to `system()`, which executes 
 *    shell commands. This means a malicious user can insert shell commands 
 *    after their input. For example, if the user input is:
 *    ```
 *    hello; rm -rf /
 *    ```
 *    This results in the following `command`:
 *    ```
 *    echo hello; rm -rf /
 *    ```
 *    The shell interprets this as two commands: `echo hello` and `rm -rf /`, 
 *    causing potentially catastrophic results.
 *
 * 3. **Example Exploit**:
 *    Input:
 *    ```
 *    %s; touch /tmp/hacked
 *    ```
 *    This input would cause the following command to execute:
 *    ```
 *    echo %s; touch /tmp/hacked
 *    ```
 *    This not only attempts to interpret the format specifier `%s`, but also 
 *    creates a file `/tmp/hacked`.
 *
 * **How to Fix**:
 * 1. Always use format specifiers when using `sprintf()`. Replace:
 *    ```
 *    sprintf(command, "echo %s", user_input);
 *    ```
 *    This ensures that `user_input` is treated as a string and not as part of 
 *    the format string.
 *
 *    In this example, even though we provided a specifier %s, it still is very 
 *    vulnerable to command injection.
 *
 *    Using `snprintf(buf, max, "%s\n"%s)` with a buffer and using popen execvp and other
 *    safter methods that reads the output is MUCH safer to use in C
 *
 * 2. Escape or sanitize user input before passing it to shell commands. 
 *    Avoid `system()` for user-controlled data and use safer alternatives like `execvp()`.
 *
 * 3. Limit the size of user input to avoid buffer overflow vulnerabilities 
 *    and check for dangerous shell metacharacters like `;`, `|`, `&`, etc.
 ****************************************/
