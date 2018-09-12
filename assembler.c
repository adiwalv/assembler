#include<stdio.h>
#include<string.h>
#include<stdlib.h>
struct symtab{
  int sym_table_index;
  char name[150];
  int size;
  int no_of_items;
  char defined;
  char type;
  char value[50];
  int address;
};

struct errorTable{
  int address;
  int errorType;
  int symTab_index;
};

char errorTypes[2][50] = {"Variable not defined!","Multiple definition of Variable"};

struct symtab table[100];
struct errorTable errors[100];

int checkEntry(char *s, int sym_table_index) {
  int i = 0;
  for(i = 0; i < sym_table_index; i++){
    if(strcmp(table[i].name,s) == 0) {
      return i;
    }
  }
  return -1;
}

int registerTable(char *s){
  if(strcmp(s,"eax") == 0)
    return 0;
  if(strcmp(s,"ecx") == 0)
    return 1;
  if(strcmp(s,"edx") == 0)
    return 2;
  if(strcmp(s,"ebx") == 0)
    return 3;
  if(strcmp(s,"esi") == 0)
    return 4;
  if(strcmp(s,"edi") == 0)
    return 5;
  return -1;
}

int main(int argc, char *argv[]) { 
  if( argc == 2 ) {
      
   }
   else if( argc > 2 ) {
      printf("At this moment this assembler can only work with one file at a time..\n");
   }
   else {
      printf("One argument expected.\n");
   }
  return 0;
}
