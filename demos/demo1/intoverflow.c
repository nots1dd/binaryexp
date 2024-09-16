#include <stdio.h>
#include <stdlib.h>

/****************************************
 * Function to check for integer overflow:
 * This function takes two integers `a` and `b`, performs their addition, and checks 
 * if the result causes an overflow. It returns -1 if an overflow occurs, and 0 otherwise.
 ****************************************/
static int addIntOvf(int result, int a, int b) {
    // Perform the addition
    result = a + b;

    // Check for positive overflow (if both numbers are positive but result is negative)
    if (a > 0 && b > 0 && result < 0) 
        return -1;

    // Check for negative overflow (if both numbers are negative but result is positive)
    if (a < 0 && b < 0 && result > 0) 
        return -1;

    // No overflow detected
    return 0;
}

int main() {
    int num1, num2, sum;   // Variables to store the input numbers and their sum
    FILE *flag;            // File pointer for reading the flag
    char c;                // Temporary character variable (unused here)

    /****************************************
     * Initial prompt to the user:
     * The program prints an explanation that the user needs to input two numbers 
     * that will cause an overflow. The user is asked for two positive integers.
     ****************************************/
    printf("n1 > n1 + n2 OR n2 > n1 + n2 \n");
    fflush(stdout);
    printf("What two positive numbers can make this possible: \n");
    fflush(stdout);
    
    /****************************************
     * Input section:
     * Using scanf, the program reads two integers from the user. 
     * If the input is valid, the program proceeds to check for overflow.
     ****************************************/
    if (scanf("%d", &num1) && scanf("%d", &num2)) {
        printf("You entered %d and %d\n", num1, num2);
        fflush(stdout);

        // Calculate the sum of the two input integers
        sum = num1 + num2;

        /****************************************
         * Overflow detection:
         * The program checks whether the addition of `num1` and `num2` causes 
         * an overflow using the `addIntOvf` function. 
         * - If no overflow is detected, the program prints "No overflow" and exits.
         * - If overflow is detected, it informs the user of the overflow.
         ****************************************/
        if (addIntOvf(sum, num1, num2) == 0) {
            printf("No overflow\n");
            fflush(stdout);
            exit(0);  // Exit if no overflow occurs
        } else if (addIntOvf(sum, num1, num2) == -1) {
            printf("You have an integer overflow\n");
            fflush(stdout);
        }

        /****************************************
         * Flag retrieval:
         * If the overflow is detected, and one of the numbers is positive, 
         * the program proceeds to read and print the contents of `flag.txt`.
         * - If the file is not found, it prints an error message.
         ****************************************/
        if (num1 > 0 || num2 > 0) {
            flag = fopen("flag.txt","r");
            if(flag == NULL){
                // If the file can't be opened, exit with an error message
                printf("flag not found: please run this on the server\n");
                fflush(stdout);
                exit(0);
            }

            // Buffer to hold the flag content
            char buf[60];

            // Read the flag content and display it
            fgets(buf, 59, flag);
            printf("YOUR FLAG IS: %s\n", buf);
            fflush(stdout);
            exit(0);  // Exit after printing the flag
        }
    }
    return 0;  // Return 0 on normal termination
}

/****************************************
 * Exploitation Explanation:
 *
 * The vulnerability in this code arises from the potential for integer overflow when 
 * adding two large positive numbers. The program uses signed 32-bit integers, 
 * meaning the maximum value a signed integer can hold is 2147483647 (2^31 - 1).
 *
 * If two large integers are added together and their sum exceeds this limit, 
 * the result wraps around into the negative range, causing an integer overflow. 
 * This overflow can be detected by the `addIntOvf()` function, which then allows 
 * access to the flag in `flag.txt`.
 *
 * **Steps to Exploit:**
 *
 * 1. Input two large positive integers that will cause the sum to exceed 2147483647 
 *    and wrap around into the negative range. For example:
 *    - Input 1: 2147483640
 *    - Input 2: 2147483640
 *
 * 2. The sum of these two numbers is 4294967280, which exceeds the maximum 
 *    representable value of a signed 32-bit integer. This causes an overflow, 
 *    and the sum becomes negative.
 *
 * 3. The program detects the overflow and grants access to the `flag.txt` file.
 *
 * **Possible Fix:**
 * To prevent this exploitation, stricter checks should be placed on the input 
 * values, or safer arithmetic operations should be used to avoid overflow.
 * Another option is to use a larger data type like `long long` to handle 
 * larger numbers.
 ****************************************/
