#include "shell.h"
#include <stdlib.h>
#include "cpu.h"
#include "pcb.h"
#include "kernel.h"
#include "ram.h"
#include <string.h>
#include "memorymanager.h"
#include "DISK_driver.h"
FILE *ram[10];
struct node* linkedList = NULL;
struct CPU mincpu =  {NULL, "", 2 };
struct CPU* cpu = &(mincpu);
void boot(){
  for(int i =0 ; i<10; i++){
    ram[i] = NULL;
  }
  char command[50];
  strcpy(command,"rm -rf BackingStore");
  system(command);
  strcpy(command,"mkdir BackingStore" );
  system(command);
}
void reset() {
/*  for(int i =0 ; i<10; i++){
    if(ram[i] != NULL){
      fclose(ram[i]);
    }
  }*/
  char command[50];
  strcpy(command,"rm BackingStore/*");
  system(command);
}
void addToReady(struct PCB * x){
  if (linkedList->head == NULL){
    linkedList->head = x;
  }
  else{
    if(linkedList->tail == NULL){
      linkedList->tail = x;
      linkedList->head->next = x;
    }
    else{
      linkedList->tail->next = x;
      linkedList->tail = x;
    }
  }
}
void scheduler(){
  struct PCB* cur;
  while( linkedList-> head != NULL ){
    if( (cpu->IP) ){
      printf("cpu already pointing to something \n");
      break;
      //error
    }
    else{
      cpu->IP = ((linkedList-> head)->PC);
      FILE * x = copyPointer((linkedList-> head)->PC);
      int b =run(cpu->quanta);
      cur = linkedList -> head;
      linkedList->head = linkedList->head->next;
      cur->next = NULL;
      if(b == -1 ){ //file at EOF
        int res = removeFromRAM(cpu->IP);
        if(cur == NULL ){
      //    break;
        }
        free(cur);
        cpu->IP = NULL;
        //todo
      }
      else{
        (cur)->PC =  cpu->IP;
        cpu->IP = NULL;
        addToReady(cur);
        //todo
      }
    }
  }
  reset();
}

int main(int argc, char **argv)
{
  boot();
  initIO();
  // Load config files, if any.

  // Run command loop.
  char prompt1[100] = {'$', '\0'};
  char userInput[1000];

  int errorCode=  0;
  printf("Kernel 2.0 loaded ! \n");
  printf("Welcome to the Geoffrey Taerim Kim shell! \n");
  printf("Version 3.0 updated April 2019 \n");
  while(1){
    char buf0[100], buf1[100], buf2[100];
    printf("%s", prompt1);
    fgets(userInput, 999, stdin);
//    userInput[strcspn(userInput, "\n")] = 0; //Since fgets add \n towards the end of user input, we need to remove as it creates problem in comparison
    errorCode = prompt(userInput);
    if(errorCode == 99){
      break;
    }
  }
  // Perform any shutdown/cleanup.


  return EXIT_SUCCESS;
}

void myinit(FILE * p){
  if(linkedList == NULL){
    linkedList = (struct node*)malloc(sizeof(struct node));
//    linkedList -> head = (struct PCB*) malloc(sizeof(struct PCB));
//    linkedList -> tail = (struct PCB*) malloc(sizeof(struct PCB));
  }
  if(cpu == NULL){ //useless for now
    printf("Initializing cpu \n");
    cpu = (struct CPU*)malloc(sizeof(struct CPU));
//    cpu ->IP = NULL;
//    cpu -> IP = ( FILE*) malloc(sizeof( FILE));
  }
//  addToRAM(p);
  struct PCB* pcb = makePCB(p);
  addToReady(pcb);
  launcher(p);
}
