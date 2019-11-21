#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "interpreter.h"
//char words[20][100];
int parse(char ui[], char words[][100]){
  int a,b;
  a =0;
  int w = 0;
  char tmp[200];
/*  while(ui[a] == ' '&&a<1000) a++;
  b = 0;
  while(a<1000&&ui[a] != ' '&&ui[a]!= '\n' &&ui[a]!= '\r'){
    arg0[b] = ui[a];
    b++;
    a++;
  }
  arg0[b] = '\0';
  b = 0;
  while(ui[a] == ' '&&a<1000) a++;
  while(a<1000&&ui[a] != ' '&&ui[a]!= '\n' &&ui[a]!= '\r'){
    arg1[b] = ui[a];
    b++;
    a++;
  }
  arg1[b] = '\0';
  b=0;
  while(ui[a] == ' '&&a<1000) a++;
  while(a<1000&&ui[a] != ' '&&ui[a]!= '\n' &&ui[a]!= '\r'){
    arg2[b] = ui[a];
    b++;
    a++;
  }
  arg2[b] = '\0';
  while(ui[a] == ' '&&a<1000) a++; */
  while(ui[a] != '\0' && a<1000 && ui[a]!='\n' && ui[a]!='\r'){
    while(ui[a] == ' '&&a<1000) a++;
    b=0;
    while(a<1000&&ui[a] != ' '&&ui[a]!= '\n' &&ui[a]!= '\r'){
      words[w][b] = ui[a];
      b++;
      a++;
    }
    words[w][b] = '\0';
    w++;
  }
  if (a==1000 || ui[a]=='\n' || ui[a]=='\r' || ui[a]=='\0')
	//	return interpreter(arg0, arg1,arg2); // completed successfully (true)
     return interpreter(words);
	else
		return -1;
}
int prompt(char buffer[]){
  // Load config files, if any.
  // Run command loop.
    char words[20][100];
    for (int i =0 ; i<20; i++){
      words[i][0]='\0';
    }

//    userInput[strcspn(userInput, "\n")] = 0; //Since fgets add \n towards the end of user input, we need to remove as it creates problem in comparison
    int errorCode = parse(buffer,words);
    if(errorCode == -1) printf("Parse error \n");
    if(errorCode == 2) printf("Incorrect argument for set \n");
    if(errorCode == 3) printf("Incorrect argument for Print \n");
    if(errorCode == 4) printf("Incorrect argument for run \n");
    if(errorCode == 5) printf("Incorrect argument for exec \n");
    if(errorCode == 6) printf("Incorrect argument for help \n");
    if(errorCode == 7) printf("No file in directory \n");
    if(errorCode == 8 ) printf("Incorrect argument for mount \n");
    if(errorCode == 9 ) printf("Mount error \n");
    if(errorCode == 10 ) printf("No data is read as file does not exist \n");
    if(errorCode == 99) return 99;
    return 0;
  // Perform any shutdown/cleanup.
}
