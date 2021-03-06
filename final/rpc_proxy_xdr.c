/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "rpc_proxy.h"

bool_t
xdr_dest_host (XDR *xdrs, dest_host *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->host, ~0))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->port))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_content (XDR *xdrs, content *objp)
{
	register int32_t *buf;

	 if (!xdr_array (xdrs, (char **)&objp->content_val, (u_int *) &objp->content_len, 100,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_p_message (XDR *xdrs, p_message *objp)
{
	register int32_t *buf;

	 if (!xdr_content (xdrs, &objp->ct))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->length))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->fd))
		 return FALSE;
	return TRUE;
}
