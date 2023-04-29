#include "tuplas.h"
#include <pthread.h> /* For threads */
#define NUM_THREADS 2

typedef struct thread_data
{
	unsigned long thread_num;
	int call_init;
	CLIENT* clnt;
} thread_data;

pthread_mutex_t protect_init_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t init_finished = PTHREAD_COND_INITIALIZER;
int busy = TRUE;

thread_data th_data;

void test_init(unsigned long thread_num, int print_value, CLIENT* clnt)
{
	int result = -1;
	enum clnt_stat ret = RPC_FAILED;
	
	ret = init(&result, clnt);

	if (ret != RPC_SUCCESS) {
		clnt_perror(clnt, "call failed");
		return;
	}

	if (print_value == TRUE)
	{
		if (result == 0)
		{
			printf("(init      - %lu):     Se inicializó satisfactoriamente la lista\n", thread_num);
		}
		else
		{
			printf("(init      - %lu):     Error (código %d) al inicializar la lista\n", thread_num, result);
		}
	}

	// printf("✅ (init): Success: la lista se limpió correctamente para realizar las pruebas\n\n");
}

void test_set_value(int key, char value1[256], int value2, double value3, unsigned long thread_num, int print_value, CLIENT *clnt)
{
	int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	Value set_value_value;
	set_value_value.key = key;
	set_value_value.value1 = value1;
	set_value_value.value2 = value2;
	set_value_value.value3 = value3;

	ret = set_value(set_value_value, &result, clnt);

	if (ret != RPC_SUCCESS) {
		clnt_perror(clnt, "call failed");
		return;
	}

	if (print_value == TRUE)
	{
		if (result == 0)
		{
			printf("(set_value - %lu):     Se guardó satisfactoriamente el valor (%s, %d, %f) en la clave %d\n",
				   thread_num, set_value_value.value1, set_value_value.value2, set_value_value.value3, set_value_value.key);
		}
		else
		{
			printf("(set_value - %lu):     Error (código: %d) al guardar el valor (%s, %d, %f) en la clave %d\n",
				   thread_num, result, set_value_value.value1, set_value_value.value2, set_value_value.value3, set_value_value.key);
		}
	}
}

void test_get_value(int key, unsigned long thread_num, int print_value, CLIENT* clnt)
{
	enum clnt_stat ret = RPC_FAILED;
	
	Value result_3;

	char value1[256] = "";

	result_3.key = -1;
	result_3.value1 = value1;
	result_3.value2 = 0;
	result_3.value3 = 0.0;

	ret = get_value(key, &result_3, clnt);

	if (ret != RPC_SUCCESS) {
		clnt_perror(clnt, "call failed");
		return;
	}

	if (print_value == TRUE)
	{
		if (result_3.key == 0)
		{
			printf("(get_value - %lu):     Se obtuvo satisfactoriamente el valor (%s, %d, %f) de la clave %d\n", 
						thread_num, result_3.value1, result_3.value2, result_3.value3, key);
		}
		else
		{
			printf("(get_value - %lu):     Error (código %d) al obtener el valor de la clave %d\n",
				thread_num, result_3.key, key);
		}
	}
}

void test_modify_value(int key, char value1_modified[256], int value2, double value3, unsigned long thread_num, int print_value, CLIENT* clnt)
{
	int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	Value modify_value_value;
	modify_value_value.key = key;
	modify_value_value.value1 = value1_modified;
	modify_value_value.value2 = value2;
	modify_value_value.value3 = value3;

	ret = modify_value(modify_value_value, &result, clnt);

	if (ret != RPC_SUCCESS) {
		clnt_perror(clnt, "call failed");
		return;
	}

	if (print_value == TRUE)
	{
		if (result == 0)
		{
			printf("(modify_value - %lu):  Se modificó satisfactoriamente el valor (%s, %d, %f) de la clave %d\n",
				   thread_num, modify_value_value.value1, modify_value_value.value2, modify_value_value.value3, modify_value_value.key);
		}
		else
		{
			printf("(modify_value - %lu):  Error (código %d) al modificar el valor (%s, %d, %f) de la clave %d\n",
				   thread_num, result, modify_value_value.value1, modify_value_value.value2, modify_value_value.value3, modify_value_value.key);
		}
	}
}

void test_delete_key(int key, unsigned long thread_num, int print_value, CLIENT* clnt)
{
	int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	ret = delete_key(key, &result, clnt);

	if (ret != RPC_SUCCESS) {
		clnt_perror(clnt, "call failed");
		return;
	}

	if (print_value == TRUE)
	{
		if (result == 0)
		{
			printf("(delete_key - %lu):    Se eliminó satisfactoriamente la clave %d\n", thread_num, key);
		}
		else
		{
			printf("(delete_key - %lu):    Error (código %d) al eliminar la clave %d\n", thread_num, result, key);
		}
	}
}

void test_exist_key(int key, unsigned long thread_num, int print_value, CLIENT* clnt)
{
	int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	ret = exist(key, &result, clnt);

	if (ret != RPC_SUCCESS) {
		clnt_perror(clnt, "call failed");
		return;
	}

	if (print_value == TRUE)
	{
		if (result == 1)
		{
			printf("(exist - %lu):         La clave %d si existe\n", thread_num, key);
		}
		else if (result == 0)
		{
			printf("(exist - %lu):         La clave %d no existe\n", thread_num, key);
		}
		else
		{
			printf("(exist - %lu):         Error (código %d) al verificar si la clave %d existe\n", thread_num, result, key);
		}
	}
}

void test_copy_key(int key, int new_key, unsigned long thread_num, int print_value, CLIENT* clnt)
{
	int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	TwoKeys copy_key_keys;
	copy_key_keys.key1 = key;
	copy_key_keys.key2 = new_key;

	ret = copy_key(copy_key_keys, &result, clnt);

	if (ret != RPC_SUCCESS) {
		clnt_perror(clnt, "call failed");
		return;
	}

	if (print_value == TRUE)
	{
		if (result == 0)
		{
			printf("(copy_key - %lu):      Se copió satisfactoriamente la clave %d en la clave %d\n", thread_num, key, new_key);
		}
		else
		{
			printf("(copy_key - %lu):      Error (código %d) al copiar la clave %d en la clave %d\n", thread_num, result, key, new_key);
		}
	}
}

int check_one_thread(unsigned long thread_num, char *test_name)
{
	if (NUM_THREADS == 1)
	{
		return 0;
	}
	else
	{
		printf("❌ (%s - %lu): Error: this test must be run with only one thread\n", test_name, thread_num);
		return -1;
	}
}

/**
 * @brief This is a test to check the concurrency of the functions, it must be verified by hand!
 * 
 * @param thread_num 
 */
void test_concurrency(thread_data th_data_copy, char value1[256], char value1_modified[256])
{
    // check there are at least 2 threads
    if (NUM_THREADS < 2) {
        printf("❌ (test_concurrency): Error: test_concurrency requires at least 2 threads\n\n");
        return;
    }

	test_set_value(1, value1, 3, 3.1416, th_data_copy.thread_num, TRUE, th_data_copy.clnt);
	test_get_value(1, th_data_copy.thread_num, TRUE, th_data_copy.clnt);
	test_modify_value(1, value1_modified, 6, 6.28, th_data_copy.thread_num, TRUE, th_data_copy.clnt);
	test_delete_key(1, th_data_copy.thread_num, TRUE, th_data_copy.clnt);

	test_set_value(4, value1, 10, 24.24, th_data_copy.thread_num, TRUE, th_data_copy.clnt);
	test_exist_key(4, th_data_copy.thread_num, TRUE, th_data_copy.clnt);
	test_copy_key(4, 5, th_data_copy.thread_num, TRUE, th_data_copy.clnt);
}

void tests(thread_data *th_data) {
	thread_data th_data_copy;
	pthread_mutex_lock(&protect_init_mutex);

	// Make a copy of the data as it will be changed by the next thread
	th_data_copy = *th_data;

	// Initialize the linked list if this is the first thread
	if (th_data->call_init == TRUE)
		test_init(th_data_copy.thread_num, TRUE, th_data_copy.clnt);

	busy = FALSE;
	pthread_cond_signal(&init_finished);
	pthread_mutex_unlock(&protect_init_mutex);

	char value1[256] = "Hola mundo";
	char value1_modified[256] = "Hola mundo, he sido modificado";

	if (NUM_THREADS > 1)
		test_concurrency(th_data_copy, value1, value1_modified);
}


void main_concurrent(CLIENT *clnt) {
	// If needed to test sequential execution -> Modify NUM_OF_THREADS from claves.h to 1
	pthread_t thread[NUM_THREADS];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for (int i = 0; i < NUM_THREADS; i++)
	{
		th_data.thread_num = i;
		th_data.call_init = i == 0 ? TRUE : FALSE; // only the first thread calls init
		th_data.clnt = clnt;

		pthread_create(&thread[i], &attr, (void *)tests, (void *)&th_data);

		pthread_mutex_lock(&protect_init_mutex);
		while (busy)
		{
			pthread_cond_wait(&init_finished, &protect_init_mutex);
		}
		busy = TRUE;
		pthread_mutex_unlock(&protect_init_mutex);
	}

	for (int i = 0; i < NUM_THREADS; i++)
		pthread_join(thread[i], NULL);

	pthread_attr_destroy(&attr);
}

void
tuple_service_1(char *host)
{
	CLIENT *clnt;
#ifndef	DEBUG
		clnt = clnt_create(host, TUPLE_SERVICE, TUPLE_SERVICE_V1, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	main_concurrent(clnt);
#ifndef	DEBUG
		clnt_destroy(clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	tuple_service_1 (host);
exit (0);
}
