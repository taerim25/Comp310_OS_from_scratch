#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "shellmemory.h"
#include "kernel.h"
#include "DISK_driver.h"
#include "Ioscheduler.h"
int help() {

  printf("COMMAND | DESCRIPTION \n");
  printf("help | Displays all the commands \n");
  printf("quit | Exits / terminates the shell with “Bye!” \n");
  printf("set VAR STRING | Assigns a value to shell memory \n");
  printf("print VAR | Prints the STRING assigned to VAR \n");
  printf("run SCRIPT.TXT | Executes the file SCRIPT.TXT \n");
  printf("exec p1 p2 p3 | Executes prog.txt prog2.txt \n");
  return 0 ;
}
int quit() {

  printf("Bye! \n" );
  return 99;
}
int set(char * name , char* val){

  setVar(name,val);
  return 0;
}
int print(char *name){
  char * val = get(name);
  if (val != NULL) {
  printf ("%s \n", val );
  }
  return 0;
}
int runS (char *filename){
  int runTimeErr = 0;
  char line[1000];
  FILE *p = fopen(filename,"rt" );
  if (p == NULL)
    return 7;
  fgets(line, 999,p);
//  char buf0[100], buf1[100], buf2[100];

  while(!feof(p)){
    if(strlen(line) >0) {
      runTimeErr = prompt(line);
    }
    if(runTimeErr == 99)
      break;
    fgets(line, 999,p);
  }
  fclose(p);
  return runTimeErr;
}
int exec(char * file1, char * file2, char *file3){
  FILE *p1, *p2,*p3;
  if(file1 != NULL){
    p1= fopen(file1, "rt");
    if(p1 != NULL){
      myinit(p1);
    }
    else if (file1[0]!= '\0'){
      return 7;
    }
  }
  if(file2 != NULL){
    p2= fopen(file2, "rt");
    if(p2 !=NULL){
      myinit(p2);
    }
    else if (file2[0] != '\0'){
      return 7;
    }
  }
  if(file3 != NULL){
    p3= fopen(file3, "rt");
    if(p3 != NULL){
      myinit(p3);
    }
    else if(file3[0] != '\0'){
      return 7;
    }
  }
  scheduler();
  return 0;
}
int Mount(char*name, char* number_of_blocks, char*block_size){
  char dir[300];
  strcpy(dir, "PARTITION/");
  strcat(dir, name);
  FILE * file;
  if(file = fopen(dir,"r")){
    fclose(file);
    mount(name);
  }
  else{
    int i = Partition(name,atoi(number_of_blocks),atoi(block_size));
    if(i== -1){
      return 9;
    }
    int d = mount(name);
    if(d== 0){
      return 9;
    }
  }
  return 0 ;
}
int Write(char * filename, char* words){
    setFileName(filename);
    IOscheduler(words,linkedList->head, 1);
    return 0;
}
int Read(char * filename, char* var){
    setFileName(filename);
    char* dataRead= IOscheduler(filename,linkedList->head, 0);
    if(dataRead == NULL){
      return 10;
    }
    set(var,dataRead);
    return 0;
}
int interpreter(char words[][100]) {
//  words[0][strcspn(words[0]," ")] =0;
  int errCode = 0;
//  int compare = strcmp(words[0], "help\0");
//  int compare2 = strcmp(words[0], "help\n");
//  printf("%s \n", words[1]);
  if (strcmp(words[0],"help") == 0 ){

    if (strlen(words[1])> 0){
      return 6;
    }
    errCode= help();
  }
  else if (strcmp(words[0],"quit") == 0 )
    errCode= quit();
  else if (strcmp(words[0] , "set")== 0){
    if (strlen(words[1])<1 || strlen(words[2])<1 ){
      return 2;
    }
    errCode = set(words[1], words[2]);
  }
  else if (strcmp(words[0] , "print")== 0){
    if (strlen(words[1])<1){
      return 3;
    }
    errCode = print(words[1]);
  }
  else if (strcmp(words[0], "run") == 0) {
    if (strlen(words[1])<1 ) return 4;
    errCode = runS(words[1]);

  }
  else if(strcmp(words[0], "exec") == 0){
    if(strlen(words[1])<1) return 5;
      errCode = exec(words[1],words[2],words[3]);
  }
  else if(strcmp(words[0], "Mount")==0 ){
    if(strlen(words[1]) < 1 || strlen(words[2]) < 1 || strlen(words[3])<1){
      return 8;
    }
    errCode = Mount(words[1],words[2],words[3]);
  }
  else if(strcmp(words[0], "Write")==0){
    Write(words[1],words[2]);
  }
  else if(strcmp(words[0], "Read")==0){
    Read(words[1],words[2]);
  }
  return errCode;

}
