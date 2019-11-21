#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *model[1000][2];
void setVar(char *name, char *value){
  int i =0;
  for(i=0; i<1000; i++){

     if(model[i][0] == NULL ){
      model[i][0] = strdup(name);
      model[i][1] = strdup(value);
      break;
    }
    else if(strcmp( model[i][0],name) == 0){
      free(model[i][1]);
      model[i][1] = strdup(value);
      break;
    }
  }
}
char* get(char *name){
  int i =0;
  for(i=0; i<1000; i++){
     if(model[i][0] != NULL && strcmp( model[i][0], name) == 0){
      return strdup(model[i][1]);
    }
  }
  printf("NOT FOUND \n");
  return NULL;
}
