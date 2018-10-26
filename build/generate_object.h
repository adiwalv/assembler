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
  char *hex = (char*)malloc(sizeof(char)*8);
  char *value = (char*)malloc(sizeof(char)*8);
  char *divide = (char*)malloc(sizeof(char)*100);
  memset(hex,0,8);
  memset(value,0,8);
  
  int i;
  int c;
  int diff;
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
  
  fclose(op);
}

void generateLST(char *path) {
  //printf("\nFilen mae---->%s",getFilename(path));
}
