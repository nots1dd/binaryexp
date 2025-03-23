# README: Understanding and Exploiting Segmentation Faults in C

## **Building and Running**

> [!IMPORTANT]
> 
> This should be run in project's root directory!
> 

```bash 
make build-all 
make run_segfault
```

## Overview
This repository contains a simple C program that demonstrates various causes of **segmentation faults**. It also includes a **format string vulnerability**, which can be exploited under certain conditions.

## Code Explanation
The program defines a `flag` variable containing a hidden message:
```c
char flag[32] = "crypto{hidden_flag_here}";
```

### **Signal Handler for SIGSEGV**
A custom signal handler is defined for segmentation faults:
```c
void sigsegv_handler(int sig)
{
  printf("\n[!] Segmentation fault detected!\n");
  printf("%s\n", flag);
  fflush(stdout);
  exit(1);
}
```
When a segmentation fault occurs, instead of crashing, the program prints the `flag` and exits.

### **Vulnerabilities Present in the Code**
1. **Segmentation Faults (SIGSEGV)**
   - Accessing NULL or uninitialized pointers
   - Buffer overflows
   - Out-of-bounds array access
   - Stack overflow
   - Accessing freed memory

2. **Format String Vulnerability**
   The following code incorrectly uses `printf` with user-controlled input:
   ```c
   printf(input);
   ```
   Instead, it should be:
   ```c
   printf("%s", input);
   ```
   Allowing the user to control the format string can lead to **arbitrary memory read/write** exploits.

## **Compiling & Running**
```sh
gcc -o segfault_demo segfault.c -fno-stack-protector -z execstack\./segfault_demo
```

## **Exploitation Scenarios**
### **1. Triggering a Segmentation Fault**
To cause a segmentation fault, we can attempt a buffer overflow or out-of-bounds access.

### **2. Exploiting the Format String Vulnerability**
A well-crafted input such as:
```sh
./segfault_demo
Enter your name: %x %x %x %x
```
can leak memory contents, including the hidden flag.

## **Mitigation Strategies**
- Always use `printf("%s", input);` instead of `printf(input);`
- Enable compiler security flags: `-fstack-protector`, `-D_FORTIFY_SOURCE=2`
- Use **AddressSanitizer** during debugging: `-fsanitize=address`

## **Disclaimer**
This code is for **educational purposes only**. Do not use it for malicious purposes.

