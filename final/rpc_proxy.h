/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _RPC_PROXY_H_RPCGEN
#define _RPC_PROXY_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct dest_host {
	char *host;
	int port;
};
typedef struct dest_host dest_host;

typedef struct {
	u_int content_len;
	char *content_val;
} content;

struct p_message {
	content ct;
	int length;
	int fd;
};
typedef struct p_message p_message;

#define rpc_proxy_program 0x40102103
#define rpc_proxy_ver 1

#if defined(__STDC__) || defined(__cplusplus)
#define connect_proxy 1
extern  int * connect_proxy_1(dest_host *, CLIENT *);
extern  int * connect_proxy_1_svc(dest_host *, struct svc_req *);
#define send_proxy 2
extern  int * send_proxy_1(p_message *, CLIENT *);
extern  int * send_proxy_1_svc(p_message *, struct svc_req *);
#define recv_proxy 3
extern  p_message * recv_proxy_1(int *, CLIENT *);
extern  p_message * recv_proxy_1_svc(int *, struct svc_req *);
#define close_proxy 4
extern  int * close_proxy_1(int *, CLIENT *);
extern  int * close_proxy_1_svc(int *, struct svc_req *);
extern int rpc_proxy_program_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define connect_proxy 1
extern  int * connect_proxy_1();
extern  int * connect_proxy_1_svc();
#define send_proxy 2
extern  int * send_proxy_1();
extern  int * send_proxy_1_svc();
#define recv_proxy 3
extern  p_message * recv_proxy_1();
extern  p_message * recv_proxy_1_svc();
#define close_proxy 4
extern  int * close_proxy_1();
extern  int * close_proxy_1_svc();
extern int rpc_proxy_program_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_dest_host (XDR *, dest_host*);
extern  bool_t xdr_content (XDR *, content*);
extern  bool_t xdr_p_message (XDR *, p_message*);

#else /* K&R C */
extern bool_t xdr_dest_host ();
extern bool_t xdr_content ();
extern bool_t xdr_p_message ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_RPC_PROXY_H_RPCGEN */
