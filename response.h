/*
 * request.h file - structure of the request
 * Authors: 100451339 & 100451170
 */

typedef struct
{
    int error_code;
    char value1[256]; // 255 chars + '\0', it will be tested if strlen(value1) > 256
    int value2;       // 4 bytes
    double value3;    // 8 bytes
} Response;
