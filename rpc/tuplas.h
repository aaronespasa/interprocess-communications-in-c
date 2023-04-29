/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _TUPLAS_H_RPCGEN
#define _TUPLAS_H_RPCGEN

#include <rpc/rpc.h>

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif


struct Value {
	int key;
	char *value1;
	int value2;
	double value3;
};
typedef struct Value Value;

struct TwoKeys {
	int key1;
	int key2;
};
typedef struct TwoKeys TwoKeys;

#define TUPLE_SERVICE 13391170
#define TUPLE_SERVICE_V1 1

#if defined(__STDC__) || defined(__cplusplus)
#define INIT 1
extern  enum clnt_stat init_clnt(int *, CLIENT *);
extern  bool_t init_1_svc(int *, struct svc_req *);
#define SET_VALUE 2
extern  enum clnt_stat set_value_clnt(Value , int *, CLIENT *);
extern  bool_t set_value_1_svc(Value , int *, struct svc_req *);
#define GET_VALUE 3
extern  enum clnt_stat get_value_clnt(int , Value *, CLIENT *);
extern  bool_t get_value_1_svc(int , Value *, struct svc_req *);
#define MODIFY_VALUE 4
extern  enum clnt_stat modify_value_clnt(Value , int *, CLIENT *);
extern  bool_t modify_value_1_svc(Value , int *, struct svc_req *);
#define DELETE_KEY 5
extern  enum clnt_stat delete_key_clnt(int , int *, CLIENT *);
extern  bool_t delete_key_1_svc(int , int *, struct svc_req *);
#define EXIST 6
extern  enum clnt_stat exist_clnt(int , int *, CLIENT *);
extern  bool_t exist_1_svc(int , int *, struct svc_req *);
#define COPY_KEY 7
extern  enum clnt_stat copy_key_clnt(TwoKeys , int *, CLIENT *);
extern  bool_t copy_key_1_svc(TwoKeys , int *, struct svc_req *);
extern int tuple_service_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define INIT 1
extern  enum clnt_stat init_clnt();
extern  bool_t init_1_svc();
#define SET_VALUE 2
extern  enum clnt_stat set_value_clnt();
extern  bool_t set_value_1_svc();
#define GET_VALUE 3
extern  enum clnt_stat get_value_clnt();
extern  bool_t get_value_1_svc();
#define MODIFY_VALUE 4
extern  enum clnt_stat modify_value_clnt();
extern  bool_t modify_value_1_svc();
#define DELETE_KEY 5
extern  enum clnt_stat delete_key_clnt();
extern  bool_t delete_key_1_svc();
#define EXIST 6
extern  enum clnt_stat exist_clnt();
extern  bool_t exist_1_svc();
#define COPY_KEY 7
extern  enum clnt_stat copy_key_clnt();
extern  bool_t copy_key_1_svc();
extern int tuple_service_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_Value (XDR *, Value*);
extern  bool_t xdr_TwoKeys (XDR *, TwoKeys*);

#else /* K&R C */
extern bool_t xdr_Value ();
extern bool_t xdr_TwoKeys ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_TUPLAS_H_RPCGEN */
