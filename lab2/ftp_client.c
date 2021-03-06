/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "ftp.h"
#include <memory.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void uploadFile(char *host, char* dest, char*src) {
	CLIENT *clnt;
	int src_file;
	file_content data;
	file argv;

#ifndef	DEBUG
	clnt = clnt_create (host, ftp_program, ftp_vers, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	/* Parameter prepare */
	argv.file_name = dest;
	argv.type = 'u';
	int *dest_file = openfile_1(&argv, clnt);
	if(dest_file == (int *)NULL){
		clnt_perror(clnt, "call failed");
		exit(1);
	}
	data.fd = *dest_file;

	/* Open file */
	src_file = open(src, O_RDONLY);

	/* transfer file until EOF */
	while(read(src_file, &(data.content), 1) != 0){
		appendcontent_1(&data, clnt);
	}

	/* Close file */
	close(src_file);
	closefile_1(&(data.fd), clnt);

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}

void downloadFile(char* host, char* dest, char* src){
	CLIENT *clnt;
	file argv;
	int *src_file, dest_file;
	return_content *content;

#ifndef DEBUG
	clnt = clnt_create(host, ftp_program, ftp_vers, "udp");
	if(clnt == NULL){
		clnt_pcreateerror(host);
		exit(1);
	}
#endif /* DEBUG */

	/* Parameter prepare */
	argv.file_name = src;
	argv.type = 'd';
	src_file = openfile_1(&argv, clnt);
	if(src_file == (int *)NULL){
		clnt_perror(clnt, "call failed");
	}

	/* Open file */
	dest_file = open(dest, O_RDWR | O_CREAT | O_TRUNC, 00644);

	/* transfer file until EOF */
	while(1){
		content = readcontent_1(src_file, clnt);
		if(content->check == 0) break;
		write(dest_file, &(content->content), 1);
	}

	/* close file */
	close(dest_file);
	closefile_1(src_file, clnt);

#ifndef DEBUG
	clnt_destroy(clnt);
#endif /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;
	char *dest;
	char *src;

	if (argc < 5) {
		printf ("usage: %s lack of args\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	dest = argv[3];
	src = argv[4];
	if(strcmp(argv[2], "-u") == 0){
		uploadFile(host, dest, src);
	}
	else if(strcmp(argv[2], "-d") == 0){
		downloadFile(host, dest, src);
	}
exit (0);
}
