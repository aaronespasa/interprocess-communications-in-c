// #include "claves.h"

#include "tuplas.h"

#include <pthread.h> /* For threads */
#include <stdio.h>   /* For printf() */
#include <stdlib.h>  /* For exit() */

#define NUM_THREADS 1

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

CLIENT *clnt = NULL;

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

int test_init(unsigned long thread_num, int print_value, CLIENT* clnt)
{
	// int result = init();

	int result = -1;

	enum clnt_stat ret = RPC_FAILED;

	ret = init_clnt(&result, clnt);

	if (ret != RPC_SUCCESS)
	{
		clnt_perror(clnt, "call failed");
		return -1;
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
	return result;
}

int test_set_value(int key, char value1[256], int value2, double value3, unsigned long thread_num, int print_value, CLIENT *clnt)
{
	int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	Value set_value_value;
	set_value_value.key = key;
	set_value_value.value1 = value1;
	set_value_value.value2 = value2;
	set_value_value.value3 = value3;

	ret = set_value_clnt(set_value_value, &result, clnt);

	if (ret != RPC_SUCCESS) {
		clnt_perror(clnt, "call failed");
		return ret;
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

	// Return the error code
	// printf("Result set: %d\n", result);
	return result;
}

int test_get_value(int key, unsigned long thread_num, int print_value, CLIENT* clnt)
{
	enum clnt_stat ret = RPC_FAILED;
	
	Value result_3;

	char value1[256] = "";

	result_3.key = -1;
	result_3.value1 = value1;
	result_3.value2 = 0;
	result_3.value3 = 0.0;

	ret = get_value_clnt(key, &result_3, clnt);

	if (ret != RPC_SUCCESS) {
		clnt_perror(clnt, "call failed");
		return result_3.key;
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

	// Return the error code
	// printf("Result get: %d\n", result_3.key);
	return result_3.key;
}

int test_modify_value(int key, char value1_modified[256], int value2, double value3, unsigned long thread_num, int print_value, CLIENT* clnt)
{
	int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	Value modify_value_value;
	modify_value_value.key = key;
	modify_value_value.value1 = value1_modified;
	modify_value_value.value2 = value2;
	modify_value_value.value3 = value3;

	ret = modify_value_clnt(modify_value_value, &result, clnt);

	if (ret != RPC_SUCCESS) {
		clnt_perror(clnt, "call failed");
		return result;
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

	// Return the error code
	// printf("Result modify: %d\n", result);
	return result;
}

// TODO: NOTA, error delete key
int test_delete_key(int key, unsigned long thread_num, int print_value, CLIENT* clnt)
{
	int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	ret = delete_key_clnt(key, &result, clnt);

	if (ret != RPC_SUCCESS) {
		clnt_perror(clnt, "call failed");
		return result;
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

	// Return the error code
	// printf("Result delete: %d\n", result);
	return result;
}

int test_exist_key(int key, unsigned long thread_num, int print_value, CLIENT* clnt)
{
	int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	ret = exist_clnt(key, &result, clnt);

	if (ret != RPC_SUCCESS) {
		clnt_perror(clnt, "call failed");
		return result;
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

	// Return the error code
	// printf("Result exist: %d\n", result);
	return result;
}

int test_copy_key(int key, int new_key, unsigned long thread_num, int print_value, CLIENT* clnt)
{
	int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	TwoKeys copy_key_keys;
	copy_key_keys.key1 = key;
	copy_key_keys.key2 = new_key;

	ret = copy_key_clnt(copy_key_keys, &result, clnt);

	if (ret != RPC_SUCCESS) {
		clnt_perror(clnt, "call failed");
		return result;
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

	// Return the error code
	// printf("Result copy: %d\n", result);
	return result;
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
 * @brief Test the get_value function before the set_value function
 * @param thread_num
 * @note This test MUST be run with only one thread (sequential execution),
 *       otherwise the execution is not guaranteed to be correct
 */ 
void test_get_before_set(thread_data th_data_copy, char value1[256])
{    
    if (check_one_thread(th_data_copy.thread_num, "test_get_before_set") == -1)
        return;

    int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	int init_ret = test_init(th_data_copy.thread_num, FALSE, th_data_copy.clnt);  // Initialize the server
    int get_ret = test_get_value(1, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Get the value of the key 1
    int set_ret = test_set_value(1, value1, 3, 3.1416, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Set the value of the key 1

    if (init_ret == 0 && get_ret == -1 && set_ret == 0)
        printf("✅ (test_get_before_set): Success: get_value returned -1 before set_value\n\n");
    else
        printf("❌ (test_get_before_set): Error: get_value returned %d before set_value returned %d\n\n", get_ret, set_ret);
}

/**
 * @brief Test the modify_value function before the set_value function
 * @param thread_num 
 * @param value1_modified 
 * @note This test MUST be run with only one thread (sequential execution),
 *       otherwise the execution is not guaranteed to be correct
 */
void test_modify_value_before_set(thread_data th_data_copy, char value1_modified[256])
{
    if (check_one_thread(th_data_copy.thread_num, "test_modify_value_before_set") == -1)
        return;

    int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	int init_ret = test_init(th_data_copy.thread_num, FALSE, th_data_copy.clnt);  // Initialize the server
	int modify_ret = test_modify_value(1, value1_modified, 6, 6.28, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Modify the value of the key 1
    int set_ret = test_set_value(1, value1_modified, 3, 3.1416, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Set the value of the key 1

    if (init_ret == 0 && modify_ret == -1 && set_ret == 0)
        printf("✅ (test_modify_value_before_set): Success: modify_value returned -1 before set_value\n\n");
    else
        printf("❌ (test_modify_value_before_set): Error: modify_value returned %d before set_value returned %d\n\n", modify_ret, set_ret);
}

/**
 * @brief Test the delete_key function before the set_value function
 * @param thread_num 
 * @note This test MUST be run with only one thread (sequential execution),
 *       otherwise the execution is not guaranteed to be correct
 */
void test_delete_key_before_set(thread_data th_data_copy, char value1[256])
{
    if (check_one_thread(th_data_copy.thread_num, "test_delete_key_before_set") == -1)
        return;

	int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	int init_ret = test_init(th_data_copy.thread_num, FALSE, th_data_copy.clnt);  // Initialize the server
    int delete_ret = test_delete_key(1, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Delete the key 1
    int set_ret = test_set_value(1, value1, 1234, 3.1416, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Set the value of the key 1

    if (init_ret == 0 && delete_ret == -1 && set_ret == 0)
        printf("✅ (test_delete_key_before_set): Success: delete_key returned -1 before set_value\n\n");
    else
        printf("❌ (test_delete_key_before_set): Error: delete_key returned %d before set_value returned %d\n\n", delete_ret, set_ret);
}

/**
 * @brief Test the copy_key function before the set_value function
 * @param thread_num 
 * @note This test MUST be run with only one thread (sequential execution),
 *       otherwise the execution is not guaranteed to be correct
 */
void test_copy_key_before_set(thread_data th_data_copy, char value1[256])
{
    if (check_one_thread(th_data_copy.thread_num, "test_copy_key_before_set") == -1)
        return;

    int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	int init_ret = test_init(th_data_copy.thread_num, FALSE, th_data_copy.clnt);  // Initialize the server
    int copy_ret = test_copy_key(1, 2, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Copy the key 1 into the key 2
    int set_ret = test_set_value(1, value1, 1234, 3.1416, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Set the value of the key 1

    if (init_ret == 0 && copy_ret == -1 && set_ret == 0)
        printf("✅ (test_copy_key_before_set): Success: copy_key returned -1 before set_value\n\n");
    else
        printf("❌ (test_copy_key_before_set): Error: copy_key returned %d before set_value returned %d\n\n", copy_ret, set_ret);
}

/**
 * @brief Test the exist function before the set_value function
 * @param thread_num 
 * @note This test MUST be run with only one thread (sequential execution),
 *       otherwise the execution is not guaranteed to be correct
 */
void test_exist_before_set(thread_data th_data_copy, char value1[256])
{
    if (check_one_thread(th_data_copy.thread_num, "test_exist_before_set") == -1)
        return;
	
    int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	int init_ret = test_init(th_data_copy.thread_num, FALSE, th_data_copy.clnt);  // Initialize the server
    int exist_ret = test_exist_key(4, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Check if the key 4 exists
    int set_ret = test_set_value(4, value1, 10, 24.24, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Set the value of the key 4

    if (init_ret == 0 && exist_ret == 0 && set_ret == 0) // if entry is not found in the list, exist returns 0
        printf("✅ (test_exist_before_set): Success: exist returned -1 before set_value\n\n");
    else
        printf("❌ (test_exist_before_set): Error: exist returned %d before set_value returned %d\n\n", exist_ret, set_ret);
}

/**
 * @brief Test the get_value function after the delete_key function
 * @param thread_num 
 * @note This test MUST be run with only one thread (sequential execution),
 *       otherwise the execution is not guaranteed to be correct
 */
void test_get_after_delete(thread_data th_data_copy, char value1[256])
{
    if (check_one_thread(th_data_copy.thread_num, "test_get_after_delete") == -1)
        return;

    int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	int init_ret = test_init(th_data_copy.thread_num, FALSE, th_data_copy.clnt);  // Initialize the server
    int set_ret = test_set_value(1, value1, 1234, 3.1416, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Set the value of the key 1
    int delete_ret = test_delete_key(1, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Delete the key 1
    int get_ret = test_get_value(1, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Get the value of the key 1

    if (init_ret == 0 && set_ret == 0 && delete_ret == 0 && get_ret == -1)
        printf("✅ (test_get_after_delete): Success: get_value returned -1 after delete_key\n\n");
    else
        printf("❌ (test_get_after_delete): Error: get_value returned %d after delete_key returned %d\n\n", get_ret, delete_ret);
}

/**
 * @brief Test the modify_value function after the delete_key function
 * @param thread_num 
 * @note This test MUST be run with only one thread (sequential execution),
 *       otherwise the execution is not guaranteed to be correct
 */
void test_modify_after_delete(thread_data th_data_copy, char value1[256], char value1_modified[256])
{
    if (check_one_thread(th_data_copy.thread_num, "test_modify_after_delete") == -1)
        return;

    int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	int init_ret = test_init(th_data_copy.thread_num, FALSE, th_data_copy.clnt);  // Initialize the server
    int set_ret = test_set_value(1, value1, 1234, 3.1416, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Set the value of the key 1
    int delete_ret = test_delete_key(1, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Delete the key 1
    int modify_ret = test_modify_value(1, value1_modified, 1234, 3.1416, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Modify the value of the key 1

    if (init_ret == 0 && set_ret == 0 && delete_ret == 0 && modify_ret == -1)
        printf("✅ (test_modify_after_delete): Success: modify_value returned -1 after delete_key\n\n");
    else
        printf("❌ (test_modify_after_delete): Error: modify_value returned %d after delete_key returned %d\n\n", modify_ret, delete_ret);
}

/**
 * @brief Test the copy_key function after the delete_key function
 * @param th_data_copy
 * @note This test MUST be run with only one thread (sequential execution),
 *       otherwise the execution is not guaranteed to be correct
 */
void test_copy_after_delete(thread_data th_data_copy, char value1[256])
{
    if (check_one_thread(th_data_copy.thread_num, "test_copy_after_delete") == -1)
        return;

    int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	int init_ret = test_init(th_data_copy.thread_num, FALSE, th_data_copy.clnt);  // Initialize the server
    int set_ret = test_set_value(1, value1, 1234, 3.1416, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Set the value of the key 1
    int delete_ret = test_delete_key(1, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Delete the key 1
    int copy_ret = test_copy_key(1, 2, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Copy the key 1 to key 2

    if (init_ret == 0 && set_ret == 0 && delete_ret == 0 && copy_ret == -1)
        printf("✅ (test_copy_after_delete): Success: copy_key returned -1 after delete_key\n\n");
    else
        printf("❌ (test_copy_after_delete): Error: copy_key returned %d after delete_key returned %d\n\n", copy_ret, delete_ret);
}

/**
 * @brief Test the exist function after the delete_key function
 * @param th_data_copy
 * @note This test MUST be run with only one thread (sequential execution),
 *       otherwise the execution is not guaranteed to be correct
 */
void test_exist_after_delete(thread_data th_data_copy, char value1[256])
{
    if (check_one_thread(th_data_copy.thread_num, "test_exist_after_delete") == -1)
        return;
    
    int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	int init_ret = test_init(th_data_copy.thread_num, FALSE, th_data_copy.clnt);  // Initialize the server
    int set_ret = test_set_value(1, value1, 1234, 3.1416, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Set the value of the key 1
    int delete_ret = test_delete_key(1, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Delete the key 1
    int exist_ret = test_exist_key(1, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Check if the key 1 exists

    if (init_ret == 0 && set_ret == 0 && delete_ret == 0 && exist_ret == 0) // if entry is not found in the list, exist returns 0
        printf("✅ (test_exist_after_delete): Success: exist returned -1 after delete_key\n\n");
    else
        printf("❌ (test_exist_after_delete): Error: exist returned %d after delete_key returned %d\n\n", exist_ret, delete_ret);
}

/**
 * @brief Test the copy_key function after the set_value function
 * @param th_data_copy
 * @note This test MUST be run with only one thread (sequential execution),
 *       otherwise the execution is not guaranteed to be correct
 */
void test_copy_after_set(thread_data th_data_copy, char value1[256])
{
    if (check_one_thread(th_data_copy.thread_num, "test_copy_after_set") == -1)
        return;

    int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	int init_ret = test_init(th_data_copy.thread_num, FALSE, th_data_copy.clnt);  // Initialize the server
    int set_ret = test_set_value(1, value1, 1234, 3.1416, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Set the value of the key 1
    int copy_ret = test_copy_key(1, 2, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Copy the key 1 to key 2

    if (init_ret == 0 && set_ret == 0 && copy_ret == 0)
        printf("✅ (test_copy_after_set): Success: copy_key returned 0 after set_value\n\n");
    else
        printf("❌ (test_copy_after_set): Error: copy_key returned %d after set_value returned %d\n\n", copy_ret, set_ret);
}

/**
 * @brief Test the get_value function after the copy_key function
 * @param th_data_copy
 * @note This test MUST be run with only one thread (sequential execution),
 *       otherwise the execution is not guaranteed to be correct
 */
void test_get_after_copy(thread_data th_data_copy, char value1[256])
{
    if (check_one_thread(th_data_copy.thread_num, "test_get_after_copy") == -1)
        return;

    int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	int init_ret = test_init(th_data_copy.thread_num, FALSE, th_data_copy.clnt);  // Initialize the server
    int set_ret = test_set_value(1, value1, 1234, 3.1416, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Set the value of the key 1
    int copy_ret = test_copy_key(1, 2, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Copy the key 1 to key 2
    int get_ret = test_get_value(2, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Get the value of the key 2

    if (init_ret == 0 && set_ret == 0 && copy_ret == 0 && get_ret == 0)
        printf("✅ (test_get_after_copy): Success: get_value returned 0 after copy_key\n\n");
    else
        printf("❌ (test_get_after_copy): Error: get_value returned %d after copy_key returned %d\n\n", get_ret, copy_ret);
}

/**
 * @brief Test the get_value function after the modify_value function
 * @param th_data_copy
 * @note This test MUST be run with only one thread (sequential execution),
 *       otherwise the execution is not guaranteed to be correct
 */
void test_copy_after_modified(thread_data th_data_copy, char value1[256], char value1_modified[256])
{
    if (check_one_thread(th_data_copy.thread_num, "test_copy_after_modified") == -1)
        return;

    int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	int init_ret = test_init(th_data_copy.thread_num, FALSE, th_data_copy.clnt);  // Initialize the server
    int set_ret = test_set_value(1, value1, 1234, 3.1416, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Set the value of the key 1
    int modify_ret = test_modify_value(1,value1_modified, 1234, 3.1416, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Modify the value of the key 1
    int copy_ret = test_copy_key(1, 2, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Copy the key 1 to key 2

    if (init_ret == 0 && set_ret == 0 && modify_ret == 0 && copy_ret == 0)
        printf("✅ (test_copy_after_modified): Success: copy_key returned 0 after modify_value\n\n");
    else
        printf("❌ (test_copy_after_modified): Error: copy_key returned %d after modify_value returned %d\n\n", copy_ret, modify_ret);
}

/**
 * @brief Test the modify_value function after the copy_key function
 * @param th_data_copy
 * @note This test MUST be run with only one thread (sequential execution),
 *       otherwise the execution is not guaranteed to be correct
 */
void test_modify_after_copy(thread_data th_data_copy, char value1[256], char value1_modified[256])
{
    if (check_one_thread(th_data_copy.thread_num, "test_modify_after_copy") == -1)
        return;
    
    int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	int init_ret = test_init(th_data_copy.thread_num, FALSE, th_data_copy.clnt);  // Initialize the server
    int set_ret = test_set_value(1, value1, 1234, 3.1416, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Set the value of the key 1
    int copy_ret = test_copy_key(1, 2, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Copy the key 1 to key 2
    int modify_ret = test_modify_value(2, value1_modified, 1234, 3.1416, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Modify the value of the key 2

    if (init_ret == 0 && set_ret == 0 && copy_ret == 0 && modify_ret == 0)
        printf("✅ (test_modify_after_copy): Success: modify_value returned 0 after copy_key\n\n");
    else
        printf("❌ (test_modify_after_copy): Error: modify_value returned %d after copy_key returned %d\n\n", modify_ret, copy_ret);
}

/**
 * @brief Test the copy_key function after the copy_key function
 * @param th_data_copy
 * @note This test MUST be run with only one thread (sequential execution),
 *       otherwise the execution is not guaranteed to be correct
 */
void test_copy_after_copy(thread_data th_data_copy, char value1[256])
{
    if (check_one_thread(th_data_copy.thread_num, "test_copy_after_copy") == -1)
        return;
    
    int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	int init_ret = test_init(th_data_copy.thread_num, FALSE, th_data_copy.clnt);  // Initialize the server
    int set_ret = test_set_value(1, value1, 1234, 3.1416, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Set the value of the key 1
    int copy_ret = test_copy_key(1, 2, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Copy the key 1 to key 2
    int copy_ret2 = test_copy_key(2, 3, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Copy the key 2 to key 3

    if (init_ret == 0 && set_ret == 0 && copy_ret == 0 && copy_ret2 == 0)
        printf("✅ (test_copy_after_copy): Success: copy_key returned 0 after copy_key\n\n");
    else
        printf("❌ (test_copy_after_copy): Error: copy_key returned %d after copy_key returned %d\n\n", copy_ret2, copy_ret);
}

/**
 * @brief Test the delete_key function after the copy_key function
 * @param th_data_copy
 * @note This test MUST be run with only one thread (sequential execution),
 *       otherwise the execution is not guaranteed to be correct
 */
void test_delete_after_copy(thread_data th_data_copy, char value1[256])
{
    if (check_one_thread(th_data_copy.thread_num, "test_delete_after_copy") == -1)
        return;
    
    int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	int init_ret = test_init(th_data_copy.thread_num, FALSE, th_data_copy.clnt);  // Initialize the server    
    int set_ret = test_set_value(1, value1, 1234, 3.1416, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Set the value of the key 1
    int copy_ret = test_copy_key(1, 2, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Copy the key 1 to key 2
    int delete_ret = test_delete_key(2, th_data_copy.thread_num, FALSE, th_data_copy.clnt); // Delete the key 2

    if (init_ret == 0 && set_ret == 0 && copy_ret == 0 && delete_ret == 0)
        printf("✅ (test_delete_after_copy): Success: delete_key returned 0 after copy_key\n\n");
    else
        printf("❌ (test_delete_after_copy): Error: delete_key returned %d after copy_key returned %d\n\n", delete_ret, copy_ret);
}


// ! ------------------------------ CONCURRENCY TESTS ------------------------------
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

	if (NUM_THREADS == 1) {

		// ! Error tests
		// ? Check error when get before set
        test_get_before_set(th_data_copy, value1);

		// ? Check error when modify before set
		test_modify_value_before_set(th_data_copy, value1_modified);

		// ? Check error when delete before set
        test_delete_key_before_set(th_data_copy, value1);

        // ? Check error when copy before set
        test_copy_key_before_set(th_data_copy, value1);

        // ? Check error when exists before set
        test_exist_before_set(th_data_copy, value1);

        // ? Check error when get after delete
        test_get_after_delete(th_data_copy, value1);

        // ? Check error when modify after delete
        test_modify_after_delete(th_data_copy, value1, value1_modified);

        // ? Check error when copy after delete
        test_copy_after_delete(th_data_copy, value1);

        // ? Check error when exists after delete
        test_exist_after_delete(th_data_copy, value1);

        // ! Success tests
        // ? Check error when copy to an already set key
        test_copy_after_set(th_data_copy, value1);

        // ? Check error when copy to an already copied key
        test_get_after_copy(th_data_copy, value1);

        // ? Check error when copy to an already modified key
        test_copy_after_modified(th_data_copy, value1, value1_modified);

        // ? Check error when modify after copy
        test_modify_after_copy(th_data_copy, value1, value1_modified);

        // ? Check error when copy after copy
        test_copy_after_copy(th_data_copy, value1);

        // ? Check error when delete after copy
        test_delete_after_copy(th_data_copy, value1);
	}

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
