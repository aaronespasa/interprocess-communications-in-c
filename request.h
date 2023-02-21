/*
 * request.h file - structure of the request
 * Authors: 100451339 & 100451170
 */

// We'll use this enum to identify the operation to be performed
typedef enum
{
    init_enum = 0,
    set_value_enum = 1,
    get_value_enum = 2,
    delete_key_enum = 3,
    modify_value_enum = 4,
    exist_enum = 5,
    copy_key_enum = 6
} OPERATION;

// ! Request structure
// If operation is not using all the fields, they will be ignored
typedef struct
{
    int key1;            // 4 bytes
    int key2;            // 4 bytes
    char *value1;        // 255 chars + '\0'
    int value2;          // 4 bytes
    double value3;       // 8 bytes
    int *value2ptr;      // 1 word
    double *value3ptr;   // 1 word
    OPERATION operacion; // 0 = init, 1 = set_value, 2 = get_value, 3 = delete_key, 4 = modify_value, 5 = exist, 6 = copy_key
    char *clientQueue;   // 255 chars + '\0' (identifies the client queue for server to send the response)
} Request;
