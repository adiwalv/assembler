#include "print.h"

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
  if(strcmp(s,"esp") == 0)
    return 6;
  if(strcmp(s,"ebp") == 0)
    return 7;
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


char* makeLittleEndian(char *str){
  char* str5 = (char*)malloc(sizeof(char)*10);
  memset(str5,0,10);
  str5[0] = str[6];
  str5[1] = str[7];
  str5[2] = str[4];
  str5[3] = str[5];
  str5[4] = str[2];
  str5[5] = str[3];
   str5[6] = str[0];
  str5[7] = str[1];
  return str5;
}

int populateLiteralTable(int sym_table_index, int lit_table_index){
  int index = 0 ;
  long array[200];
  char *str4 = (char*)malloc(sizeof(char)*8);
  memset(str4,0,8);
  for(int i = 0; i < sym_table_index; i++) {
    a = atoi(symtable[i].name);
    if (strcmp(symtable[i].name,"0") == 0){
      symtable[i].type = 'I';
      symtable[i].defined = 'd';
      array[index] = a;
      index++;
      array[index] = i;
      index++;
    }
    if(a != (long)NULL ){
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
    sprintf(str4,"%08X",(unsigned int)array[i]);
    str4 = makeLittleEndian(str4);
    //printf("%s\n",str4);
    strcpy(littab[lit_table_index].value,str4);
    littab[lit_table_index].sym_table_index = array[i+1];
    symtable[array[i+1]].literal_table_link = lit_table_index;
    lit_table_index++;
    }
  return lit_table_index;
  free(str4);
}


void generateTables(char *filename){
  char *token = (char*)malloc(sizeof(char) * 100);
    char *token1 = (char*)malloc(sizeof(char) * 100);
    char *token2 = (char*)malloc(sizeof(char) * 100);
    char *token3 = (char*)malloc(sizeof(char) * 100);
    ip = fopen(filename,"r");
    op = fopen(immediate_output,"w");
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
              char *hex = (char*)malloc(sizeof(char)*10);
              token1 = strtok(token,",");
              sprintf(hex,"%08X",atoi(token1));
              hex = makeLittleEndian(hex);
              littab[lit_table_index].lit_table_index = lit_table_index;
              symtable[sym_table_index].literal_table_link = lit_table_index;
              strcpy(littab[lit_table_index].value,hex);
              littab[lit_table_index].sym_table_index = sym_table_index;
              while(token1) {
                token1 = strtok(NULL,",");
                if(token1 != NULL) {
                sprintf(hex,"%08X",atoi(token1));
                hex = makeLittleEndian(hex);
                strcat(littab[lit_table_index].value,hex);
                }
                count++;
              }
              lit_table_index++;
              symtable[sym_table_index].no_of_items = count;
              symtable[sym_table_index].size = 4 * symtable[sym_table_index].no_of_items;
              symtable[sym_table_index].address = symtable[sym_table_index-1].address + symtable[sym_table_index-1].size;
              //char hexAdd[10];
              //int i = sprintf(hexAdd,"%08X",symtable[sym_table_index].address);
              //printf("%s,--->",hexAdd);
              //symtable[sym_table_index].address =
              //printf("%d,-->",symtable[sym_table_index].address);
              symtable[sym_table_index].defined = 'd';
              symtable[sym_table_index].type = 's';
            } else if(strcmp(token,"db") == 0) { 
              count = 0;
               char* substring = (char*)malloc(sizeof(char)*100);
              token = strtok(NULL,",");
              while(token) {
              if(token[0] == '\"') {
                substring = extract_quoted_string(substring,token);
                //printf("%s",substring);
                count+= strlen(substring);
                //printf("count: %d",count);
                strcat(symtable[sym_table_index].value,substring);
                littab[lit_table_index].lit_table_index = lit_table_index;
                symtable[sym_table_index].literal_table_link = lit_table_index;
                strcat(littab[lit_table_index].value,convertStringToHex(substring));
                littab[lit_table_index].sym_table_index = sym_table_index;
              }
              if(strcmp(token,"10") == 0) {
                  strcat(littab[lit_table_index].value,"0A");
              }
              if(*token == '0') {
                  strcat(littab[lit_table_index].value,"00");
              }
              token = strtok(NULL,",");
              count++;
              }
              lit_table_index++;
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
              char *hex = (char*)malloc(sizeof(char)*8);
              sprintf(hex,"%08X",atoi(token1));
              hex = makeLittleEndian(hex);
              littab[lit_table_index].lit_table_index = lit_table_index;
              symtable[sym_table_index].literal_table_link = lit_table_index;
              strcpy(littab[lit_table_index].value,hex);
              littab[lit_table_index].sym_table_index = sym_table_index;
              while(token1) {
                token1 = strtok(NULL,",");
                if(token1 != NULL) {
                sprintf(hex,"%08X",atoi(token1));
                hex = makeLittleEndian(hex);
                strcat(littab[lit_table_index].value,hex);
                }
                count++;
              }
              lit_table_index++;              
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
               char* substring = (char*)malloc(sizeof(char)*100);
              token = strtok(NULL,",");
              while(token) {
              if(token[0] == '\"') {
                substring = extract_quoted_string(substring,token);
                //printf("%s",substring);
                count+= strlen(substring);
                //printf("count: %d",count);
                strcat(symtable[sym_table_index].value,substring);
                littab[lit_table_index].lit_table_index = lit_table_index;
                symtable[sym_table_index].literal_table_link = lit_table_index;
                strcat(littab[lit_table_index].value,convertStringToHex(substring));
                littab[lit_table_index].sym_table_index = sym_table_index;
              }
              if(strcmp(token,"10") == 0) {
                  strcat(littab[lit_table_index].value,"0A");
               
              }
              if(*token == '0') {
                  strcat(littab[lit_table_index].value,"00");
                }
              
              token = strtok(NULL,",");
              count++;
              }free(substring);
              lit_table_index++;
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
              char *hex = (char*)malloc(sizeof(char)*8);
              sprintf(hex,"%08X",atoi(token));
              hex = makeLittleEndian(hex);
              littab[lit_table_index].lit_table_index = lit_table_index;
              symtable[sym_table_index].literal_table_link = lit_table_index;
              strcpy(littab[lit_table_index].value,hex);
              littab[lit_table_index].sym_table_index = sym_table_index;
              lit_table_index++;
              strcpy(symtable[sym_table_index].value,token);
              symtable[sym_table_index].no_of_items = 0;
              symtable[sym_table_index].sym_table_index = sym_table_index;
              symtable[sym_table_index].size = 1 * atoi(token);
              symtable[sym_table_index].address = symtable[sym_table_index-1].address + symtable[sym_table_index-1].size;
              symtable[sym_table_index].defined = 'd';
              symtable[sym_table_index].type = 's';
            } else if(strcmp(token,"resd") == 0) {
              token = strtok(NULL,"\n\t\r ");
              char *hex = (char*)malloc(sizeof(char)*8);
              sprintf(hex,"%08X",atoi(token));
              hex = makeLittleEndian(hex);
              littab[lit_table_index].lit_table_index = lit_table_index;
              symtable[sym_table_index].literal_table_link = lit_table_index;
              strcpy(littab[lit_table_index].value,hex);
              littab[lit_table_index].sym_table_index = sym_table_index;
              lit_table_index++;
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
              char *hex = (char*)malloc(sizeof(char)*8);
              sprintf(hex,"%08X",atoi(token));
              hex = makeLittleEndian(hex);
              littab[lit_table_index].lit_table_index = lit_table_index;
              symtable[sym_table_index].literal_table_link = lit_table_index;
              strcpy(littab[lit_table_index].value,hex);
              littab[lit_table_index].sym_table_index = sym_table_index;
              lit_table_index++;
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
              
              char *hex = (char*)malloc(sizeof(char)*8);
              sprintf(hex,"%08X",atoi(token));
              hex = makeLittleEndian(hex);
              littab[lit_table_index].lit_table_index = lit_table_index;
              symtable[sym_table_index].literal_table_link = lit_table_index;
              strcpy(littab[lit_table_index].value,hex);
              littab[lit_table_index].sym_table_index = sym_table_index;
              lit_table_index++;
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
            symtable[sym_table_index].address = address + 1;
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
            symtable[sym_table_index].address = address + 1 ;
            sym_table_index++;}
          else {
            symtable[check].defined = 'd';
            symtable[check].address = address + 1;
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
                a = atoi(token2);
                if (a != (long)NULL || *token2 == '0') {
                  char *str4 = (char*)malloc(sizeof(char) * 10);
                  littab[lit_table_index].lit_table_index = lit_table_index;
                  sprintf(str4,"%08X",(unsigned int)a);
                  str4 = makeLittleEndian(str4);
                  //printf("Helloooo%s\n",str4);
                  strcpy(littab[lit_table_index].value,str4);
                  littab[lit_table_index].sym_table_index = -1;
                  fprintf(op,"%s Littab#%d\n", token, lit_table_index);  
                  lit_table_index++;
                } else {
                strcpy(symtable[sym_table_index].name,token2);
                symtable[sym_table_index].defined = 'u';
                symtable[sym_table_index].address = address+3;
                symtable[sym_table_index].type = 'l';
                //printf("Hellooooaaa\n");
                strcpy(symtable[sym_table_index].value,"***");
                symtable[sym_table_index].sym_table_index = sym_table_index;
                fprintf(op,"%s Symtab#%d\n", token, sym_table_index);
                                sym_table_index++;
                }
              } else
                fprintf(op,"%s Symtab#%d\n", token, check);
            } else {
              fprintf(op,"%s Reg#%d\n", token, check);
            }
          } else {
            op1 = registerTable(token2);
            op2 = registerTable(token3);
            if(op1 < 0 && op2 >= 0) {
              check = checkEntry(token2, sym_table_index);
              if(check < 0) {
                a = atoi(token2);
                if (a != (long)NULL || *token2 == '0') {
                  char *str4 = (char*)malloc(sizeof(char) * 10);
                  littab[lit_table_index].lit_table_index = lit_table_index;
                  sprintf(str4,"%08X",(unsigned int)a);
                  str4 = makeLittleEndian(str4);
                  //printf("Helloooo%s\n",str4);
                  strcpy(littab[lit_table_index].value,str4);
                  littab[lit_table_index].sym_table_index = -1;
                  fprintf(op,"%s Littab#%d , Reg#%d\n", token, lit_table_index, op2);  
                  lit_table_index++;
                } else {
                strcpy(symtable[sym_table_index].name,token2);
                symtable[sym_table_index].defined = 'u';
                symtable[sym_table_index].address = address+3;
                symtable[sym_table_index].type = 'l';
                strcpy(symtable[sym_table_index].value,"***");
                symtable[sym_table_index].sym_table_index = sym_table_index;
                fprintf(op,"%s SymTab#%d , Reg#%d\n",token, sym_table_index , op2 );
                sym_table_index++;
                }
              } else {
                fprintf(op,"%s SymTab#%d , Reg#%d\n",token, check , op2 );
              }
            }
            else if(op2 < 0 && op1 >= 0) {
              check = checkEntry(token3, sym_table_index);
              if(check < 0) {
                a = atoi(token3);
                if (a != (long)NULL || *token3 == '0') {
                  char *str4 = (char*)malloc(sizeof(char) * 10);
                  littab[lit_table_index].lit_table_index = lit_table_index;
                  sprintf(str4,"%08X",(unsigned int)a);
                  str4 = makeLittleEndian(str4);
                  //printf("Helloooo%s\n",str4);
                  strcpy(littab[lit_table_index].value,str4);
                  littab[lit_table_index].sym_table_index = -1;
                  fprintf(op,"%s Reg#%d , Littab#%d\n", token, op1, lit_table_index);  
                  lit_table_index++;
                } else {
                strcpy(symtable[sym_table_index].name,token3);
                symtable[sym_table_index].defined = 'u';
                symtable[sym_table_index].address = address+3;
                symtable[sym_table_index].type = 'l';
                strcpy(symtable[sym_table_index].value,"***");
                symtable[sym_table_index].sym_table_index = sym_table_index;
                fprintf(op,"%s Reg#%d , SymTab#%d\n",token, op1, symtable[sym_table_index].sym_table_index);
                sym_table_index++;
                }
              }
              else {
                fprintf(op,"%s Reg#%d , SymTab#%d\n",token, op1 , check );
              }
            }
            else if(op1>=0 && op2>=0) {
              fprintf(op,"%s Reg#%d , Reg#%d\n",token, op1 , op2 );
            }
            else {
              check = checkEntry(token2, sym_table_index);
              check2 = checkEntry(token3 , sym_table_index);
              if( check < 0) {
                a = atoi(token2);
                if (a != (long)NULL || *token2 == '0') {
                  char *str4 = (char*)malloc(sizeof(char) * 10);
                  littab[lit_table_index].lit_table_index = lit_table_index;
                  entry1 = lit_table_index;
                  sprintf(str4,"%08X",(unsigned int)a);
                  str4 = makeLittleEndian(str4);
                  //printf("Helloooo%s\n",str4);
                  strcpy(littab[lit_table_index].value,str4);
                  littab[lit_table_index].sym_table_index = -1;
                  //fprintf(op,"\n%s Littab#%d , Symtab#%d", token, lit_table_index, checkEntry(token3, sym_table_index));  
                  lit_table_index++;
                } else {
                strcpy(symtable[sym_table_index].name,token2);
                symtable[sym_table_index].defined = 'u';
                symtable[sym_table_index].address = address+3;
                symtable[sym_table_index].type = 'l';
                strcpy(symtable[sym_table_index].value,"***");
                symtable[sym_table_index].sym_table_index = sym_table_index;
                sym_table_index++;
                }
              }
              check = checkEntry(token3, sym_table_index);
              if(check < 0) {
                a = atoi(token3);
                if (a != (long)NULL || *token3 == '0') {
                  char *str4 = (char*)malloc(sizeof(char) * 10);
                  littab[lit_table_index].lit_table_index = lit_table_index;
                  entry2 = lit_table_index;
                  sprintf(str4,"%08X",(unsigned int)a);
                  str4 = makeLittleEndian(str4);
                  //printf("Helloooo%s\n",str4);
                  strcpy(littab[lit_table_index].value,str4);
                  littab[lit_table_index].sym_table_index = -1;
                  //fprintf(op,"\n%s Symtab#%d , Littab#%d", token, checkEntry(token2, sym_table_index), lit_table_index);  
                  lit_table_index++;
                  // goto k;
                } else {
                strcpy(symtable[sym_table_index].name,token3);
                symtable[sym_table_index].defined = 'u';
                symtable[sym_table_index].address = address+3;
                symtable[sym_table_index].type = 'l';
                strcpy(symtable[sym_table_index].value,"***");
                symtable[sym_table_index].sym_table_index = sym_table_index;
                sym_table_index++;
                }
              }
              a = atoi(token2);
              b = atoi(token3);
              if((a != (long)NULL || *token2 == '0') && (b != (long)NULL || *token3 == '0')) {
                fprintf(op,"%s Littab#%d , Littab#%d\n", token, entry1, entry2);  
              }
              else if((a != (long)NULL || *token2 == '0') && (b == (long)NULL || *token3 != '0')) {
                fprintf(op,"%s Littab#%d , Symtab#%d\n", token, entry1, checkEntry(token3, sym_table_index));  
              }
              else if((a == (long)NULL || *token2 != '0') && (b != (long)NULL || *token3 == '0')) {
                fprintf(op,"%s Symtab#%d , Littab#%d\n", token, checkEntry(token2, sym_table_index), entry2);  
              }
              else if((a == (long)NULL || *token2 != '0') && (b == (long)NULL || *token3 != '0')) {
                fprintf(op,"%s Symtab#%d , Symtab#%d\n", token, checkEntry(token2, sym_table_index), checkEntry(token3, sym_table_index));  
              }
                  //   k: continue;
            }
            // memory to memory not allowed but will require this to fill symtable
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
                a = atoi(token1);
                if (a != (long)NULL || *token1 == '0') {
                  char *str4 = (char*)malloc(sizeof(char) * 10);
                  littab[lit_table_index].lit_table_index = lit_table_index;
                  sprintf(str4,"%08X",(unsigned int)a);
                  str4 = makeLittleEndian(str4);
                  //printf("Helloooo%s\n",str4);
                  strcpy(littab[lit_table_index].value,str4);
                  littab[lit_table_index].sym_table_index = -1;
                  fprintf(op,"%s Littab#%d\n", token, lit_table_index);  
                  lit_table_index++;
                } else {
                strcpy(symtable[sym_table_index].name,token1);
                symtable[sym_table_index].defined = 'u';
                symtable[sym_table_index].address = address + 3;
                symtable[sym_table_index].type = 'l';
                strcpy(symtable[sym_table_index].value,"***");
                symtable[sym_table_index].sym_table_index = sym_table_index;
                sym_table_index++; 
                fprintf(op,"%s Symtab#%d\n", token, sym_table_index);
                }
              } else {
                fprintf(op,"%s Symtab#%d\n", token, check);
              }
            } else {
              fprintf(op,"%s Reg#%d\n", token, check);
            }
          } else {
            op1 = registerTable(token1);
            op2 = registerTable(token2);
            if(op1 < 0 && op2 >= 0) {
              check = checkEntry(token1, sym_table_index);           
              if(check < 0) {
                a = atoi(token1);
                if (a != (long)NULL || *token1 == '0') {
                  char *str4 = (char*)malloc(sizeof(char) * 10);
                  littab[lit_table_index].lit_table_index = lit_table_index;
                  sprintf(str4,"%08X",(unsigned int)a);
                  str4 = makeLittleEndian(str4);
                  //printf("Helloooo%s\n",str4);
                  strcpy(littab[lit_table_index].value,str4);
                  littab[lit_table_index].sym_table_index = -1;
                  fprintf(op,"%s Littab#%d , Reg#%d\n", token, lit_table_index, op2);  
                  lit_table_index++;
                } else {  
                strcpy(symtable[sym_table_index].name,token1);
                symtable[sym_table_index].defined = 'u';
                symtable[sym_table_index].address = address+3;
                symtable[sym_table_index].type = 'l';
                strcpy(symtable[sym_table_index].value,"***");
                symtable[sym_table_index].sym_table_index = sym_table_index;
                fprintf(op,"%s SymTab#%d , Reg#%d\n",token, sym_table_index , op2 );
                sym_table_index++;
                }
              } else {
                fprintf(op,"%s Symtab#%d , Reg#%d\n",token, check , op2 );
              }
            }
            else if(op2 < 0 && op1 >= 0) {
              check = checkEntry(token2, sym_table_index);
              if(check < 0) {
                                a = atoi(token2);
                if (a != (long)NULL || *token2 == '0') {
                  char *str4 = (char*)malloc(sizeof(char) * 10);
                  littab[lit_table_index].lit_table_index = lit_table_index;
                  sprintf(str4,"%08X",(unsigned int)a);
                  str4 = makeLittleEndian(str4);
                  //printf("Helloooo%s\n",str4);
                  strcpy(littab[lit_table_index].value,str4);
                  littab[lit_table_index].sym_table_index = -1;
                  fprintf(op,"%s Reg#%d , Littab#%d\n", token, op1, lit_table_index);  
                  lit_table_index++;
                } else {
                strcpy(symtable[sym_table_index].name,token2);
                symtable[sym_table_index].defined = 'u';
                symtable[sym_table_index].address = address+3;
                symtable[sym_table_index].type = 'l';
                strcpy(symtable[sym_table_index].value,"***");
                symtable[sym_table_index].sym_table_index = sym_table_index;
                fprintf(op,"%s Reg#%d , SymTab#%d\n",token, op1, symtable[sym_table_index].sym_table_index);
                sym_table_index++;
                }
              } else {
                fprintf(op,"%s Reg#%d , SymTab#%d\n",token, op1 , check );
              }
            }
            else if(op1>=0 && op2>=0) {
              fprintf(op,"%s Reg#%d , Reg#%d\n",token, op1 , op2 );
            }
            else {
              check = checkEntry(token2, sym_table_index);
              if( check < 0) {
                a = atoi(token2);
                if (a != (long)NULL || *token2 == '0') {
                  char *str4 = (char*)malloc(sizeof(char) * 10);
                  littab[lit_table_index].lit_table_index = lit_table_index;
                  entry1 = lit_table_index;
                  sprintf(str4,"%08X",(unsigned int)a);
                  str4 = makeLittleEndian(str4);
                  //printf("Helloooo%s\n",str4);
                  strcpy(littab[lit_table_index].value,str4);
                  littab[lit_table_index].sym_table_index = -1;
                  // fprintf(op,"\n%s Symtab#%d , Littab#%d", token, checkEntry(token1, sym_table_index), lit_table_index);  
                  lit_table_index++;
                } else {
                strcpy(symtable[sym_table_index].name,token2);
                symtable[sym_table_index].defined = 'u';
                symtable[sym_table_index].address = address+3;
                symtable[sym_table_index].type = 'l';
                strcpy(symtable[sym_table_index].value,"***");
                symtable[sym_table_index].sym_table_index = sym_table_index;
                sym_table_index++;
                }
              }
              check = checkEntry(token1, sym_table_index);
              if(check < 0) {
                 a = atoi(token1);
                if (a != (long)NULL || *token1 == '0') {
                  char *str4 = (char*)malloc(sizeof(char) * 10);
                  littab[lit_table_index].lit_table_index = lit_table_index;
                  entry2 = lit_table_index;
                  sprintf(str4,"%08X",(unsigned int)a);
                  str4 = makeLittleEndian(str4);
                  //printf("Helloooo%s\n",str4);
                  strcpy(littab[lit_table_index].value,str4);
                  littab[lit_table_index].sym_table_index = -1;
                  //                  fprintf(op,"\n%s Littab#%d , Symtab#%d", token, lit_table_index, checkEntry(token2, sym_table_index));
                  lit_table_index++;
                  //goto k2;
                } else {
                strcpy(symtable[sym_table_index].name,token1);
                symtable[sym_table_index].defined = 'u';
                symtable[sym_table_index].address = address+3;
                symtable[sym_table_index].type = 'l';
                strcpy(symtable[sym_table_index].value,"***");
                symtable[sym_table_index].sym_table_index = sym_table_index;
                sym_table_index++;
                }
              }
              
              a = atoi(token1);
              b = atoi(token2);
              if((a != (long)NULL || *token1 == '0') && (b != (long)NULL || *token2 == '0')) {
                fprintf(op,"%s Littab#%d , Littab#%d\n", token, entry1, entry2);  
              }
              else if((a != (long)NULL || *token1 == '0') && (b == (long)NULL || *token2 != '0')) {
                fprintf(op,"%s Littab#%d , Symtab#%d\n", token, entry1, checkEntry(token2, sym_table_index));  
              }
              else if((a == (long)NULL || *token1 != '0') && (b != (long)NULL || *token2 == '0')) {
                fprintf(op,"%s Symtab#%d , Littab#%d\n", token, checkEntry(token1, sym_table_index), entry2);  
              }
              else if((a == (long)NULL || *token1 != '0') && (b == (long)NULL || *token2 != '0')) {
                fprintf(op,"%s Symtab#%d , Symtab#%d\n", token, checkEntry(token1, sym_table_index), checkEntry(token2, sym_table_index));  
              }
              //fprintf(op,"\n%s SymTab#%d , Symtab#%d ",token, checkEntry(token1,sym_table_index), checkEntry(token2,sym_table_index));
              //           k2: continue;   
            }
          }
        }
        address++;
      }
        fclose(op);
        
        op = fopen(immediate_output,"r");
        //printf("%s",errorTypes[errors[0].errorType]);
        rewind(ip);
        lit_table_index = populateLiteralTable(sym_table_index,lit_table_index);
        printErrorList(filename,error_table_index, sym_table_index);
    fclose(ip);
    } else {
      perror(filename);
      exit(0);
    }
}
