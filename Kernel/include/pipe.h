
typedef struct pipe_t pipe_t;
typedef struct pipeNode pipeNode;

void unlinkPipe(char * name);
pipeNode * unlinkPipeR(pipeNode * node, char * name);
pipe_t * pipe(char * name);
int readPipe(pipe_t * pipe, char * resp, size_t ammount);
int writePipe(pipe_t * pipe, char * msg, size_t ammount);
int getAvailableToWrite(int start, int final, int size);
int cmp(int * pid1, int * pid2);