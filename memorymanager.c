#include "kernel.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "pcb.h"
#include "memorymanager.h"
#include "ram.h"
//char filename[0xFFF ];
FILE * copyPointer(FILE *f){
    //THIS IS LINUX SPECIFIC SOLUTION, try to find another way of copying file pointer
  char filename[0xFFF ];
  memset(filename, 0,sizeof filename);
  int fno = fileno(f);
  char proclnk[0xFFF ];
  ssize_t r;
  sprintf(proclnk, "/proc/self/fd/%d", fno);
  r = readlink(proclnk, filename, 0xFFF);
  FILE *fp2 = fopen (filename, "r");
  return fp2;
}
int countTotalPages(FILE *f){
  FILE *fp2 = copyPointer(f);
  int lines = 0;
  while(!feof(fp2))
  {
    char ch = fgetc(fp2);
    if(ch == '\n')
      {
        lines++;
      }
  }
  fclose(fp2);
  if(lines%4 != 0){
    return lines/4 +1;
  }
  else return lines/4;
}
FILE *findPage(int pageNumber, FILE *f){
  FILE *fp2 = copyPointer(f);
  int page=0;
  char buf[1000];
  while(page <pageNumber){
    fgets(buf, 999, fp2);
    fgets(buf, 999, fp2);
    fgets(buf, 999, fp2);
    fgets(buf, 999, fp2);
    page++;
  }
  return fp2;
}
int findFrame(FILE *page){
  for(int a=0; a< 10; a++){
    if(ram[a] == NULL){ //might have to change
      return a;
    }
  }
  return -1;
}
int findVictim(struct PCB *p){
  int r = rand()%10;
  int checkPage=0;
  for(int b =0; b<10; b++){
    if(r == p-> pageTable[b]){
      checkPage= 1;
    }
  }
  if(checkPage == 0){
    return r;
  }
  else{
    int a = r+1;
    while(a !=r){
      if(a> 9){ //use mod so that index doesn't go out of bounds
        a =a %10;
      }
      checkPage = 0;
      for(int b =0; b<10; b++){
        if(a == p-> pageTable[b]){ //TO FIX
          checkPage= 1;
        }
      }
      if(checkPage == 0){
        return a;
      }
      a++;
    }
  }
  return -1;
}
int updateFrame(int frameNumber, int victimFrame, FILE *page){
  if(frameNumber == -1){
    ram[victimFrame] = page;
  }
  else{
    ram[frameNumber] = page;
  }
  return 0;
}
int updatePageTable(struct PCB *p, int pageNumber, int frameNumber, int victimFrame){
  if(frameNumber == -1){
    p->pageTable[pageNumber] = victimFrame;
  }
  else{
    p->pageTable[pageNumber] = frameNumber;
  }
  return 0;
}
int launcher(FILE * p){
  if( p == NULL){
    printf("File given is NULL \n");
    return -1;
  }
  char dir[200] = "BackingStore/";
  char b[200];
  sprintf(b, "%d", fileno(p));
  strcat(dir,b);
  char d[200] = ".txt";
  strcat(dir,d);
  FILE * dest = fopen( dir, "w");
  char buf[1000];
  char c = fgetc(p);
//  printf(" %c \n" , c);
//  if(c == EOF){
//    printf("EOF \n" );
//  }
  while (c!=EOF)
  {
        fputc(c, dest);
        c = fgetc(p);
  }
  fclose(p);
  fclose(dest);
  dest = fopen(dir, "r");
  int numPages = countTotalPages(dest);
  int frameNo = findFrame(p);
  int vicNo = -1;
  struct PCB *pcb;
  if(linkedList-> tail ==NULL ){
    pcb = linkedList->head;
  }
  else{
    pcb = linkedList->tail;
  }
  pcb->PC = dest;
  if(pcb == NULL){
    printf("No PCB initialized \n");
    return 0;
  }
  if(numPages <=1 ){

    if (frameNo == -1){
      vicNo = findVictim(pcb);
    }
    updateFrame(frameNo, vicNo,dest);
    updatePageTable(pcb, 0, frameNo,vicNo);
  }
  else{
    FILE * p2 =  copyPointer(dest); // second page, with first page being dest
    char buf[1000];
    fgets(buf, 999, p2);
    fgets(buf, 999, p2);
    fgets(buf, 999, p2);
    fgets(buf, 999, p2);
    if (frameNo == -1){
      vicNo = findVictim(pcb);
    }
    updateFrame(frameNo, vicNo,dest);
    updatePageTable(pcb, 0, frameNo,vicNo);
    vicNo = -1;
    frameNo = findFrame(p2);
    if (frameNo == -1){
      vicNo = findVictim(pcb);
    }
    updateFrame(frameNo, vicNo,p2);
    updatePageTable(pcb, 1, frameNo,vicNo);
  }
  return 1;
}
