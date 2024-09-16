#include <stdio.h>
#include <stdlib.h>

/**************************************************************************************
 *
 * @INTEGER OVERFLOW
 * 
 * $SOLUTION
 *
 * Upon checking the source code (or decompiled code)
 *
 * We find that input char is of 16 length only
 * 
 *
 * @HOW MANY CHARS TO OVERFLOW:
 *
 *   In your case, you have 24 characters followed by 'A'. The overflow affects num starting from the 17th byte, and potentially changes its value.
 *   Why 24 characters? This length suggests that after 16 characters fill the input buffer, the subsequent 8 characters start affecting num. 
 *   The exact number of bytes affecting num can vary based on system architecture and compiler padding.
 *
 *   TLDR :: Keep checking the val of num with payloads with greater than 16 length and based on deduction and logic,
 *   you will come to understand how to manipulate its value to desired value
 *
 * So we can overflow it and the overflown chars go to the var num
 * 
 * We can overflow it by the following string format:
 *
 * <random>{desired num val}
 *
 * We want num to be 65 for it to print the flag
 *
 * So converting 65 to ascii, it is A (int will convert the string to its ascii)
 *
 * Therefore, the payload will be:
 *
 * <random>A
 *
 * Give it to the executable, and thats it! You will get the contents of flag.txt
 *
 **************************************************************************************/

int main(){
  FILE *fptr;
  char c;

  char input[16];
  int num = 64;
  
  printf("Enter a string: ");
  fflush(stdout);
  gets(input);
  printf("\n");
  
  printf("num is %d\n", num);
  fflush(stdout);
  
  if( num == 65 ){
    printf("You win!\n");
    fflush(stdout);
    // Open file
    fptr = fopen("flag.txt", "r");
    if (fptr == NULL)
    {
        printf("Cannot open file.\n");
        fflush(stdout);
        exit(0);
    }

    // Read contents from file
    c = fgetc(fptr);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(fptr);
    }
    fflush(stdout);

    printf("\n");
    fflush(stdout);
    fclose(fptr);
    exit(0);
  }
  
  printf("Bye!\n");
  fflush(stdout);
}
