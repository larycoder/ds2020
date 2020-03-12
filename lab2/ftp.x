struct file{
	string file_name<255>;
	char type;
};

struct file_content{
	int fd;
	char content;
};

struct return_content{
	char content;
	int check;
};

program ftp_program {
	version ftp_vers {
		int openFile(file) = 1;
		int appendContent(file_content) = 2;
		return_content readContent(int) = 3;
		int closeFile(int) = 4;
	} = 1;
} = 0x23451111;
