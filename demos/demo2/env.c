#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SCRIPT_FILE "/tmp/restricted_sh.sh"
#define GIVE_NAME_ENV "GIVE_NAME"

/********************************************************************************
 *
 * @COMMAND INJECTION
 *
 * This is an example of command injection and restricted shell
 *
 * $PROBLEM:
 * The binary will try to read a "GIVE_NAME" environment variable and attempt
 * to execute it
 *
 * It will then launch a restricted shell that will not allow to read, edit files
 *
 * This type of question can be given in a netcat instance and the user needs to
 * find a flag.txt
 *
 * $SOLUTION
 * Upon noticing that the binary attempts to execute the "cat $(GIVE_NAME)" env var,
 * We have to go to a directory flag/ to get flag.txt
 *
 * Notice that we can use pushd and dirs, they seem weird. What are they?
 *
 * Basically bash has a directory stack of the recent most directories you have
 * been to
 * 
 * Works exactly like a stack (push, pop)
 *
 * Ex: `pushd <some-dir>` -> that is on top of the dirstack
 *
 * Bash will automatically set the current directory to the top of the dirstack
 *
 * So to traverse directories just use `pushd <gotodir>`
 *
 * We will then go inside flag/ where there is indeed a flag.txt
 *
 * Now in order to cat it we can see the script that when we type run
 * it attempts to cat $GIVE_NAME environment variable
 *
 * We simple export GIVE_NAME=flag.txt
 *
 * and type run
 *
 * It will then give us the flag
 * 
 *
 ********************************************************************************/

const char *restricted_shell_script = "#!/bin/sh\n"
    "# Restricted shell script\n"
    "\n"
    "# Define allowed commands\n"
    "ALLOWED_COMMANDS=\"ls pwd lsblk whoami clear export run pushd dirs\"\n"
    "\n"
    "# Function to handle command execution\n"
    "restricted_exec() {\n"
    "    if echo \"$ALLOWED_COMMANDS\" | grep -wq \"$1\"; then\n"
    "        if [ \"$1\" = \"run\" ]; then\n"
    "            # Execute the command stored in GIVE_NAME_ENV\n"
    "            command=$(printenv GIVE_NAME)\n"
    "            echo \"Debug: Command from GIVE_NAME: '$command'\"\n"
    "            # cat $command\n"
    "            if [ -n \"$command\" ]; then\n"
    "                # Safely handle command execution with quotes\n"
    "                bash -c \"echo 'Your name is $command'; cat $command\"\n"
    "            else\n"
    "                echo \"GIVE_NAME environment variable is not set.\"\n"
    "            fi\n"
    "        else\n"
    "            # Execute other allowed commands\n"
    "            \"$@\"\n"
    "        fi\n"
    "    else\n"
    "        echo \"Command '$1' is not allowed.\"\n"
    "    fi\n"
    "}\n"
    "\n"
    "# Set environment to restrict path access\n"
    "export PATH=/usr/bin:/bin\n"
    "\n"
    "# Display initial message\n"
    "echo \"You are in a restricted shell.\"\n"
    "echo \"Allowed commands: ls, pwd, lsblk, whoami, clear, export, run, pushd, dirs\"\n"
    "\n"
    "# Main loop to read and execute commands\n"
    "while true; do\n"
    "    # Prompt user\n"
    "    echo -n \"$ \"\n"
    "    \n"
    "    # Read command from user input\n"
    "    read -r input\n"
    "    \n"
    "    # Execute command\n"
    "    restricted_exec $input\n"
    "done\n";

int main() {
    // Remove GIVE_NAME environment variable if it exists
    unsetenv(GIVE_NAME_ENV);

    // Write the restricted shell script to a temporary file
    int fd = open(SCRIPT_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0700);
    if (fd < 0) {
        perror("Failed to open temporary script file");
        return 1;
    }

    if (write(fd, restricted_shell_script, strlen(restricted_shell_script)) != strlen(restricted_shell_script)) {
        perror("Failed to write to temporary script file");
        close(fd);
        return 1;
    }

    close(fd);

    // Launch the restricted shell
    printf("\nLaunching restricted shell...\n");

    char *args[] = {SCRIPT_FILE, NULL};

    execv(SCRIPT_FILE, args);

    // If execv returns, it must have failed
    perror("execv");
    return 1;
}
