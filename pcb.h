#include <stdio.h>
#include <stdlib.h>
struct PCB { FILE *PC; struct PCB* next; int pageTable[10]; int PC_page; int PC_offset; int pages_max;};
struct PCB* makePCB(FILE *p);
