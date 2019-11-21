#include <stdio.h>
extern FILE *ram[10];
extern struct node* linkedList;
extern struct CPU* cpu;
struct CPU { FILE *IP; char IR[1000]; int quanta; };
struct node{
  struct PCB * head;
  struct PCB * tail;
};
void myinit(FILE * p);
void scheduler();
