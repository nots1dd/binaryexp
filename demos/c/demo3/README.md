# Shared Memory Poisoning Vulnerability

## **Building and Running**

> [!IMPORTANT]
> 
> This should be run in project's root directory!
> 

```bash 
make build-all 
make run_shm_victim # to run the victim 
```

In another terminal before the timeout send **THIS**:

```bash 
make run_shm_exploit
```

## Overview

This repository demonstrates a shared memory poisoning vulnerability, a type of security flaw that can occur when multiple processes use shared memory for inter-process communication without proper validation and safeguards.

The demonstration consists of two C programs:
1. A vulnerable victim program that creates shared memory and uses it to determine which function to call
2. An exploit program that modifies the shared memory to trigger execution of restricted code

## The Vulnerability Explained

### How It Works

The victim program:
1. Creates a segment of shared memory using System V IPC mechanisms (shmget, shmat)
2. Stores a function index (0) in this shared memory
3. Waits for 5 seconds (simulating normal program operation)
4. Reads the function index from shared memory
5. Uses the index to call a function from a function table
6. The function at index 0 is the benign `safe_function()`
7. The function at index 1 is the restricted `secret_function()` that reveals a flag

The vulnerability lies in the fact that any process with appropriate permissions can attach to this shared memory segment and modify the stored value during the 5-second window.

### The Attack Vector

The exploit program:
1. Attaches to the same shared memory segment using the same key
2. Modifies the stored function index from 0 to 1
3. This causes the victim to execute `secret_function()` instead of `safe_function()`
4. As a result, the attacker gains access to the secret flag that should never be exposed

## Why This Is Dangerous

This vulnerability demonstrates several important security issues:

1. **Trust Boundaries**: The victim program implicitly trusts that the data in shared memory hasn't been tampered with between when it was written and when it was read.

2. **Privilege Escalation**: An unprivileged process can potentially cause privileged operations to be executed if the victim program runs with higher privileges.

3. **Code Flow Hijacking**: The attacker effectively changes the execution path of the victim program.

4. **Weak Access Controls**: System V shared memory segments can be accessed by any process that knows the key and has sufficient permissions.

## Real-World Implications

In real-world applications, this type of vulnerability could lead to:

- Unauthorized access to sensitive information
- Privilege escalation
- Arbitrary code execution
- System compromise
- Data corruption or theft

## How To Fix The Vulnerability

There are several approaches to mitigate this type of vulnerability:

### 1. Use Proper Authentication and Authorization

Implement a mechanism to ensure that only authorized processes can access and modify the shared memory:

```c
// Use a more complex authentication mechanism than just a key
// For example, use a combination of key, process ID, and a secret token
```

### 2. Data Integrity Checks

Add checksums or signatures to verify that the data hasn't been tampered with:

```c
struct secure_data {
    int function_index;
    unsigned int checksum;
};

// Calculate checksum before reading the data
unsigned int calculate_checksum(int index, const char* secret) {
    // Implement a secure hashing algorithm
    return hash_function(index, secret);
}

// Verify checksum before using the data
if (data.checksum != calculate_checksum(data.function_index, "SECRET_KEY")) {
    // Data has been tampered with, take appropriate action
}
```

### 3. Principle of Least Privilege

Restrict the permissions on the shared memory segment:

```c
// Create shared memory with restricted permissions
int shmid = shmget(key, sizeof(int), 0600 | IPC_CREAT);
```

### 4. Input Validation

Implement stricter validation of values read from shared memory:

```c
// Whitelist specific valid values rather than just checking ranges
bool is_valid_index(int index) {
    return index == 0; // Only allow index 0
}

if (!is_valid_index(func_index)) {
    // Handle invalid index more securely
}
```

### 5. Use Alternative IPC Mechanisms

Consider using more secure IPC mechanisms that provide built-in authentication:

- Unix domain sockets with credential passing
- D-Bus with PolicyKit integration
- Message queues with authentication

### 6. Immutable Function Pointers

Design the system so that function pointers or indexes cannot be changed after initialization:

```c
// Set the function pointer once during initialization
// and use const to prevent modification
const void (*func)() = safe_function;
```

## Demonstration Setup

To demonstrate this vulnerability:

1. Compile both programs:
```bash
gcc -o victim victim.c
gcc -o exploit exploit.c
```

2. Run the victim program in one terminal:
```bash
./victim
```

3. While the victim is in its 5-second waiting period, run the exploit in another terminal:
```bash
./exploit
```

4. Observe that the victim executes the secret function instead of the safe function, revealing the flag.

## Conclusion

Shared memory mechanisms provide a powerful and efficient means of inter-process communication, but they must be used with careful consideration of security implications. Always assume that shared memory can be accessed by malicious processes, and implement appropriate safeguards to protect sensitive data and control program flow.
