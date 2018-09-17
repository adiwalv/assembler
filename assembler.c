#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
struct symtab{
  int sym_table_index;
  char name[150];
  int size;
  int no_of_items;
  char defined;
  char type;
  char value[100];
  int address;
  int literal_table_link;
}symtable[100];

struct literaltab{
  int lit_table_index;
  char value[100];
  int address;
}littab[100];

struct errorTable{
  int address;
  int errorType;
  int symTab_index;
}errors[100];

char errorTypes[2][50] = {"Variable not defined!","Multiple definition of Variable"};

char* convertStringToHex(char* str) {
  unsigned long i;
  char *strH = (char*)malloc(sizeof(char)*100);
  //printf("%s",*str);
  memset(strH,0,strlen(strH));
  for(i = 0; i < strlen(str); i++) {
    sprintf((char*)strH+i*2,"%02X",str[i]);
  }
  //printf("Hex is %s",strH);
  return strH;
}

char* convertDecToHex(int decimalNumber) {
  long int quotient;
	int i=1,temp;
	char *hexadecimalNumber = (char*)malloc(sizeof(char)*100);
	quotient = decimalNumber;
	while(quotient!=0) {
		temp = quotient % 16;
		//To convert integer into character
		if( temp < 10)
		           temp =temp + 48; else
		         temp = temp + 55;
		hexadecimalNumber[i++]= temp;
		quotient = quotient / 16;
	}
        return hexadecimalNumber;
}

char* extract_quoted_string(char *substring, char *token1) {
  substring[0] = '\0';
  char *start = strchr(token1, '"') + 1;
  strncat(substring, start, strcspn(start, "\""));
  return substring;
}

int checkEntry(char *s, int sym_table_index) {
  int i = 0;
  for(i = 0; i < sym_table_index; i++){
    if(strcmp(symtable[i].name,s) == 0) {
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

void fetchSection(char* section, FILE **fp, char *token, int *address){
  char line[50];
  while(fgets(line, sizeof line, *fp ) != NULL ) {
          token = strtok(line,"\n\t\r ");
          if(strcmp(token,"section") == 0) {
            token = strtok(NULL,"\n\t\r ");
            if(strcmp(token,section) == 0) {
              break;
            }	
          }
          (*address)++;
        }
}

int populateLiteralTable(int sym_table_index){
  int index = 0 , lit_table_index = 1;
  long array[200];
  char *str;
  long a;
  for(int i = 0; i < sym_table_index; i++) {
    a = strtol(symtable[i].name, &str, 10);
    if(a != 0 ){
      symtable[i].type = 'I';
      symtable[i].defined = 'd';
      array[index] = a;
      index++;
      array[index] = i;
      index++;
    }
  }

  
  for(int i = 0; i < index ; i=i+2) {
    littab[lit_table_index].lit_table_index = lit_table_index;
    sprintf(str,"%X",(unsigned int)array[i]);
    strcpy(littab[lit_table_index].value,str);
    littab[lit_table_index].address = array[i+1];
    symtable[array[i+1]].literal_table_link = lit_table_index;
    lit_table_index++;
    }
  return lit_table_index;  
}

int main(int argc, char *argv[]) {
  if( argc == 2 ) {
    char line[150];
    char *token = (char*)malloc(sizeof(char) * 100);
    char *token1 = (char*)malloc(sizeof(char) * 100);
    char *token2 = (char*)malloc(sizeof(char) * 100);
    char *token3 = (char*)malloc(sizeof(char) * 100);
    int outer, address = 0, sym_table_index=1, count=0, error_table_index = 0, lit_table_index = 0, check;
    int op1,op2;
    //static const char input[] = "program.asm";
    static const char immediate_output[] = "immediate.asm";
    
    FILE *ip = fopen(argv[1],"r");
    FILE *op = fopen(immediate_output,"w");
    if(ip!=NULL){
      fetchSection(".data",&ip,token,&address);
      while(fgets(line, sizeof line, ip) != NULL) {     
        token = strtok(line,"\n\t\r ");
        if(strcmp(token,"section") == 0)
          break;
        check = checkEntry(token,sym_table_index); // checks for entries in the symtable, returns -1 if no entry and symtable index if entry exists
        if (check < 0) {
          strcpy(symtable[sym_table_index].name,token);
          token = strtok(NULL, "\n\t\r ");
          while(token) {
            if(strcmp(token,"dd") == 0) {
              token = strtok(NULL,"\n\t\r ");
              count = 0;
              strcpy(symtable[sym_table_index].value,token);
              symtable[sym_table_index].sym_table_index = sym_table_index;
              token1 = strtok(token,",");
              while(token1) {
                token1 = strtok(NULL,",");
                count++;
              }
              symtable[sym_table_index].no_of_items = count;
              symtable[sym_table_index].size = 4 * symtable[sym_table_index].no_of_items;
              symtable[sym_table_index].address = symtable[sym_table_index-1].address + symtable[sym_table_index-1].size;
              symtable[sym_table_index].defined = 'd';
              symtable[sym_table_index].type = 's';
            } else if(strcmp(token,"db") == 0) {
              count = 0;
              token = strtok(NULL,"\n\t\r ");
              token1 = strtok(token,",");
              char *substring = (char*)malloc(sizeof(char) * 100);
              substring = extract_quoted_string(substring,token1);
              count = strlen(substring);
              strcpy(symtable[sym_table_index].value,substring);
              token1 = strtok(NULL,","); 
              while(token1) {
                if(strcmp(token1,"10") == 0) {
                  strcpy(substring,"\\n");
                }
                if(strcmp(token1,"0") == 0) {
                  strcpy(substring,"\\0");
                }
                strcat(symtable[sym_table_index].value,substring);
                token1 = strtok(NULL,",");
                count++;
              }
              free(substring);
              symtable[sym_table_index].no_of_items = count;
              symtable[sym_table_index].size = 1 * symtable[sym_table_index].no_of_items;
              symtable[sym_table_index].address = symtable[sym_table_index-1].address + symtable[sym_table_index-1].size;
              symtable[sym_table_index].defined = 'd';
              symtable[sym_table_index].type = 's';
              symtable[sym_table_index].sym_table_index = sym_table_index;
            }
            token = strtok(NULL,"\n\t\r");
          }
        } else {   
          strcpy(symtable[sym_table_index].name,token);
          token = strtok(NULL, "\n\t\r ");
          while(token) {
            if(strcmp(token,"dd") == 0) {
              token = strtok(NULL,"\n\t\r ");
              count = 0;
              strcpy(symtable[sym_table_index].value,token);
              symtable[sym_table_index].sym_table_index = sym_table_index;
              token1 = strtok(token,",");
              while(token1) {
                token1 = strtok(NULL,",");
                count++;
              }
              symtable[sym_table_index].no_of_items = count;
              symtable[sym_table_index].size = 4 * symtable[sym_table_index].no_of_items;
              symtable[sym_table_index].address = symtable[sym_table_index-1].address + symtable[sym_table_index-1].size;
              symtable[sym_table_index].defined = 'm';
              errors[error_table_index].address = address+2;
              errors[error_table_index].errorType = 1;
              errors[error_table_index].symTab_index = sym_table_index;;
              error_table_index++;
              symtable[sym_table_index].type = 's';
            } else if(strcmp(token,"db") == 0) {
              count = 0;
              token = strtok(NULL,"\n\t\r ");
              strcpy(symtable[sym_table_index].value,token);
              token1 = strtok(token,",");
              count = strlen(token) - 3;
              while(token1) {
                token1 = strtok(NULL,",");
                count++;
              }
              symtable[sym_table_index].no_of_items = count;
              symtable[sym_table_index].size = 1 * symtable[sym_table_index].no_of_items;
              symtable[sym_table_index].address = symtable[sym_table_index-1].address + symtable[sym_table_index-1].size;
              symtable[sym_table_index].defined = 'm';
              errors[error_table_index].address = address+2;
              errors[error_table_index].errorType = 1;
              errors[error_table_index].symTab_index = sym_table_index;;
              error_table_index++;
              symtable[sym_table_index].type = 's';
              symtable[sym_table_index].sym_table_index = sym_table_index;
            }
            token = strtok(NULL,"\n\t\r");
          }
        }
        address++;
        sym_table_index++;
      }
      rewind(ip);
      address=0;
      fetchSection(".bss",&ip,token,&address);
      while(fgets(line, sizeof line, ip) != NULL) {
        token = strtok(line,"\n\t\r ");
        if(strcmp(token,"section") == 0)
          break;
        check = checkEntry(token,sym_table_index);
        if (check < 0) {
          strcpy(symtable[sym_table_index].name,token);
          token = strtok(NULL, "\n\t\r ");
          while(token) {
            if(strcmp(token,"resb") == 0) {
              token = strtok(NULL,"\n\t\r ");
              strcpy(symtable[sym_table_index].value,token);
              symtable[sym_table_index].no_of_items = 0;
              symtable[sym_table_index].sym_table_index = sym_table_index;
              symtable[sym_table_index].size = 1 * atoi(token);
              symtable[sym_table_index].address = symtable[sym_table_index-1].address + symtable[sym_table_index-1].size;
              symtable[sym_table_index].defined = 'd';
              symtable[sym_table_index].type = 's';
            } else if(strcmp(token,"resd") == 0) {
              token = strtok(NULL,"\n\t\r ");
              strcpy(symtable[sym_table_index].value,token);
              symtable[sym_table_index].no_of_items = atoi(token);
              symtable[sym_table_index].sym_table_index = sym_table_index;
              symtable[sym_table_index].size = 4 * atoi(token);
              symtable[sym_table_index].address = symtable[sym_table_index-1].address + symtable[sym_table_index-1].size;
              symtable[sym_table_index].defined = 'd';
              symtable[sym_table_index].type = 's';
            }
            token = strtok(NULL,"\n\t\r");
          }
        }else {
          strcpy(symtable[sym_table_index].name,token);
          token = strtok(NULL, "\n\t\r ");
          while(token) {
            if(strcmp(token,"resb") == 0) {
              token = strtok(NULL,"\n\t\r ");
              strcpy(symtable[sym_table_index].value,token);
              symtable[sym_table_index].no_of_items = 0;
              symtable[sym_table_index].sym_table_index = sym_table_index;
              symtable[sym_table_index].size = 1 * atoi(token);
              symtable[sym_table_index].address = symtable[sym_table_index-1].address + symtable[sym_table_index-1].size;
              symtable[sym_table_index].defined = 'm'; //giving it a flag m for multiple definitions
              errors[error_table_index].address = address+2;
              errors[error_table_index].errorType = 1;
              errors[error_table_index].symTab_index = sym_table_index;;
              error_table_index++;
              symtable[sym_table_index].type = 's';
            } else if(strcmp(token,"resd") == 0) {
              token = strtok(NULL,"\n\t\r ");
              strcpy(symtable[sym_table_index].value,token);
              symtable[sym_table_index].no_of_items = atoi(token);
              symtable[sym_table_index].sym_table_index = sym_table_index;
              symtable[sym_table_index].size = 4 * atoi(token);
              symtable[sym_table_index].address = symtable[sym_table_index-1].address + symtable[sym_table_index-1].size;
              symtable[sym_table_index].defined = 'm';//giving it a flag m for multiple definitions
              errors[error_table_index].address = address+2;
              errors[error_table_index].errorType = 1;
              errors[error_table_index].symTab_index = sym_table_index;;
              error_table_index++;
              symtable[sym_table_index].type = 's';
            }
            token = strtok(NULL,"\n\t\r");
          }
        }
        sym_table_index++;
        address++;
      }
      rewind(ip);
      address = 0;
      fetchSection(".text",&ip,token,&address);
      while(fgets(line, sizeof line, ip) != NULL) {
        address++;
        token = strtok(line,"\n\t\r ");
        if(strcmp(token,"section") == 0)
          break;
        if(strcmp(token,"global") == 0) {
          token = strtok(NULL,"\n\t\r ");
          token1 = strtok(token,",");
          while(token1) {
            strcpy(symtable[sym_table_index].name,token1);
            symtable[sym_table_index].defined = 'u';
            symtable[sym_table_index].address = address;
            symtable[sym_table_index].type = 'l';
            strcpy(symtable[sym_table_index].value,"***");
            symtable[sym_table_index].sym_table_index = sym_table_index;
            sym_table_index++;
            token1 = strtok(NULL,",");
          }
        }
        if (strcmp(&token[strlen(token)-1],":") == 0) {
          token[strlen(token)-1] = '\0';
          check = checkEntry(token,sym_table_index);
          if(check < 0){
            symtable[sym_table_index].sym_table_index = sym_table_index;
            strcpy(symtable[sym_table_index].name,token);
            symtable[sym_table_index].defined = 'd';
            symtable[sym_table_index].type = 'l';
            strcpy(symtable[sym_table_index].value,"***");
            symtable[sym_table_index].address = address;
            sym_table_index++;}
          else {
            symtable[check].defined = 'd';
            symtable[check].address = address;
          }
        }
        if((strcmp(token,"jmp") == 0)||(strcmp(token,"jnz") == 0) || (strcmp(token,"jz") == 0)){
          token = strtok(NULL,"\n\t\r ");
          int check = checkEntry(token,sym_table_index);
          if(check < 0) {
            symtable[sym_table_index].sym_table_index = sym_table_index;
            strcpy(symtable[sym_table_index].name,token);
            symtable[sym_table_index].defined = 'u';
            symtable[sym_table_index].type = 'l';
            strcpy(symtable[sym_table_index].value,"***");
            symtable[sym_table_index].address = address + 1;
            sym_table_index++;
          } 
        }
      }
      rewind(ip);
      address = 0;
      fetchSection(".text",&ip,token,&address);
      while ( fgets ( line, sizeof line, ip ) != NULL )
      {
        token = strtok(line,"\n\t\r ");
        if(strcmp(token,"global") == 0) {
          break;
        }
        address++;
      }
      while ( fgets ( line, sizeof line, ip ) != NULL )
      {
        token = strtok(line,"\n\t\r ");
        if (strcmp(&token[strlen(token)-1],":") == 0) {
          token = strtok(NULL,"\n\t\r ");
          token1 = strtok(NULL,"\n\t\r ");
          token2 = strtok(token1,", ");
          token3 = strtok(NULL,", ");
          if(token3 == NULL) {
            check = registerTable(token2);
            if (check < 0) {
              check = checkEntry(token2, sym_table_index);
              if(check < 0) {
                strcpy(symtable[sym_table_index].name,token2);
                symtable[sym_table_index].defined = 'u';
                symtable[sym_table_index].address = address+3;
                symtable[sym_table_index].type = 'l';
                strcpy(symtable[sym_table_index].value,"***");
                symtable[sym_table_index].sym_table_index = sym_table_index;
                fprintf(op,"\n%s Symtab#%d", token, sym_table_index);
                sym_table_index++;
              } else
                fprintf(op,"\n%s Symtab#%d", token, check);
            } else {
              fprintf(op,"\n%s Reg#%d", token, check);
            }
          } else {
            op1 = registerTable(token2);
            op2 = registerTable(token3);
            if(op1 < 0 && op2 >= 0) {
              check = checkEntry(token2, sym_table_index);
              if(check < 0) {
                strcpy(symtable[sym_table_index].name,token2);
                symtable[sym_table_index].defined = 'u';
                symtable[sym_table_index].address = address+3;
                symtable[sym_table_index].type = 'l';
                strcpy(symtable[sym_table_index].value,"***");
                symtable[sym_table_index].sym_table_index = sym_table_index;
                fprintf(op,"\n%s SymTab#%d , Reg#%d ",token, sym_table_index , op2 );
                sym_table_index++;
              } else {
                fprintf(op,"\n%s SymTab#%d , Reg#%d ",token, check , op2 );
              }
            }
            else if(op2 < 0 && op1 >= 0) {
              check = checkEntry(token3, sym_table_index);
              if(check < 0) {
                strcpy(symtable[sym_table_index].name,token3);
                symtable[sym_table_index].defined = 'u';
                symtable[sym_table_index].address = address+3;
                symtable[sym_table_index].type = 'l';
                strcpy(symtable[sym_table_index].value,"***");
                symtable[sym_table_index].sym_table_index = sym_table_index;
                fprintf(op,"\n%s Reg#%d , SymTab#%d ",token, op1, symtable[sym_table_index].sym_table_index);
                sym_table_index++;
              }
              else {
                fprintf(op,"\n%s Reg#%d , SymTab#%d ",token, op1 , check );
              }
            }
            else if(op1>=0 && op2>=0) {
              fprintf(op,"\n%s Reg#%d , Reg#%d ",token, op1 , op2 );
            }
            else {
              check = checkEntry(token2, sym_table_index);
              if( check< 0) {
                strcpy(symtable[sym_table_index].name,token3);
                symtable[sym_table_index].defined = 'u';
                symtable[sym_table_index].address = address+3;
                symtable[sym_table_index].type = 'l';
                strcpy(symtable[sym_table_index].value,"***");
                symtable[sym_table_index].sym_table_index = sym_table_index;
                sym_table_index++;
              }
              check = checkEntry(token1, sym_table_index);
              if(check < 0) {
                strcpy(symtable[sym_table_index].name,token2);
                symtable[sym_table_index].defined = 'u';
                symtable[sym_table_index].address = address+3;
                symtable[sym_table_index].type = 'l';
                strcpy(symtable[sym_table_index].value,"***");
                symtable[sym_table_index].sym_table_index = sym_table_index;
                sym_table_index++;
              }
              fprintf(op,"\n%s SymTab#%d , Symtab#%d ",token, checkEntry(token2,sym_table_index), checkEntry(token3,sym_table_index));
              
            } // memory to memory not allowed but will require this to fill symtable
          }
        }
        else {
          token1 = strtok(NULL,"\n\t\r ,");
          token2 = strtok(NULL,"\n\t\r ,");
          if(token2 == NULL) {
            check = registerTable(token1);
            if (check < 0) {
              check = checkEntry(token1, sym_table_index);
              if(check < 0) {
                strcpy(symtable[sym_table_index].name,token1);
                symtable[sym_table_index].defined = 'u';
                symtable[sym_table_index].address = address + 3;
                symtable[sym_table_index].type = 'l';
                strcpy(symtable[sym_table_index].value,"***");
                symtable[sym_table_index].sym_table_index = sym_table_index;
                sym_table_index++; 
                
                fprintf(op,"\n%s Symtab#%d", token, sym_table_index);
              } else {
                fprintf(op,"\n%s Symtab#%d", token, check);
              }
            } else {
              fprintf(op,"\n%s Reg#%d", token, check);
            }
          } else {
            op1 = registerTable(token1);
            op2 = registerTable(token2);
            if(op1 < 0 && op2 >= 0) {
              check = checkEntry(token1, sym_table_index);           
              if(check < 0) {
                strcpy(symtable[sym_table_index].name,token1);
                symtable[sym_table_index].defined = 'u';
                symtable[sym_table_index].address = address+3;
                symtable[sym_table_index].type = 'l';
                strcpy(symtable[sym_table_index].value,"***");
                symtable[sym_table_index].sym_table_index = sym_table_index;
                fprintf(op,"\n%s SymTab#%d , Reg#%d ",token, sym_table_index , op2 );
                sym_table_index++;
              } else {
                fprintf(op,"\n%s Symtab#%d , Reg#%d ",token, check , op2 );
              }
            }
            else if(op2 < 0 && op1 >= 0) {
              check = checkEntry(token2, sym_table_index);
              if(check < 0) {
                strcpy(symtable[sym_table_index].name,token2);
                symtable[sym_table_index].defined = 'u';
                symtable[sym_table_index].address = address+3;
                symtable[sym_table_index].type = 'l';
                strcpy(symtable[sym_table_index].value,"***");
                symtable[sym_table_index].sym_table_index = sym_table_index;
                fprintf(op,"\n%s Reg#%d , SymTab#%d ",token, op1, symtable[sym_table_index].sym_table_index);
                sym_table_index++;
              } else {
                fprintf(op,"\n%s Reg#%d , SymTab#%d ",token, op1 , check );
              }
            }
            else if(op1>=0 && op2>=0) {
              fprintf(op,"\n%s Reg#%d , Reg#%d ",token, op1 , op2 );
            }
            else {
              check = checkEntry(token2, sym_table_index);
              if( check< 0) {
                strcpy(symtable[sym_table_index].name,token2);
                symtable[sym_table_index].defined = 'u';
                symtable[sym_table_index].address = address+3;
                symtable[sym_table_index].type = 'l';
                strcpy(symtable[sym_table_index].value,"***");
                symtable[sym_table_index].sym_table_index = sym_table_index;
                sym_table_index++;
              }
              check = checkEntry(token1, sym_table_index);
              if(check < 0) {
                strcpy(symtable[sym_table_index].name,token1);
                symtable[sym_table_index].defined = 'u';
                symtable[sym_table_index].address = address+3;
                symtable[sym_table_index].type = 'l';
                strcpy(symtable[sym_table_index].value,"***");
                symtable[sym_table_index].sym_table_index = sym_table_index;
                sym_table_index++;
              }
              fprintf(op,"\n%s SymTab#%d , Symtab#%d ",token, checkEntry(token1,sym_table_index), checkEntry(token2,sym_table_index)); 
            }
          }
        }
        address++;
      }
        fclose(op);
        op = fopen(immediate_output,"r");
        //printf("%s",errorTypes[errors[0].errorType]);
        rewind(ip);
        lit_table_index = populateLiteralTable(sym_table_index);
        printf("\tProgram:\n");
        while ( fgets ( line, sizeof line, ip ) != NULL )
          printf("%s",line);
        printf("\n\tSym Table:\n");
        printf("%12s%12s%12s%12s%12s%12s%30s%10s%18s\n","Table Index","Name","Size","No of items","Type","Defined","Value","Address","Littab Entry");
        for(outer = 1; outer < sym_table_index; outer++) {
          printf("%12d%12s%12d%12d%12c%12c%30s%10d%18d\n",symtable[outer].sym_table_index,symtable[outer].name,symtable[outer].size,symtable[outer].no_of_items,symtable[outer].type,symtable[outer].defined,symtable[outer].value,symtable[outer].address,symtable[outer].literal_table_link);  
        }
        
             printf("\n\tLiteral Table:\n");
        printf("%12s%12s%12s\n","Table Index","Value","Address");
        for(outer = 1; outer < lit_table_index; outer++) {
          printf("%12d%12s%12d\n",littab[outer].lit_table_index,littab[outer].value,littab[outer].address);
          }
        printf("\n\nImmediate Code from the file created:");
        while ( fgets ( line, sizeof line, op ) != NULL )
          printf("%s", line);
        
        for(outer = 0; outer < sym_table_index; outer++) {
          if(symtable[outer].defined == 'u') {
            errors[error_table_index].address = symtable[outer].address;
            errors[error_table_index].errorType = 0;
            errors[error_table_index].symTab_index = outer;
            error_table_index++;
          }
        }
        printf("\n\nErrors:");
        for(outer = 0; outer < error_table_index; outer++) {
          printf("\nLine %d : %s %s",errors[outer].address,symtable[errors[outer].symTab_index].name,errorTypes[errors[outer].errorType]);
          
        }
        fclose(ip);
    } else {
      perror(argv[1]);
    }
    return 0;
  }
  else if( argc > 2 ) {
    printf("At the moment this assembler can only work with one .asm source file at a time...\n");
  }
  else {
    printf("One argument expected.\n");
  }
  return 0;
}
