/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "tuplas.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

enum clnt_stat 
init_service_1(status *clnt_res, CLIENT *clnt)
{
	 return (clnt_call (clnt, init_service, (xdrproc_t) xdr_void, (caddr_t) NULL,
		(xdrproc_t) xdr_status, (caddr_t) clnt_res,
		TIMEOUT));

}

enum clnt_stat 
set_tuple_1(int key, char *value1, int value2, double value3, status *clnt_res,  CLIENT *clnt)
{
	set_tuple_1_argument arg;
	arg.key = key;
	arg.value1 = value1;
	arg.value2 = value2;
	arg.value3 = value3;
	return (clnt_call (clnt, set_tuple, (xdrproc_t) xdr_set_tuple_1_argument, (caddr_t) &arg,
		(xdrproc_t) xdr_status, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
get_tuple_1(int key, tuple *clnt_res,  CLIENT *clnt)
{
	return (clnt_call(clnt, get_tuple,
		(xdrproc_t) xdr_int, (caddr_t) &key,
		(xdrproc_t) xdr_tuple, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
modify_tuple_1(int key, char *value1, int value2, double value3, status *clnt_res,  CLIENT *clnt)
{
	modify_tuple_1_argument arg;
	arg.key = key;
	arg.value1 = value1;
	arg.value2 = value2;
	arg.value3 = value3;
	return (clnt_call (clnt, modify_tuple, (xdrproc_t) xdr_modify_tuple_1_argument, (caddr_t) &arg,
		(xdrproc_t) xdr_status, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
delete_tuple_1(int key, status *clnt_res,  CLIENT *clnt)
{
	return (clnt_call(clnt, delete_tuple,
		(xdrproc_t) xdr_int, (caddr_t) &key,
		(xdrproc_t) xdr_status, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
check_existence_1(int key, status *clnt_res,  CLIENT *clnt)
{
	return (clnt_call(clnt, check_existence,
		(xdrproc_t) xdr_int, (caddr_t) &key,
		(xdrproc_t) xdr_status, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
copy_tuple_1(int key1, int key2, status *clnt_res,  CLIENT *clnt)
{
	copy_tuple_1_argument arg;
	arg.key1 = key1;
	arg.key2 = key2;
	return (clnt_call (clnt, copy_tuple, (xdrproc_t) xdr_copy_tuple_1_argument, (caddr_t) &arg,
		(xdrproc_t) xdr_status, (caddr_t) clnt_res,
		TIMEOUT));
}
