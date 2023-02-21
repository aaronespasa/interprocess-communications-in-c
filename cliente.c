#include "claves.h"

void test_set_value() {
    int key = 0;
    char* value1= "Hola mundo";
    int value2 = 1234;
    double value3 = 3.1416;

    int ret = set_value(key, value1, value2, value3);

    if (ret == 0) {
        printf("(set_value): Se guardó el valor (%s, %d, %f) en la clave %d\n", value1, value2, value3, key);
    } else {
        printf("(set_value): Error (código %d) al guardar el valor (%s, %d, %f) en la clave %d\n", ret, value1, value2, value3, key);
    }
}

void test_get_value() {
    int key = 0;
    char* value1 = NULL;
    int* value2 = NULL;
    double* value3 = NULL;

    int ret = get_value(key, value1, value2, value3);

    if (ret == 0) {
        printf("(get_value): Se obtuvo el valor (%s, %d, %f) de la clave %d\n", value1, *value2, *value3, key);
    } else {
        printf("(get_value): Error (código %d) al obtener el valor de la clave %d\n", ret, key);
    }
}

void test_modify_value() {
    int key = 0;
    char* value1 = "Hola jefe";
    int value2 = 4321;
    double value3 = 6.2832;

    int ret = modify_value(key, value1, value2, value3);

    if (ret == 0) {
        printf("(modify_value): Se modificó la clave %d con el nuevo valor (%s, %d, %f)\n", key, value1, value2, value3);
    } else {
        printf("(modify_value): Error (código %d) al modificar la clave %d con el nuevo valor (%s, %d, %f)\n", ret, key, value1, value2, value3);
    }
}

void test_delete_key() {
    int key = 0;

    int ret = delete_key(key);

    if (ret == 0) {
        printf("(delete_key): Se eliminó la clave %d\n", key);
    } else {
        printf("(delete_key): Error (código %d) al eliminar la clave %d\n", ret, key);
    }
}

void test_exist() {
    int key = 0;

    int ret = exist(key);

    if (ret == 0) {
        printf("(exist): La clave %d existe\n", key);
    } else {
        printf("(exist): La clave %d no existe\n", key);
    }
}

void test_copy_key() {
    int key1 = 0;
    int key2 = 1;

    int ret = copy_key(key1, key2);

    if (ret == 0) {
        printf("(copy_key): La clave %d se copio en la clave %d\n", key1, key2);
    } else {
        printf("(copy_key): Error (código %d) al copiar la clave %d en la clave %d\n", ret, key1, key2);
    }
}

int main() {
    if (init() != 0) {
        printf("Error al conectase con el servidor\n");
        return -1;
    }

    test_set_value();
    test_get_value();
    test_modify_value();
    test_delete_key();

    test_set_value();
    test_exist();
    test_copy_key();
}