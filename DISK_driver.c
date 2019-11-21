#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct PARTITION { int total_blocks; int block_size; } partition;
struct FAT { char *filename; int file_length; int blockPtrs[10]; int current_location; int fploc; } fat[20];// = {'\0',0,{-1},-1,-1};
char * block_buffer; FILE *fp[5];
char partitionName[300]= {'\0'};
void initIO(){
  partition.total_blocks = 0;
  partition.block_size = 0;
  for(int i = 0; i<20; i++){
    fat[i].filename = (char*) malloc(sizeof(char)*100);
    fat[i].file_length = 0;
    for(int a =0 ; a<10; a++){
      fat[i].blockPtrs[a] = -1;
    }
    fat[i].current_location = -1;
    fat[i].fploc = -1;
  }
  block_buffer = NULL;
  for(int i = 0 ; i< 5; i++){
    fp[i] = NULL;
  }
}
int Partition(char *name, int blocksize, int totalblocks){
  partition.block_size = blocksize;
  partition.total_blocks = totalblocks;
  char command[50];
  strcpy(command,"mkdir -p PARTITION");
  system(command);
  char dir[300];
  strcpy(dir, "PARTITION/");
  strcat(dir, name);
  strcpy(partitionName,dir);
//  char n[6] = ".txt";
//  strcat(dir,n);
  FILE * f =fopen(dir, "w");
  if(f== NULL){
    return 0;
  }
  fwrite(&(partition.total_blocks), sizeof(partition.total_blocks),1,f);
  fwrite(&(partition.block_size), sizeof(partition.block_size),1 , f);
  //fwrite(&(partition),sizeof(partition),1,f);
  for(int i =0; i<20; i++){
    fwrite(fat[i].filename, sizeof(fat[i].filename),1,f);
    fwrite(&(fat[i].file_length), sizeof(fat[i].file_length),1,f);
    fwrite(&(fat[i].blockPtrs), sizeof(fat[i].blockPtrs),1,f);
    fwrite(&(fat[i].current_location), sizeof(fat[i].current_location),1,f);
    fwrite(&(fat[i].fploc), sizeof(fat[i].fploc),1,f);
    //fwrite(&(fat[i]),sizeof(fat[i]),1,f);

  }
/*  fprintf(f, "%d %d ", patition.total_blocks, partition.block_size);
  for(int i = 0; i<20; i++){
    fprintf(f, "%s %d",  fat[i].filename, fat[i].file_length);
    fprintf(f, " %d %d %d %d %d %d %d %d %d %d",fat[i].blockPtrs[0],fat[i].blockPtrs[1], fat[i].blockPtrs[2],
  fat[i].blockPtrs[3],fat[i].blockPtrs[4],fat[i].blockPtrs[5],fat[i].blockPtrs[6],fat[i].blockPtrs[7],fat[i].blockPtrs[8]
,fat[i].blockPtrs[9]  );
    fprintf(f, " %d ", fat[i].current_location);

  }*/
  for(int i = 0; i < partition.total_blocks * partition.block_size; i++){
    fputc('0',f);
  }
  fputc(EOF, f);
  fclose(f);

  return 1;
}
int mount(char *name){
  block_buffer = malloc(sizeof(partition.block_size));

  char dir[300];
  strcpy(dir, "PARTITION/");
  strcat(dir, name);
  strcpy(partitionName,dir);
  FILE * f = fopen(dir,"r");
  if(f == NULL){
    return 0;
  }
//  fread(&(partition), sizeof(partition),1,f);
  fread(&(partition.total_blocks), sizeof(partition.total_blocks),1,f);
  fread(&(partition.block_size), sizeof(partition.block_size),1 , f);
  for(int i =0; i<20; i++){
    fread(fat[i].filename, sizeof(fat[i].filename),1,f);
    fread(&(fat[i].file_length), sizeof(fat[i].file_length),1,f);
    fread(&(fat[i].blockPtrs), sizeof(fat[i].blockPtrs),1,f);
    fread(&(fat[i].current_location), sizeof(fat[i].current_location),1,f);
    fread(&(fat[i].fploc), sizeof(fat[i].fploc),1,f);
  //  fread(&(fat[i]), sizeof(fat[i]),1,f);

  }
/*  fscanf(f, "%d %d ", &(patition.total_blocks),&(partition.block_size));
  for(int i = 0; i<20; i++){
    fscanf(f, "%s %d",  fat[i].filename, &(fat[i].file_length));
    fscanf(f, " %d %d %d %d %d %d %d %d %d %d",&(fat[i].blockPtrs[0]),&(fat[i].blockPtrs[1]), &(fat[i].blockPtrs[2]),
  &(fat[i].blockPtrs[3]),&(fat[i].blockPtrs[4]),&(fat[i].blockPtrs[5]),&(fat[i].blockPtrs[6]),&(fat[i].blockPtrs[7]),&(fat[i].blockPtrs[8])
,&(fat[i].blockPtrs[9])  );
    fscanf(f, " %d ", &(fat[i].current_location));
  }*/
  fclose(f);
  return 1;
}
int openfile(char *name){
  for(int i = 0; i< 20; i++){
    if(fat[i].filename != NULL){
/*      printf("Filename: %s, index: %i \n",fat[i].filename, i);
      printf("Name: %s \n",name);
      printf("comparison result: %i \n",strcmp(fat[i].filename, name)  );*/
      if(strcmp(fat[i].filename, name) == 0 ){
        for(int a = 0; a< 5; a++){
          if(fp[a] == NULL){
            char dir[300];
    //      strcpy(dir, "PARTITION/")
            strcat(dir, name);
            FILE *f = fopen(partitionName,"r+");//fp made to point first block of file
            fseek(f,sizeof(partition)+sizeof(fat)+fat[i].blockPtrs[0], SEEK_SET);
            fp[a] = f;
            fat[i].fploc = a;
            return i;
          }
          else{
            fat[i].fploc = a;
            return i;
          }
        }
        return -1; //fp FULL
      }
    }
  }
  //file not found
  for(int b = 0; b<20; b++){
    if(fat[b].file_length == 0){
      int blocks[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
  //    int offset = 0;
/*      if(b >0){ //not the first file in FAT
        offset = fat[b-1].blockPtrs[9] + partition.block_size;
      }
      for(int c = 0; c<10; c++){
        blocks[c] =  offset+ (c) * patition.block_size;
      }*/
//      fat[b].filename = (char*)malloc(sizeof(char)*strlen(name)+1);
      for(int i =0; i<strlen(name); i++){
        fat[b].filename[i] = name[i];
      }
      strcpy(fat[b].filename, name);
  //    fat[b].filename = name;
      fat[b].file_length = 0;
      memcpy(fat[b].blockPtrs, blocks,sizeof(blocks));
      fat[b].current_location = 0;
      fat[b].fploc = -1;
      //add new entry to fat
      return b;
    }
  }
  return -1; //fat full
}
int readBlock(int file){
  if(file == -1){
    return -1; //invalid index number
  }
  if(fat[file].fploc == -1){
    return -99;
  }
  FILE *f = fp[fat[file].fploc];
  struct FAT info = fat[file];
  if(fat[file].current_location == 10){
    return -1; //at end of file
  }
  memset(block_buffer, 0,sizeof(block_buffer));
  int loc = fat[file].blockPtrs[fat[file].current_location];
  size_t size = sizeof(partition.total_blocks) + sizeof(partition.block_size);
  for(int i =0 ; i<20; i++){
    size = size + sizeof(fat[i].filename)+sizeof(fat[i].file_length)+ sizeof(fat[i].blockPtrs)+ sizeof(fat[i].current_location)+ sizeof(fat[i].fploc);
  }
  if(loc == -1){
    if(file == 0 &&fat[file].current_location == 0 ){
      fseek(f, size, SEEK_SET);
    }
    else if(fat[file].current_location == 0){
      int z;
      for(z = 0; z<10; z++){
        if(fat[file-1].blockPtrs[z] == -1) break;
      }
      fseek(f, size+fat[file-1].blockPtrs[z-1]+partition.block_size, SEEK_SET);
    }
    else{
      fseek(f,size+fat[file].blockPtrs[fat[file].current_location-1] +partition.block_size,SEEK_SET);
    }
  }
  else{
    fseek(f, size+loc, SEEK_SET);
  }
  fread(block_buffer, partition.block_size,1,f);
  fat[file].current_location = fat[file].current_location +1;
  return 0;
}
char *returnBlock(){
  return block_buffer;
}
int writeBlock(int file, char *data){
  int totalLength= 0;
  for(int e = 0; e< 20; e++){
    totalLength = totalLength + fat[e].file_length;
  }
  if(totalLength > partition.total_blocks){
    return -10;
  }
  if(file == -1){
    return -1; //invalid index number
  }
  if(fat[file].fploc == -1){
    return -99;
  }
  FILE *f = fp[fat[file].fploc];
  if(fat[file].current_location == 10){
    return -1; //at end of file
  }
  int loc = fat[file].blockPtrs[fat[file].current_location];
  size_t size = sizeof(partition.total_blocks) + sizeof(partition.block_size);
  for(int i =0 ; i<20; i++){
    size = size + sizeof(fat[i].filename)+sizeof(fat[i].file_length)+ sizeof(fat[i].blockPtrs)+ sizeof(fat[i].current_location)+ sizeof(fat[i].fploc);
  }
  if(loc == -1){
    if(file == 0 &&fat[file].current_location == 0){
      fat[file].blockPtrs[fat[file].current_location] = 0;
      fseek(f, size, SEEK_SET);
    }
    else if(fat[file].current_location == 0){ // problem
      int z ;
      for(z = 0; z<10; z++){
        if(fat[file-1].blockPtrs[z] == -1) break;
      }
      fat[file].blockPtrs[fat[file].current_location] = fat[file-1].blockPtrs[z-1]+partition.block_size;
      fseek(f, size+fat[file].blockPtrs[fat[file].current_location], SEEK_SET);
    }
    else{
        fat[file].blockPtrs[fat[file].current_location] =   fat[file].blockPtrs[fat[file].current_location-1] + partition.block_size;
        fseek(f, size+fat[file].blockPtrs[fat[file].current_location], SEEK_SET);
    }
  }
  else{
    fseek(f, size+loc, SEEK_SET);
  }
  strncpy(block_buffer,data,partition.block_size*sizeof(char));
  fputs(block_buffer,f);
  fat[file].current_location = fat[file].current_location+1;
  fat[file].file_length++;
  if(strlen(data) > partition.block_size){
    int block_required;
    if(strlen(data)%partition.block_size == 0){
      block_required = strlen(data)/partition.block_size;
    }
    else{
      block_required = strlen(data)/partition.block_size +1;
    }
    block_required = block_required -1;
    int i = partition.block_size;
    while(block_required > 0){
      strncpy(block_buffer,data+i,partition.block_size*sizeof(char));
      if(loc == -1){
          fat[file].blockPtrs[fat[file].current_location] =   fat[file].blockPtrs[fat[file].current_location-1] + partition.block_size;
      }
      i = i + partition.block_size;
      fputs(block_buffer,f);
      fat[file].current_location = fat[file].current_location+1;
      block_required--;
      fat[file].file_length++;
    }
  }
  return 0;
}
