#include "claves.h"

void test_init()
{
    int ret = init();

    if (ret == 0)
    {
        printf("(init):          Se inicializó satisfactoriamente la lista\n");
    }
    else
    {
        printf("(init):          Error (código %d) al inicializar la lista\n", ret);
    }
}

void test_set_value()
{
    int key = 0;
    char value1[256] = "Hola mundo";
    int value2 = 1234;
    double value3 = 3.1416;

    int ret = set_value(key, value1, value2, value3);

    if (ret == 0)
    {
        printf("(set_value):     Se guardó satisfactoriamente el valor (%s, %d, %f) en la clave %d\n", value1, value2, value3, key);
    }
    else
    {
        printf("(set_value):     Error (código %d) al guardar el valor (%s, %d, %f) en la clave %d\n", ret, value1, value2, value3, key);
    }
}

void test_set_value1()
{
    int key = 3;
    char value1[256] = "Hola mundo";
    int value2 = 1234;
    double value3 = 3.1416;

    int ret = set_value(key, value1, value2, value3);

    if (ret == 0)
    {
        printf("(set_value):     Se guardó satisfactoriamente el valor (%s, %d, %f) en la clave %d\n", value1, value2, value3, key);
    }
    else
    {
        printf("(set_value):     Error (código %d) al guardar el valor (%s, %d, %f) en la clave %d\n", ret, value1, value2, value3, key);
    }
}

void test_set_value2()
{
    int key = 2;
    char value1[256] = "Hola mundo";
    int value2 = 13114;
    double value3 = 3.23123;

    int ret = set_value(key, value1, value2, value3);

    if (ret == 0)
    {
        printf("(set_value):     Se guardó satisfactoriamente el valor (%s, %d, %f) en la clave %d\n", value1, value2, value3, key);
    }
    else
    {
        printf("(set_value):     Error (código %d) al guardar el valor (%s, %d, %f) en la clave %d\n", ret, value1, value2, value3, key);
    }
}

void test_get_value()
{
    int key = 0;
    char value1[256] = "";
    int *value2 = malloc(sizeof(int));
    double *value3 = malloc(sizeof(double));

    int ret = get_value(key, value1, value2, value3);

    if (ret == 0)
    {
        printf("(get_value):     Se obtuvo satisfactoriamente el valor (%s, %d, %f) de la clave %d\n", value1, *value2, *value3, key);
    }
    else
    {
        printf("(get_value):     Error (código %d) al obtener el valor de la clave %d\n", ret, key);
    }
}

void test_modify_value()
{
    int key = 2;
    char value1[256] = "Hola jefe";
    int value2 = 4321;
    double value3 = 6.2832;

    int ret = modify_value(key, value1, value2, value3);

    if (ret == 0)
    {
        printf("(modify_value):  Se modificó satisfactoriamente la clave %d con el nuevo valor (%s, %d, %f)\n", key, value1, value2, value3);
    }
    else
    {
        printf("(modify_value):  Error (código %d) al modificar la clave %d con el nuevo valor (%s, %d, %f)\n", ret, key, value1, value2, value3);
    }
}

void test_delete_key()
{
    int key = 0;

    int ret = delete_key(key);

    if (ret == 0)
    {
        printf("(delete_key):    Se eliminó satisfactoriamente la clave %d\n", key);
    }
    else
    {
        printf("(delete_key):    Error (código %d) al eliminar la clave %d\n", ret, key);
    }
}

void test_exist()
{
    int key = 0;

    int ret = exist(key);

    if (ret == 1)
    {
        printf("(exist):         La clave %d si existe\n", key);
    }
    else if (ret == 0)
    {
        printf("(exist):         La clave %d no existe\n", key);
    }
    else
    {
        printf("(exist):         Error (código %d) al verificar si la clave %d existe\n", ret, key);
    }
}

void test_copy_key()
{
    int key1 = 0;
    int key2 = 1;

    int ret = copy_key(key1, key2);

    if (ret == 0)
    {
        printf("(copy_key):      La clave %d se copio satisfactoriamente en la clave %d\n", key1, key2);
    }
    else
    {
        printf("(copy_key):      Error (código %d) al copiar la clave %d en la clave %d\n", ret, key1, key2);
    }
}

void test_copy_key1()
{
    int key1 = 0;
    int key2 = 2;

    int ret = copy_key(key1, key2);

    if (ret == 0)
    {
        printf("(copy_key):      La clave %d se copio satisfactoriamente en la clave %d\n", key1, key2);
    }
    else
    {
        printf("(copy_key):      Error (código %d) al copiar la clave %d en la clave %d\n", ret, key1, key2);
    }
}

int main()
{
    test_init();

    test_set_value();
    test_set_value1();
    test_set_value2();
    test_get_value();
    // test_modify_value();
    // test_delete_key();
    // test_exist();

    // test_set_value();
    // test_exist();
    // test_copy_key();

    // test_copy_key1();

    return 0;
}