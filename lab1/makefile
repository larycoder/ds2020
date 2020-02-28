CC			= gcc
CFLAGS	=
OBJFILES= chat_client.c chat_server.c FTP_func.c FTP.h
LDFLAGS	=
TARGET	=

default:

client: $(OBJFILES)
				$(CC) $(CFLAGS) -o chat_client.o $(OBJFILES) $(LDFLAGS)

server: $(OBJFILES)
				$(CC) $(CFLAGS) -o chat_server.o $(OBJFILES) $(LDFLAGS)
