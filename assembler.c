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
  int errorType;;
};

char errorTypes[2][50] = {"Variable not defined!","Multiple definition of Variable"};

struct symtab table[100];
struct errorTable errors[100];

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

int main() { 
  char line[150], *token, *token1, *token2, *token3;
  int outer, address = 0, table_index=0, count=0,c;
  int op1,op2;
  static const char input[] = "program.asm";
  static const char immediate_output[] = "immediate.asm";
  FILE *ip = fopen(input,"r");
  FILE *op = fopen(immediate_output,"w");
  if(ip!=NULL){
    while(fgets(line, sizeof line, ip ) != NULL ) {
      token = strtok(line,"\n\t\r ");
      if(strcmp(token,"section") == 0) {
	token = strtok(NULL,"\n\t\r ");
	if(strcmp(token,".data") == 0) {
	  break;
	}	
      }
    }
    while(fgets(line, sizeof line, ip) != NULL) {     
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
    rewind(ip);
    
    while(fgets(line, sizeof line, ip ) != NULL ) {
      token = strtok(line,"\n\t\r ");
      if(strcmp(token,"section") == 0) {
	token = strtok(NULL,"\n\t\r ");
	if(strcmp(token,".bss") == 0) {
	  break;
	}	
      }
    }
    while(fgets(line, sizeof line, ip) != NULL) {
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
    rewind(ip);
    address = 0;
    
    while(fgets(line, sizeof line, ip ) != NULL ) {
      address++;
      token = strtok(line,"\n\t\r ");
      if(strcmp(token,"section") == 0) {
	token = strtok(NULL,"\n\t\r ");
	if(strcmp(token,".text") == 0) {
	  break;
	}	
      }
    }
    while(fgets(line, sizeof line, ip) != NULL) {
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
          strcpy(table[table_index].value,"***");
          table[table_index].table_index = table_index;
	  table_index++;
	  token1 = strtok(NULL,",");
	}
      }
      if (strcmp(&token[strlen(token)-1],":") == 0) {
	token[strlen(token)-1] = '\0';
        c = checkEntry(token,table_index);
        if(c < 0){
        table[table_index].table_index = table_index;
	strcpy(table[table_index].name,token);
	table[table_index].defined = 'd';
	table[table_index].type = 'l';
        strcpy(table[table_index].value,"***");
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
        strcpy(table[table_index].value,"***");
	table[table_index].address = address;
	table_index++;
        } 
      }
    }
    table[0].address = 0;
    printf("\tProgram:\n");
    rewind(ip);
    while ( fgets ( line, sizeof line, ip ) != NULL )
      printf("%s",line);
    printf("\n\tSym Table:\n");
    /**printf("Table Index\tName\t\tSize\tNo of items\tType\tDefined\tValue\t\t\t\t\tAddress\n");
    for(outer = 0; outer < table_index; outer++) {
      printf("%d\t\t%s\t\t%d\t%d\t\t%c\t%c\t%s\t\t\t\t\t%d\n",table[outer].table_index,table[outer].name,table[outer].size,table[outer].no_of_items,table[outer].type,table[outer].defined,table[outer].value,table[outer].address);  
      }**/
    printf("%12s%12s%12s%12s%12s%12s%40s%12s\n","Table Index","Name","Size","No of items","Type","Defined","Value","Address");
    for(outer = 0; outer < table_index; outer++) {
      printf("%12d%12s%12d%12d%12c%12c%40s%12d\n",table[outer].table_index,table[outer].name,table[outer].size,table[outer].no_of_items,table[outer].type,table[outer].defined,table[outer].value,table[outer].address);  
      }

    //printf("\n\nImmediate Code:\n");
    rewind(ip);
    while ( fgets ( line, sizeof line, ip ) != NULL )
    {
      token = strtok(line,"\n\t\r ");
      if(strcmp(token,"section") == 0) {
	token = strtok(NULL,"\n\t\r ");
	if(strcmp(token,".text") == 0) {
          break;
	}	
      }
    }
    while ( fgets ( line, sizeof line, ip ) != NULL )
    {
      token = strtok(line,"\n\t\r ");
      if(strcmp(token,"global") == 0) {
          break;
	}
    }
    while ( fgets ( line, sizeof line, ip ) != NULL )
    {
      token = strtok(line,"\n\t\r ");
      if (strcmp(&token[strlen(token)-1],":") == 0) {
        token = strtok(NULL,"\n\t\r ");
        token1 = strtok(NULL,"\n\t\r ");
        token2 = strtok(token1,", ");
        token3 = strtok(NULL,", ");

        // printf("\nHello %s %s", token2,token3);
        op1 = registerTable(token2);
        op2 = registerTable(token3);
        //printf("\n\n%s %s %d %d",token2,token3,op1,op2);
        if(op1 < 0 && op2 >= 0) {
           c = checkEntry(token2, table_index);
           if(c < 0) {
             fprintf(op,"Not definded variable");
           } else {
             fprintf(op,"\n%s SysTab#%d , Reg#%d ",token, c , op2 );
           }
        }
        else if(op2 < 0 && op1 >= 0) {
          
           c = checkEntry(token3, table_index);
           if(c < 0) {
             fprintf(op,"Not definded variable");
           } else {
             fprintf(op,"\n%s Reg#%d , SysTab#%d ",token, op1 , c );
           } 
        }
        else if(op1>=0 && op2>=0) {
          fprintf(op,"\n%s Reg#%d , Reg#%d ",token, op1 , op2 );
          }
      }
      else {
        token1 = strtok(NULL,"\n\t\r ,");
        token2 = strtok(NULL,"\n\t\r ,");
        //printf("\n%s %s %s %d %d", token,token1, token2, op1, op2);
        if(token2 == NULL) {
          //printf("Hello!\n");
          c = registerTable(token1);
          if (c < 0) {
            c = checkEntry(token1, table_index);
            if(c < 0) {
              printf("Error\n");
            } else {
              fprintf(op,"\n%s Symtab#%d", token, c);
            }
          } else {
            fprintf(op,"\n%s Reg#%d", token, c);
          }
        } else {
          op1 = registerTable(token1);
          op2 = registerTable(token2);
      
          //printf("\n\n%s %s %d %d",token2,token3,op1,op2);
          if(op1 < 0 && op2 >= 0) {
            c = checkEntry(token1, table_index);
            if(c < 0) {
              printf("Not definded variable1");
            } else {
              fprintf(op,"\n%s SysTab#%d , Reg#%d ",token, c , op2 );
            }
          }
          else if(op2 < 0 && op1 >= 0) {
            
            c = checkEntry(token2, table_index);
            if(c < 0) {
              printf("Not definded variable");
            } else {
              fprintf(op,"\n%s Reg#%d , SysTab#%d ",token, op1 , c );
            } 
          }
          else if(op1>=0 && op2>=0) {
            fprintf(op,"\n%s Reg#%d , Reg#%d ",token, op1 , op2 );
          }
         
          }
      }
    }
    fclose(ip);
    fclose(op);
    op = fopen(immediate_output,"r");
    printf("\n\nImmediate Code from the file created:");
    while ( fgets ( line, sizeof line, op ) != NULL )
      printf("%s", line);
  } else {
    perror(input);
  }
  return 0;
}
