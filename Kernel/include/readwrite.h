typedef struct fileDecryptor fileDecryptor;

int pipe(int fd[]);
int open(int fd);
int close(int fd);
int read(int fd, char * msg, int ammount);
int write(int fd, char * msg, int ammount);
