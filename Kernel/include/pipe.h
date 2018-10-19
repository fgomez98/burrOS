
typedef struct pipe_t pipe_t;
typedef struct pipeNode pipeNode;

void destroyPipe(char * name);
pipeNode * unlinkPipeR(pipeNode * node, char * name);
pipe_t * pipe(char * name);
int readPipe(pipe_t * pipe, char * resp, size_t ammount);
int writePipe(pipe_t * pipe, char * msg, size_t ammount);
int cmp(int * pid1, int * pid2);
int exists(pipe_t * pipe);
int availableToWrite(pipe_t * pipe);
int availableToRead(pipe_t * pipe);