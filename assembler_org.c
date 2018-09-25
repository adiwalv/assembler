#include "symbol_literal_error.h"
int main(int argc, char *argv[]) {
  if (optind != argc-2) {
      printf("Usage: %s file_name [-psilt] [-h for help] \n",argv[0]);
  }
  char *token = (char*)malloc(sizeof(char) * 100);
    char *token1 = (char*)malloc(sizeof(char) * 100);
    char *token2 = (char*)malloc(sizeof(char) * 100);
    char *token3 = (char*)malloc(sizeof(char) * 100);


    FILE *ip = fopen(argv[optind],"r");
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
              littab[lit_table_index].lit_table_index = lit_table_index;
              symtable[sym_table_index].literal_table_link = lit_table_index;
              strcpy(littab[lit_table_index].value,convertStringToHex(substring));
              littab[lit_table_index].sym_table_index = sym_table_index;
              token1 = strtok(NULL,","); 
              while(token1) {
                if(strcmp(token1,"10") == 0) {
                  strcat(littab[lit_table_index].value,"0A");
                  strcpy(substring,"\\n");
                }
                if(strcmp(token1,"0") == 0) {
                  strcat(littab[lit_table_index].value,"00");
                  strcpy(substring,"\\0");
                }
                strcat(symtable[sym_table_index].value,substring);
                token1 = strtok(NULL,",");
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
              token = strtok(NULL,"\n\t\r ");
              token1 = strtok(token,",");
              char *substring = (char*)malloc(sizeof(char) * 100);
              substring = extract_quoted_string(substring,token1);
              count = strlen(substring);
              strcpy(symtable[sym_table_index].value,substring);
              symtable[sym_table_index].literal_table_link = lit_table_index;
              littab[lit_table_index].lit_table_index = lit_table_index;
              strcpy(littab[lit_table_index].value,convertStringToHex(substring));
              littab[lit_table_index].sym_table_index = sym_table_index;              
              token1 = strtok(NULL,","); 
              while(token1) {
                if(strcmp(token1,"10") == 0) {
                  strcat(littab[lit_table_index].value,"0A");
                  strcpy(substring,"\\n");
                }
                if(strcmp(token1,"0") == 0) {
                  strcat(littab[lit_table_index].value,"00");
                  strcpy(substring,"\\0");
                }
                strcat(symtable[sym_table_index].value,substring);
                token1 = strtok(NULL,",");
                count++;
              }
              free(substring);
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
        lit_table_index = populateLiteralTable(sym_table_index,lit_table_index);
    while ((ch = getopt(argc, argv, "psitlh")) != -1)
    {
      switch(ch){
        case 'p':
          printSource(ip);
          break;
          
        case 's':
          printSymTab(sym_table_index);
          break;
          
        case 'i':
          printImmediateCode(op);
          break;
          
        case 't':
          printLiteralTab(lit_table_index);
          break;

           case 'l':
          break;
          
        case 'h':
          printf("\n-p : To print source program.\n-s : To print symbol table\n-i : To print immediate code\n-t : To print literal table\n-l : To print lst of the source file");
          break;
      } 
    }
    printErrorList(error_table_index, sym_table_index);
    fclose(ip);
    } else {
      perror(argv[1]);
    }
  return 0;
}    
