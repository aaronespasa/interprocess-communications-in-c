#include "claves.h"

#include <stdbool.h>

typedef struct thread_data
{
    unsigned long thread_num;
    bool call_init;
} thread_data;

pthread_mutex_t protect_init_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t init_finished = PTHREAD_COND_INITIALIZER;
int busy = true;

thread_data th_data;

void test_init()
{
    int ret = init();

    if (ret == 0)
    {
        printf("(    init     ):     Se inicializó satisfactoriamente la lista\n");
    }
    else
    {
        printf("(    init     ):     Error (código %d) al inicializar la lista\n", ret);
    }
}

void test_set_value(unsigned long thread_num)
{
    int key = 0;
    char value1[256] = "Hola mundo";
    int value2 = 1234;
    double value3 = 3.1416;

    int ret = set_value(key, value1, value2, value3);

    if (ret == 0)
    {
        printf("(set_value - %lu):     Se guardó satisfactoriamente el valor (%s, %d, %f) en la clave %d\n", thread_num, value1, value2, value3, key);
    }
    else
    {
        printf("(set_value - %lu):     Error (código %d) al guardar el valor (%s, %d, %f) en la clave %d\n", thread_num, ret, value1, value2, value3, key);
    }
}

void test_set_value1(unsigned long thread_num)
{
    int key = 3;
    char value1[256] = "Hola mundo";
    int value2 = 1234;
    double value3 = 3.1416;

    int ret = set_value(key, value1, value2, value3);

    if (ret == 0)
    {
        printf("(set_value - %lu):     Se guardó satisfactoriamente el valor (%s, %d, %f) en la clave %d\n", thread_num, value1, value2, value3, key);
    }
    else
    {
        printf("(set_value - %lu):     Error (código %d) al guardar el valor (%s, %d, %f) en la clave %d\n", thread_num, ret, value1, value2, value3, key);
    }
}

void test_set_value2(unsigned long thread_num)
{
    int key = 2;
    char value1[256] = "Hola mundo";
    int value2 = 13114;
    double value3 = 3.23123;

    int ret = set_value(key, value1, value2, value3);

    if (ret == 0)
    {
        printf("(set_value - %lu):     Se guardó satisfactoriamente el valor (%s, %d, %f) en la clave %d\n", thread_num, value1, value2, value3, key);
    }
    else
    {
        printf("(set_value - %lu):     Error (código %d) al guardar el valor (%s, %d, %f) en la clave %d\n", thread_num, ret, value1, value2, value3, key);
    }
}

void test_get_value(int key, unsigned long thread_num)
{
    char value1[256] = "";
    int *value2 = malloc(sizeof(int));
    double *value3 = malloc(sizeof(double));

    int ret = get_value(key, value1, value2, value3);

    if (ret == 0)
    {
        printf("(get_value - %lu):     Se obtuvo satisfactoriamente el valor (%s, %d, %f) de la clave %d\n", thread_num, value1, *value2, *value3, key);
    }
    else
    {
        printf("(get_value - %lu):     Error (código %d) al obtener el valor de la clave %d\n", thread_num, ret, key);
    }

    free(value2);
    free(value3);
}

void test_modify_value(unsigned long thread_num)
{
    int key = 2;
    char value1[256] = "Hola jefe";
    int value2 = 4321;
    double value3 = 6.2832;

    int ret = modify_value(key, value1, value2, value3);

    if (ret == 0)
    {
        printf("(modify_value - %lu):  Se modificó satisfactoriamente la clave %d con el nuevo valor (%s, %d, %f)\n", thread_num, key, value1, value2, value3);
    }
    else
    {
        printf("(modify_value - %lu):  Error (código %d) al modificar la clave %d con el nuevo valor (%s, %d, %f)\n", thread_num, ret, key, value1, value2, value3);
    }
}

void test_delete_key(unsigned long thread_num)
{
    int key = 0;

    int ret = delete_key(key);

    if (ret == 0)
    {
        printf("(delete_key - %lu):    Se eliminó satisfactoriamente la clave %d\n", thread_num, key);
    }
    else
    {
        printf("(delete_key - %lu):    Error (código %d) al eliminar la clave %d\n", thread_num, ret, key);
    }
}

void test_exist(unsigned long thread_num)
{
    int key = 0;

    int ret = exist(key);

    if (ret == 1)
    {
        printf("(exist - %lu):         La clave %d si existe\n", thread_num, key);
    }
    else if (ret == 0)
    {
        printf("(exist - %lu):         La clave %d no existe\n", thread_num, key);
    }
    else
    {
        printf("(exist - %lu):         Error (código %d) al verificar si la clave %d existe\n", thread_num, ret, key);
    }
}

void test_copy_key(unsigned long thread_num)
{
    int key1 = 0;
    int key2 = 1;

    int ret = copy_key(key1, key2);

    if (ret == 0)
    {
        printf("(copy_key - %lu):      La clave %d se copio satisfactoriamente en la clave %d\n", thread_num, key1, key2);
    }
    else
    {
        printf("(copy_key - %lu):      Error (código %d) al copiar la clave %d en la clave %d\n", thread_num, ret, key1, key2);
    }
}

void test_copy_key1(unsigned long thread_num)
{
    int key1 = 0;
    int key2 = 2;

    int ret = copy_key(key1, key2);

    if (ret == 0)
    {
        printf("(copy_key - %lu):      La clave %d se copio satisfactoriamente en la clave %d\n", thread_num, key1, key2);
    }
    else
    {
        printf("(copy_key - %lu):      Error (código %d) al copiar la clave %d en la clave %d\n", thread_num, ret, key1, key2);
    }
}

// test_modify_value(thread_num);
// test_delete_key(thread_num);
// test_exist(thread_num);

// test_set_value(thread_num);
// test_exist(thread_num);
// test_copy_key(thread_num);

// test_copy_key1(thread_num);

void call_test(thread_data* th_data)
{
    thread_data th_data_copy;
    pthread_mutex_lock(&protect_init_mutex);

    // Make a copy of the data as it will be changed by the next thread
    th_data_copy = *th_data;

    // Initialize the linked list if this is the first thread
    if (th_data->call_init == true)
        test_init();
    
    busy = false;
    pthread_cond_signal(&init_finished);
    pthread_mutex_unlock(&protect_init_mutex);
    
    test_set_value(th_data_copy.thread_num);
    test_set_value1(th_data_copy.thread_num);
    test_set_value2(th_data_copy.thread_num);
    test_get_value(2, th_data_copy.thread_num);
}

int main()
{
    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        th_data.thread_num = i;
        th_data.call_init = i == 0 ? true : false; // only the first thread calls init

        pthread_create(&thread[i], &attr, (void *)call_test, (void *)&th_data);

        pthread_mutex_lock(&protect_init_mutex);
        while(busy) {
            pthread_cond_wait(&init_finished, &protect_init_mutex);
        }
        busy = true;
        pthread_mutex_unlock(&protect_init_mutex);
    }


    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(thread[i], NULL);
    }

    pthread_attr_destroy(&attr);

    return 0;
}
