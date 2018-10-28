#include "symbol_literal_error.h"

char *getDotFilename(char* mystr) {
    char *retstr;
    char *lastdot;
    if (mystr == NULL)
         return NULL;
    if ((retstr = malloc (strlen (mystr) + 1)) == NULL)
        return NULL;
    strcpy (retstr, mystr);
    lastdot = strrchr (retstr, '.');
    if (lastdot != NULL)
        *lastdot = '\0';
    strcat(retstr,".o");
    return retstr;
}

void generateObjectFile(char *filename){
  FILE *op = fopen(getDotFilename(filename),"w");
  FILE *ip = fopen(immediate_output,"r");
  char *hex = (char*)malloc(sizeof(char)*8);
  char *value = (char*)malloc(sizeof(char)*8);
  
  memset(hex,0,8);
  memset(value,0,8);
  char *type1,*type2;
  
  int i;
  int c;
  long ret1,ret2;
  char *s;
  for(i = 1; symtable[i].section != 'B'; i++) {
    c = 0;
    sprintf(hex,"%08X",(unsigned int)symtable[i].address); 
    fprintf(op,"%s  %s\n",hex, littab[symtable[i].literal_table_link].value);
  }

  fprintf(op,"\n");
  for(; symtable[i].section != 'T'; i++) {
    sprintf(hex,"%08X",(unsigned int)symtable[i].address);
    sprintf(value,"%08X",(unsigned int)symtable[i].size);
    fprintf(op,"%s  <res %s>\n", hex, value);
  }
  
  fprintf(op,"\n");

  while(fgets(line, sizeof line, ip) != NULL) {

    token = strtok(line,"\n\t\r ");
    if(strcmp(token,"mov") == 0){
      token1 = strtok(NULL,"\n\t\r ,");
      token2 = strtok(NULL,"\n\t\r ,");
      extractNumber(&token1,&token2,&ret1,&ret2,&type1,&type2);
      
    }
    if(strcmp(token,"add") == 0){
      
    }
    if(strcmp(token,"mov") == 0){
      
    }
    if(strcmp(token,"sub") == 0){
      
    }
  }
  fclose(op);
  fclose(ip);
}

void generateLST(char *path) {

}
