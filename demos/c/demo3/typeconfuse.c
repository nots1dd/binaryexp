#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
void normal_function(char *input);
void secret_function(void);
void perform_operation(int op_type, void *data);

// Enum for operation types
typedef enum {
    OP_STRING = 0,
    OP_CONFIG = 1,
    OP_DEBUG = 2  // New operation type for exploitation
} operation_type;

// Configuration structure
typedef struct {
    int config_id;
    void (*callback)(void);  // Function pointer for callbacks
    int parameter;
} config_t;

// Union that can either hold a string or a configuration
typedef union {
    char string_data[16];
    config_t config_data;
} data_container;

// Global containers for operations
data_container containers[5];

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <operation_type> <data>\n", argv[0]);
        printf("  operation_type: 0 (string), 1 (config), or 2 (debug)\n");
        return 1;
    }

    // Print address of secret function for reference
    printf("Address of secret_function: %p\n", secret_function);

    int op_type = atoi(argv[1]);
    char *input = argv[2];
    
    // Initialize containers
    for (int i = 0; i < 5; i++) {
        memset(&containers[i], 0, sizeof(data_container));
    }

    // Process the input based on operation type
    perform_operation(op_type, input);
    
    // Execute operations
    for (int i = 0; i < 5; i++) {
        if (op_type == OP_STRING) {
            if (strlen(containers[i].string_data) > 0) {
                printf("Container %d: Processing string: %s\n", i, containers[i].string_data);
                normal_function(containers[i].string_data);
            }
        } else if (op_type == OP_CONFIG || op_type == OP_DEBUG) {
            if (containers[i].config_data.config_id != 0) {
                printf("Container %d: Processing config ID: %d\n", i, containers[i].config_data.config_id);
                
                // Execute callback if set
                if (containers[i].config_data.callback != NULL) {
                    printf("Calling function at address: %p\n", containers[i].config_data.callback);
                    containers[i].config_data.callback();
                }
            }
        }
    }

    return 0;
}

// Function to perform operations based on type
void perform_operation(int op_type, void *data) {
    if (op_type == OP_STRING) {
        // Copy string to first container
        strncpy(containers[0].string_data, (char *)data, 15);
        containers[0].string_data[15] = '\0';
        
        // The vulnerability: Improper parsing of special sequence
        char *input = (char *)data;
        if (strstr(input, "CONTAINER:") == input) {
            int container_idx = input[10] - '0';
            if (container_idx >= 0 && container_idx < 5) {
                strncpy(containers[container_idx].string_data, input + 12, 15);
                containers[container_idx].string_data[15] = '\0';
            }
        }
    } else if (op_type == OP_CONFIG) {
        // Parse config from data string (format: "id:param")
        char *input = (char *)data;
        int id, param;
        if (sscanf(input, "%d:%d", &id, &param) == 2) {
            containers[0].config_data.config_id = id;
            containers[0].config_data.parameter = param;
            containers[0].config_data.callback = NULL;
        }
    } else if (op_type == OP_DEBUG) {
        // Special debug mode for exploitation 
        // Format: "container_idx:config_id:use_secret"
        char *input = (char *)data;
        int container_idx, config_id, use_secret;
        if (sscanf(input, "%d:%d:%d", &container_idx, &config_id, &use_secret) == 3) {
            if (container_idx >= 0 && container_idx < 5) {
                containers[container_idx].config_data.config_id = config_id;
                containers[container_idx].config_data.parameter = 1337;
                
                // Set the callback to the secret function if requested
                if (use_secret) {
                    containers[container_idx].config_data.callback = secret_function;
                    printf("Set container %d callback to secret_function (%p)\n", 
                           container_idx, secret_function);
                }
            }
        }
    }
}

// Normal function that processes strings
void normal_function(char *input) {
    printf("Normal function executed with: %s\n", input);
}

// Secret function that should not be accessible
void secret_function(void) {
    printf("\n*** SECRET FUNCTION EXECUTED ***\n");
    printf("This represents successful exploitation through type confusion!\n");
    printf("In a real attack, this could be shellcode or privileged operations.\n\n");
}
