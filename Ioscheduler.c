#include "pcb.h"
#include "DISK_driver.h"
#include <string.h>
struct request{
  char *data;
  struct PCB *ptr;
  int cmd;
}IOqueue[10]={0};
char filename[200];
int ind = 0;
char *IOscheduler(char *data, struct PCB *ptr, int cmd){
  if(ind > 9 ){
    ind = ind%10;
  }
  //enqueue process
  IOqueue[ind].data = data;
  IOqueue[ind].ptr = ptr;
  IOqueue[ind].cmd = cmd;
  ind++;
  int cur = ind;
  int i = openfile(filename); 
  if(cmd == 0){ //read
    int err = readBlock(i);
    IOqueue[cur] = (const struct request){ 0 }; //dequeue
    if(err == -99){
    //  readBlock(openfile(filename)); //openfile again to create file pointer, might change this
      return NULL;
    }
    return returnBlock();
  }else if(cmd == 1){ //write
    IOqueue[cur] = (const struct request){ 0 };
    int err = writeBlock(i,data);

    if(err == -99){
      err =writeBlock(openfile(filename),data); //openfile again to create file pointer
    }
    if(err == -10){
      printf("Used all blocks for writing \n");
    }
  }
  ind--;
  return NULL;

}
void setFileName(char* file){
  strcpy(filename,file);
}
