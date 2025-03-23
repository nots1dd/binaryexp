#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define FLAG "crypto{th1s_i5_n3v3r_c4lL3d!}"

// Safe function that should be called normally
void safe_function() { printf("Safe function executed!\n"); }

// Secret function that should never be called
void secret_function()
{
  printf("EXPLOITED! Secret function executed!\n");
  printf("Captured flag: %s\n", FLAG);
}

int main()
{
  key_t key   = 1234;
  int   shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT);
  if (shmid == -1)
  {
    perror("shmget failed");
    return 1;
  }

  // Attach shared memory
  int* shm_ptr = (int*)shmat(shmid, NULL, 0);
  if (shm_ptr == (void*)-1)
  {
    perror("shmat failed");
    return 1;
  }

  // Store function index in shared memory (0 = safe_function)
  *shm_ptr = 0;
  printf("Function index stored in shared memory: %d\n", *shm_ptr);
  printf("Waiting for 5 seconds...\n");
  sleep(5); // Give attacker time to modify memory

  // Function table approach - choose function based on index
  void (*func_table[])() = {safe_function, secret_function};

  // Read function index from shared memory
  int func_index = *shm_ptr;
  printf("Function index read from shared memory: %d\n", func_index);

  // Validate index to prevent obvious exploitation (but not enough!)
  if (func_index < 0 || func_index > 1)
  {
    printf("Invalid function index! Defaulting to safe function.\n");
    func_index = 0;
  }

  // Call the function from the table
  printf("Calling function at index %d\n", func_index);
  func_table[func_index]();

  // Cleanup
  shmdt(shm_ptr);
  shmctl(shmid, IPC_RMID, NULL);
  return 0;
}
