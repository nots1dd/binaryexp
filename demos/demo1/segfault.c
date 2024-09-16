#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define BUFSIZE 32

/************************************************************************
 *
 * @Segmentation Fault 
 *
 * When we try to access a region of memory that we are not allowed to 
 * access.
 *
 * Example:
 *  CASE 1: Trying to access or modify memory through a pointer that
 *  hasn’t been properly initialized or that has been set to NULL.
 *  ---------------------------------------------------------------------
 *    int *ptr = NULL;
 *    *ptr = 10;
 *  ---------------------------------------------------------------------
 *
 * CASE 2: Writing more data to a buffer than it can hold,
 * which can overwrite adjacent memory.
 *  ---------------------------------------------------------------------
 *    char buffer[10];
 *    strcpy(buffer, "hello world i am new!!!!"); 
 *  ---------------------------------------------------------------------
 *
 *  CASE 3: Accessing elements which are out of bounds
 *  ---------------------------------------------------------------------
 *    int arr[2];
 *    arr[3] = 100;
 *  ---------------------------------------------------------------------
 *  
 *  OTHER CASES:
 *
 *  -> Stack overflow 
 *  -> Accessing memory after it has been freed
 *  -> Unaligned data type access (you have char but you give it to int)
 *
 ************************************************************************/

char flag[32] = "c0d{hidden_flag_here}";

void sigsegv_handler(int sig) {
    printf("\n[!] Segmentation fault detected!\n");
    printf("%s\n", flag);
    fflush(stdout);
    exit(1);
}

int main() {
    signal(SIGSEGV, sigsegv_handler);
    char input[BUFSIZE];

    printf("Enter your name: ");
    fgets(input, BUFSIZE, stdin);

    // Potential format string vulnerability
    printf("Hello, ");
    printf(input);  // Vulnerable! The input is used directly as the format string
    printf("\n");

    return 0;
}
