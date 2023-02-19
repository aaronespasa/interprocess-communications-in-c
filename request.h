// typedef struct {
//     char *value1;
//     int value2;
//     double value3;
// } Request;

typedef struct
{
    int id;
    char text[100];
} Request;

//! Nota: se debe implementar un enum para el handler de los servicios

// typedef enum
// {
// init = 0,
// set_value = 1,
// get_value = 2,
// delete_key = 3,
// modify_value = 4,
// exist = 5,
// copy_key = 6
// } Services;