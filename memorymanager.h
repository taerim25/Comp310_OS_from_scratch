#include <stdio.h>
FILE * copyPointer(FILE *f);
int countTotalPages(FILE *f);
FILE *findPage(int pageNumber, FILE *f);
int findFrame(FILE *page);
int launcher(FILE * p);
int findVictim(struct PCB *p);
int updateFrame(int frameNumber, int victimFrame, FILE *page);
int updatePageTable(struct PCB *p, int pageNumber, int frameNumber, int victimFrame);
