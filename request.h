/*
 * request.h file - structure of the request
 * Authors: 100451339 & 100451170
 */

// We'll use this enum to identify the operation to be performed
typedef enum
{
    init_op = 0,
    set_value_op = 1,
    get_value_op = 2,
    delete_key_op = 3,
    modify_value_op = 4,
    exist_op = 5,
    copy_key_op = 6,
} OPERATION;

char* OPERATION_NAMES[7] = {"init", "set_value", "get_value", "delete_key", "modify_value", "exist", "copy_key"};

// Array to store the number of parameters that each operation needs
int OPERATION_PARAMS[7] = {0, 4, 1, 4, 1, 1, 2};

// // ! Request structure
// // If operation is not using all the fields, they will be ignored
// typedef struct
// {
//     int key1;            // 4 bytes
//     int key2;            // 4 bytes
//     char value1[256];    // 255 chars + '\0', it will be tested if strlen(value1) > 256
//     int value2;          // 4 bytes
//     double value3;       // 8 bytes
//     OPERATION operacion; // 0 = init, 1 = set_value, 2 = get_value, 3 = delete_key, 4 = modify_value, 5 = exist, 6 = copy_key
//     char clientPID[32];  // Identifies the client queue for server to send the response
// } Request;

typedef struct
{
    int socket; // Socket descriptor
    int operation_code; // Operation code
} Request;

typedef struct
{
    int key1;            // 4 bytes
    int key2;            // 4 bytes
    char value1[256];    // 255 chars + '\0', it will be tested if strlen(value1) > 256
    int value2;          // 4 bytes
    double value3;       // 8 bytes
} Parameters;