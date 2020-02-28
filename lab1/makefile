CC			= gcc
CFLAGS	=
OBJFILES= FTP_func.c FTP.h
LDFLAGS	=
TARGET	=

default:

client: $(OBJFILES)
				$(CC) $(CFLAGS) -o chat_client.o $(OBJFILES) chat_client.c $(LDFLAGS)

server: $(OBJFILES)
				$(CC) $(CFLAGS) -o chat_server.o $(OBJFILES) chat_server.c $(LDFLAGS)
