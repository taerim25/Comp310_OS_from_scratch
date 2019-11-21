#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "kernel.h"
#include "cpu.h"
#include "shell.h"
#include "pcb.h"
#include "memorymanager.h"
#include "DISK_driver.h"

int run(int quanta){
  if(cpu-> IP == NULL){
    return -1 ; // IP should not be null
  }
  int q = quanta;
  struct PCB* cur = linkedList->head;
  FILE * f2 = copyPointer(cpu->IP);
//  printf("Quanta: %d \n", q);
  while(q>0 ){
    if(feof(cpu-> IP)){ //reached end of file
      return -1;
    }
    if(cur->PC_offset == 4){  //generate pseudo interrupt
      cur->PC_page = cur->PC_page +1;
      if( cur->PC_page > cur-> pages_max){
        return -1; //terminate
      }
      if(cur->pageTable[cur->PC_page] != -1 ){ //valid page exists in frame
        int frame = cur->pageTable[cur->PC_page];
        cur->PC = ram[frame];
        cpu->IP = cur->PC;
      }
      else{
        //findPage
        FILE *pagePointer = findPage(cur->PC_page,cpu->IP);
/*        if(pagePointer == NULL){
          printf("problem :o \n");
        }*/
        fclose(cpu->IP);
        int space = findFrame(pagePointer);
        if(space == -1 ) { //ram full
          int vicNo = findVictim(cur);
          updateFrame(space, vicNo, pagePointer);
          updatePageTable(cur,cur->PC_page, space,vicNo);
          cur->PC = ram[vicNo];
        }
        else{
          updateFrame(space, -1, pagePointer);
          updatePageTable(cur,cur->PC_page, space,-1);
          cur->PC = ram[space];
        }
        cpu->IP = cur->PC;
      }
      cur->PC_offset = 0;
      return 0;
    }
    if(fgets( cpu->IR, 999,cpu->IP )!=NULL){
      cur->PC_offset= cur->PC_offset +1;
      int result= prompt(cpu->IR);
      if(result == 99){
        exit(99);
      }
    }
  q--;
  }
  if(feof(cpu-> IP)){ //reached end of file
    return -1;
  }

  return 0;
}
//int main(void){
  //struct CPU* cpu =  (struct CPU*) malloc (sizeof(struct CPU));
  //cpu->quanta = 2;
//}
