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
  for(int i = 0; i < sym_table_index; i++) {
    
  }
  fclose(op);
}

void generateLST(char *path) {
  //printf("\nFilen mae---->%s",getFilename(path));
}
