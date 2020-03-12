/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "ftp.h"
#include "stdio.h"
#include <fcntl.h>
#include <unistd.h>

int *
openfile_1_svc(file *argp, struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */
	switch(argp->type){
		case 'u':
			result = open(argp->file_name, O_RDWR | O_CREAT | O_TRUNC, 00644);
			break;
		case 'd':
			result = open(argp->file_name, O_RDONLY);
			break;
		default:
			result = -1;
			break;
	}

	return &result;
}

int *
appendcontent_1_svc(file_content *argp, struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */
	result = write(argp->fd, &(argp->content), 1);

	return &result;
}

char *
readcontent_1_svc(int *argp, struct svc_req *rqstp)
{
	static char  result;

	/*
	 * insert server code here
	 */
	int check = read(*argp, &result, 1);
	if(check == 0) result = EOF;

	return &result;
}

int *
closefile_1_svc(int *argp, struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */
	result = close(*argp);

	return &result;
}
