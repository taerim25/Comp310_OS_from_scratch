#include <stdio.h>
#include <stdlib.h>
#include "pcb.h"
#include "memorymanager.h"
struct PCB* makePCB(FILE *p){
  struct PCB * p2 = (struct PCB*) malloc (sizeof(struct PCB));
//  p2 ->PC = (FILE*) malloc(sizeof(FILE));
//  *(p2->PC) = *p;
  p2->PC = p;
  p2->next = NULL;
  p2->PC_page = 0;
  p2->pages_max = countTotalPages(p);
  p2->PC_offset = 0 ;
  for(int i =0 ; i<10; i++){
    p2->pageTable[i] = -1; //set all pageTable entries to invalid first
  }
  return p2;
}
