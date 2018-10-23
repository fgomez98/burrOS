#define BUFFERSIZE 108192
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

typedef struct fileDecryptor fileDecryptor;

int pipe(int fd[]);
int open(int fd);
int close(int fd);
int read(int fd, char * msg, int amount);
int write(int fd, char * msg, int amount);
void initializeFileDecryptors();