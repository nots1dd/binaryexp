#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/****************************************************************************
 *
 * @IMPORTANT
 *
 * In order for this C code's compiled binary to work,
 *
 * You need to disable Address Space Layout Randomization (ASLR)
 *
 * And compile with flag -fno-stack-protector
 *
 * THIS IS NOT ADVISABLE
 *
 * @ONLY DO THE ABOVE IF YOU KNOW WHAT YOU ARE DOING
 *
 * YOU HAVE BEEN WARNED.
 *
 * To test it out without any issues,
 *
 * @QUESTION FLOW
 *
 * Just use gdb on `picker-IV` ELF provided in the same directory
 *
 * (gdb) info address win 
 * <some-address>
 * 
 * Exit GDB 
 * ./picker-IV 
 * <some-address>
 *
 * Get the flag
 *
 ****************************************************************************/

/*********************************************************
 * Function to handle segmentation faults (SIGSEGV)
 * This function will be triggered if the program attempts
 * to access invalid memory, causing a segmentation fault.
 * It prints a message and then exits with the signal code.
 *********************************************************/
void print_segf_message(){
  printf("Segfault triggered! Exiting.\n");
  sleep(15);
  exit(SIGSEGV); // Exit with SIGSEGV signal code
}

/*********************************************************
 * Function that prints a winning message and tries to
 * open and read from a file named "flag.txt".
 * If the file can be opened, it reads and prints its contents.
 *********************************************************/
int win() {
  FILE *fptr;
  char c;

  printf("You won!\n");
  // Open file
  fptr = fopen("flag.txt", "r");
  if (fptr == NULL) {
      printf("Cannot open file.\n");
      exit(0);
  }

  // Read contents from file
  c = fgetc(fptr);
  while (c != EOF) {
      printf ("%c", c);
      c = fgetc(fptr);
  }

  printf("\n");
  fclose(fptr);
}

/*************************************************************************
 *
 * Main function
 * 
 * 1. Sets up a signal handler for SIGSEGV (segmentation
 *    fault) using the print_segf_message function.
 * 2. Sets the stdout stream to unbuffered mode to ensure
 *    immediate output.
 * 3. Prompts the user to enter an address in hexadecimal
 *    format.
 * 4. Reads the address and casts it to a function pointer.
 * 5. Calls the function pointer, effectively jumping to
 *    the specified address.
 * 
 * Vulnerability:
 * - The main vulnerability is the ability to control the
 *   function pointer and make it point to arbitrary memory
 *   locations. This can lead to executing arbitrary code,
 *   including the `win` function if its address is provided.
 *
 *************************************************************************/
int main() {
  // Set up the signal handler for SIGSEGV
  signal(SIGSEGV, print_segf_message);
  // Set stdout to unbuffered mode
  setvbuf(stdout, NULL, _IONBF, 0); // _IONBF = Unbuffered

  unsigned int val;
  // Prompt the user to enter an address in hexadecimal
  printf("Enter the address in hex to jump to, excluding '0x': ");
  // Read the address input
  scanf("%x", &val);
  printf("You input 0x%x\n", val);

  // Cast the input address to a function pointer
  void (*foo)(void) = (void (*)())val;
  // Call the function pointer (jump to the address)
  foo();
}

/******************************
 *
 * @COMMANDS FOR GDB
 * 
 * 1. gdb ./picker-IV
 * 2. info functions
 *
 * Now find something like:
 * 0x..... win
 * COPY THAT
 *
 * EXIT GDB
 *
 * NOW EXECUTE BINARY:
 * ./picker-IV
 *
 * Paste the copied content to get flag
 * 
 *
 ******************************/ 
