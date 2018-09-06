#include<stdio.h>
#include<string.h>
#include<stdlib.h>
struct symtab{
  int table_index;
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
  char errorDescription[50];
};
struct symtab table[100];

int checkEntry(char *s, int table_index) {
  int i = 0;
  for(i = 0; i < table_index; i++){
    if(strcmp(table[i].name,s) == 0) {
      return i;
    }
  }
  return -1;
}
int registerTable(char *s){
  if(strcmp(s,"eax") == 0)
    return 000;
  if(strcmp(s,"ecx") == 0)
    return 001;
  if(strcmp(s,"edx") == 0)
    return 010;
  if(strcmp(s,"ebx") == 0)
    return 011;
  if(strcmp(s,"esi") == 0)
    return 100;
  if(strcmp(s,"edi") == 0)
    return 101;
  return -1;
}
int main() { 
  char line[150], *token, *token1;
  int outer, address = 0, table_index=0, count=0;
  static const char input[] = "program.asm";
  FILE *fp = fopen(input,"r");
  
  if(fp!=NULL){
    while(fgets(line, sizeof line, fp ) != NULL ) {
      token = strtok(line,"\n\t\r ");
      if(strcmp(token,"section") == 0) {
	token = strtok(NULL,"\n\t\r ");
	if(strcmp(token,".data") == 0) {
	  break;
	}	
      }
    }
    while(fgets(line, sizeof line, fp) != NULL) {     
      token = strtok(line,"\n\t\r ");
      if(strcmp(token,"section") == 0)
	break;
      strcpy(table[table_index].name,token);
      token = strtok(NULL, "\n\t\r ");
      while(token) {
	if(strcmp(token,"dd") == 0) {
	  token = strtok(NULL,"\n\t\r ");
	  count = 0;
	  strcpy(table[table_index].value,token);
          table[table_index].table_index = table_index;
	  token1 = strtok(token,",");
	  while(token1) {
	    token1 = strtok(NULL,",");
	    count++;
	  }
	  table[table_index].no_of_items = count;
	  table[table_index].size = 4 * table[table_index].no_of_items;
	  table[table_index].address = table[table_index-1].address + table[table_index-1].size;
	  table[table_index].defined = 'd';
	  table[table_index].type = 's';
	} else if(strcmp(token,"db") == 0) {
	  count = 0;
	  token = strtok(NULL,"\n\t\r ");
	  strcpy(table[table_index].value,token);
	  token1 = strtok(token,",");
	  count = strlen(token) - 3;
	  while(token1) {
	    token1 = strtok(NULL,",");
	    count++;
	  }
	  table[table_index].no_of_items = count;
	  table[table_index].size = 1 * table[table_index].no_of_items;
	  table[table_index].address = address + table[table_index-1].size;
	  table[table_index].defined = 'd';
	  table[table_index].type = 's';
          table[table_index].table_index = table_index;
	}
	token = strtok(NULL,"\n\t\r");
      }
      table_index++;
    }
    rewind(fp);
    
    while(fgets(line, sizeof line, fp ) != NULL ) {
      token = strtok(line,"\n\t\r ");
      if(strcmp(token,"section") == 0) {
	token = strtok(NULL,"\n\t\r ");
	if(strcmp(token,".bss") == 0) {
	  break;
	}	
      }
    }
    while(fgets(line, sizeof line, fp) != NULL) {
      token = strtok(line,"\n\t\r ");
      if(strcmp(token,"section") == 0)
	break;
      strcpy(table[table_index].name,token);
      token = strtok(NULL, "\n\t\r ");
      while(token) {
	if(strcmp(token,"resb") == 0) {
	  token = strtok(NULL,"\n\t\r ");
	  strcpy(table[table_index].value,token);
	  table[table_index].no_of_items = 0;
          table[table_index].table_index = table_index;
	  table[table_index].size = 1 * atoi(token);
	  table[table_index].address = table[table_index-1].address + table[table_index-1].size;
	  table[table_index].defined = 'd';
	  table[table_index].type = 's';
	} else if(strcmp(token,"resd") == 0) {
	  token = strtok(NULL,"\n\t\r ");
	  strcpy(table[table_index].value,token);
	  table[table_index].no_of_items = atoi(token);
          table[table_index].table_index = table_index;
	  table[table_index].size = 4 * atoi(token);
	  table[table_index].address = table[table_index-1].address + table[table_index-1].size;
	  table[table_index].defined = 'd';
	  table[table_index].type = 's';
	}
	token = strtok(NULL,"\n\t\r");
      }
      table_index++;
    }
    rewind(fp);
    address = 0;
    
    while(fgets(line, sizeof line, fp ) != NULL ) {
      address++;
      token = strtok(line,"\n\t\r ");
      if(strcmp(token,"section") == 0) {
	token = strtok(NULL,"\n\t\r ");
	if(strcmp(token,".text") == 0) {
	  break;
	}	
      }
    }
    while(fgets(line, sizeof line, fp) != NULL) {
      address++;
      token = strtok(line,"\n\t\r ");
      if(strcmp(token,"section") == 0)
	break;
      if(strcmp(token,"global") == 0) {
	token = strtok(NULL,"\n\t\r ");
	token1 = strtok(token,",");
	while(token1) {
	  strcpy(table[table_index].name,token1);
	  table[table_index].defined = 'u';
	  table[table_index].type = 'l';
          table[table_index].table_index = table_index;
	  table_index++;
	  token1 = strtok(NULL,",");
	}
      }
      if (strcmp(&token[strlen(token)-1],":") == 0) {
	token[strlen(token)-1] = '\0';
        int c = checkEntry(token,table_index);
        if(c < 0){
        table[table_index].table_index = table_index;
	strcpy(table[table_index].name,token);
	table[table_index].defined = 'd';
	table[table_index].type = 'l';
	table[table_index].address = address;
	table_index++;}
        else {
          table[c].defined = 'd';
          table[c].address = address;
        }
      }
      if(strcmp(token,"jmp") == 0){
        token = strtok(NULL,"\n\t\r ");
        int c = checkEntry(token,table_index);
        if(c < 0) {
        table[table_index].table_index = table_index;
	strcpy(table[table_index].name,token);
	table[table_index].defined = 'u';
	table[table_index].type = 'l';
	table[table_index].address = address;
	table_index++;
        } 
      }
    }
    table[0].address = 0;
    printf("\tProgram:\n");
    rewind(fp);
    while ( fgets ( line, sizeof line, fp ) != NULL )
      printf("%s",line);
    printf("\n\tSym Table:\n");
    printf("Table Index\tName\tSize\tNo of items\tType\tDefined\tValue\t\t\t\tAddress\n");
    for(outer = 0; outer < table_index; outer++) {
      printf("%d\t\t%s\t%d\t%d\t\t%c\t%c\t%s\t\t\t\t%d\n",table[outer].table_index,table[outer].name,table[outer].size,table[outer].no_of_items,table[outer].type,table[outer].defined,table[outer].value,table[outer].address);  
    }
    
    //printf("\n%d",no_of_lines);
  } else {
      perror(input);
   }
  return 0;
}
