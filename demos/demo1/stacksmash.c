#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 12

/***********************************************************************************
 * 
 * @Stack Smashing
 *
 * A specific type of buffer overflow that occurs when a program writes more data to a buffer
 * on the stack than it was allocated. This overflow can overwrite adjacent memory areas, 
 * such as the function’s return address, leading to unpredictable behavior
 *
 * Examples:
 *
 * CASE 1:
 * --------------------------------------------------------------------------------
 *  void vulnerable_function() {
 *     char buffer[10];
 *     strcpy(buffer, "This string is too long for the buffer!");
 *   }
 * --------------------------------------------------------------------------------
 *
 *  CASE 2: UNCONTROLLED INPUT
 * --------------------------------------------------------------------------------
 *    void get_input() {
 *       char buffer[100];
 *       gets(buffer);  // Unsafe function that can cause buffer overflow
 *   }
 * --------------------------------------------------------------------------------
 *
 *  $FIX
 *
 *  -> Set up your own __stack_chk_fail() handler to ensure security
 *  -> Use stack canaries (compile code with -fstack-protector flag)
 *
 ***********************************************************************************/

char flag[32] = "c0d{hello_world}";

void __stack_chk_fail() {
    // When a stack smash is detected, print the flag and exit
    printf("\n[!] Stack smashing detected!\n");
    printf("%s\n", flag);
    fflush(stdout);
    exit(1);
}

int main() {
  char helo[BUFSIZE];
  scanf("%s", helo);

  return 0;
}
