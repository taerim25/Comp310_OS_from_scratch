#include <stdio.h>
#include <stdlib.h>
#include "kernel.h"
void addToRAM(FILE *p){
  for(int i =0 ; i< 10 ; i++){
    if(ram[i] == NULL){
      ram[i] = p;
      break;
    }
  }
}
int removeFromRAM(FILE *p){
  for(int i =0 ; i< 10 ; i++){
    if(p== NULL){
      return -1;
    }
    if(ram[i] == p){
      fclose(ram[i]);
      ram[i] = NULL ;
      return 0;
    }
  }
  return -1;
}
