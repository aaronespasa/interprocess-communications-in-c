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

void test_init(unsigned long thread_num)
{
    int ret = init();

    if (ret == 0)
    {
        printf("(init      - %lu):     Se inicializó satisfactoriamente la lista\n", thread_num);
    }
    else
    {
        printf("(init      - %lu):     Error (código %d) al inicializar la lista\n", thread_num, ret);
    }
}

void test_set_value(int key, char value1[256], int value2, double value3, unsigned long thread_num)
{
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

    // initialize values
    *value2 = 0;
    *value3 = 0.0;

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

void test_modify_value(int key, char value1_modified[256], int value2, double value3, unsigned long thread_num)
{
    int ret = modify_value(key, value1_modified, value2, value3);

    if (ret == 0)
    {
        printf("(modify_value - %lu):  Se modificó satisfactoriamente la clave %d con el nuevo valor (%s, %d, %f)\n", thread_num, key, value1_modified, value2, value3);
    }
    else
    {
        printf("(modify_value - %lu):  Error (código %d) al modificar la clave %d con el nuevo valor (%s, %d, %f)\n", thread_num, ret, key, value1_modified, value2, value3);
    }
}

void test_delete_key(int key, unsigned long thread_num)
{
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

void test_exist(int key, unsigned long thread_num)
{
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

void test_copy_key(int key1, int key2, unsigned long thread_num)
{
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

void call_test(thread_data* th_data)
{
    thread_data th_data_copy;
    pthread_mutex_lock(&protect_init_mutex);

    // Make a copy of the data as it will be changed by the next thread
    th_data_copy = *th_data;

    // Initialize the linked list if this is the first thread
    if (th_data->call_init == true)
        test_init(th_data_copy.thread_num);
    
    busy = false;
    pthread_cond_signal(&init_finished);
    pthread_mutex_unlock(&protect_init_mutex);

    char value1[256] = "Hola mundo";
    char value1_modified[256] = "Hola jefe";

    test_set_value(0, value1, 1234, 3.1416, th_data_copy.thread_num);
    test_set_value(3, value1, 1234, 3.1416, th_data_copy.thread_num);
    test_set_value(2, value1, 13114, 3.23123, th_data_copy.thread_num);
    test_get_value(2, th_data_copy.thread_num);

    test_modify_value(2, value1_modified, 4321, 6.2832, th_data_copy.thread_num);
    test_delete_key(0, th_data_copy.thread_num);
    test_exist(0, th_data_copy.thread_num);
    test_set_value(0, value1, 1234, 3.1416, th_data_copy.thread_num);
    test_exist(0, th_data_copy.thread_num);
    test_copy_key(0, 1, th_data_copy.thread_num);
    test_copy_key(0, 2, th_data_copy.thread_num);
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
        pthread_join(thread[i], NULL);

    pthread_attr_destroy(&attr);

    return 0;
}
