/*
 * request.h file - structure of the request
 * Authors: 100451339 & 100451170
 */

// We'll use this enum to identify the operation to be performed
typedef enum
{
    init = 0,
    set_value = 1,
    get_value = 2,
    delete_key = 3,
    modify_value = 4,
    exist = 5,
    copy_key = 6
} OPERATION;

typedef struct
{
    int clave;           // 4 bytes
    char valor1[256];    // 255 chars + '\0'
    int valor2;          // 4 bytes
    double valor3;       // 8 bytes
    OPERATION operacion; // 0 = init, 1 = set_value, 2 = get_value, 3 = delete_key, 4 = modify_value, 5 = exist, 6 = copy_key
    int end;             // (default) 0 = false, 1 = true (used to end the loop, not necessary)
} Request;
