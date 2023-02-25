/*
 * request.h file - structure of the request
 * Authors: 100451339 & 100451170
 */

// We'll use this enum to identify the operation to be performed
typedef enum
{
    set_value_op = 0,
    get_value_op = 1,
    delete_key_op = 2,
    modify_value_op = 3,
    exist_op = 4,
    copy_key_op = 5
} OPERATION;

// ! Request structure
// If operation is not using all the fields, they will be ignored
typedef struct
{
    int key1;             // 4 bytes
    int key2;             // 4 bytes
    char value1[256];     // 255 chars + '\0', it will be tested if strlen(value1) > 256
    int value2;           // 4 bytes
    double value3;        // 8 bytes
    OPERATION operacion;  // 0 = init, 1 = set_value, 2 = get_value, 3 = delete_key, 4 = modify_value, 5 = exist, 6 = copy_key
    char clientPID[32]; // Identifies the client queue for server to send the response
} Request;
