#define BUFFERSIZE 108192
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

typedef struct fileDecryptorStruct fileDecryptor;

int pipe(int fd[]);
int open(int fd);
int close(int fd);
int read(int fd, char * msg, int amount);
int write(int fd, char * msg, int amount);
void initializeFileDecryptors();
void dup2(int newFd, int fdToReplace);
void printfd();
int openWithPid(int fd, int pid);