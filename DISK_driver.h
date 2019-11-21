void initIO();
int Partition(char *name, int blocksize, int totalblocks);
int mount(char *name);
int openfile(char *name);
int readBlock(int file);
char *returnBlock();
int writeBlock(int file, char *data);
