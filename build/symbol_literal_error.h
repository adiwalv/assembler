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
    
}
