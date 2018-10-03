// Author : Vikas Adiwal
#include "definitions.h"

void printSymTab(int sym_table_index) {
  int outer;
          printf("\n\n\tSym Table:\n");
        printf("============================================================================================================================================\n");
        printf("|%-12s|%-12s|%-12s|%-12s|%-12s|%-12s|%-30s|%-10s|%-18s|\n","Table Index","Name","Size","No of items","Type","Defined","Value","Address","Littab Entry");
        printf("============================================================================================================================================\n");
        for (outer = 1; outer < sym_table_index; outer++) {
          printf ("|%-12d|%-12s|%-12d|%-12d|%-12c|%-12c|%-30s|%-10d|%-18d|\n", symtable[outer].sym_table_index, symtable[outer].name, symtable[outer].size,
                  symtable[outer].no_of_items, symtable[outer].type, symtable[outer].defined,
                  symtable[outer].value, symtable[outer].address, symtable[outer].literal_table_link);
        }
        printf("============================================================================================================================================\n");
}

void printSource(char *filename){
  int line_no = 1;
  char line[50];
  FILE *ip = fopen(filename,"r");
  printf("\n\n\tProgram:\n");
  while ( fgets ( line, sizeof line, ip ) != NULL ){
    printf("%12d%20s",line_no, line);
    line_no++;
  }
}

void printLiteralTab(int lit_table_index){
  
  printf("\n\n\tLiteral Table:\n");
             
        printf("============================================================================\n");
        printf("|%-12s|%-40s|%20s|\n","Table Index","Value","Symbol Table Index");
        printf("============================================================================\n");
        for(outer = 1; outer < lit_table_index; outer++) {
          printf("|%-12d|%-40s|%20d|\n",littab[outer].lit_table_index,littab[outer].value,littab[outer].sym_table_index);
      
          }
         printf("============================================================================\n");
}
/**
void printImmediateCode(FILE* op){

  
  printf("\n\n\tImmediate Code from the file created:\n");
        while ( fgets ( line, sizeof line, op ) != NULL )
          printf("%s", line);

}
**/

void printImmediateCode(FILE* op, char *filename){
  token = (char*)malloc(sizeof(char)*100);
  char source_file[50][50];
  char immediate[50][50];
  int i = 0;
  char space = ' ';
  FILE *source = fopen(filename, "r");
  printf("\nIntermediate Code generated for per line in the source:\n\n");
  while (fgets(line, sizeof line, source) != NULL) {
     token = strtok(line, "\n\t\r ");
     if (strcmp(token, "section") == 0) {
       token = strtok(NULL, "\n\t\r ");
       if (strcmp(token, ".text") == 0) {
         break;
       }
     }
  }
  while ( fgets ( line, sizeof line, source ) != NULL ) {
    token = strtok(line, "\n\t\r ");
    if (strcmp(token, "global") == 0) {
      break;
    }
    //if (strcmp(token, "extern") == 0) {
    //break;
    //}
  }
  while ( fgets ( line, sizeof line, source ) != NULL ) {
    // fputs ( line, stdout ); /* write the line */
     strcpy(source_file[i], line);
     source_file[i][strlen(source_file[i])-1] = space;
    i++;
  }
  i = 0;
  op = fopen("immediate.i","r");
  while ( fgets ( line, sizeof line, op ) != NULL ) {
    //fputs ( line, stdout ); 
    strcpy(immediate[i], line);
    //immediate[i][strlen(immediate[i])-1] = space;
    i++;
  }

  printf("\t Source File \t\t\t\t\t\t Immediate Code\n\n");
  for (int j = 0; j < i; j++) {
    printf("%d\t %s\t\t\t\t\t\t%s", j+1, source_file[j], immediate[j]);
  }
/**printf("\n\n\tImmediate Code from the file created:");
        while ( fgets ( line, sizeof line, op ) != NULL )
        printf("%s", line);**/
}



void printErrorList(char *filename,int error_table_index, int sym_table_index){
  int outer;
  for(outer = 1; outer <= sym_table_index; outer++) {
          if(symtable[outer].defined == 'u') {
            errors[error_table_index].address = symtable[outer].address;
            errors[error_table_index].errorType = 0;
            errors[error_table_index].symTab_index = outer;
            error_table_index++;
          }
        }
        for(outer = 1; outer < error_table_index; outer++) {
          if(errors[outer].errorType == 0) {
            printf("\n%s:%d: error: Symbol %s undefined",filename,errors[outer].address,symtable[errors[outer].symTab_index].name);
          }
          if(errors[outer].errorType == 1) {
            printf("\n%s:%d: error: Symbol %s redefined",filename,errors[outer].address,symtable[errors[outer].symTab_index].name);
          }
          if(errors[outer].errorType == 2) {
            printf("\n%s:%d: error: Not a valid instruction",filename, errors[outer].address);
          }
          //printf("\n%s:%d: %s %s",filename,errors[outer].address,symtable[errors[outer].symTab_index].name,errorTypes[errors[outer].errorType]);
        }
}

void printUsage(char *argv){
   printf("Usage: %s file_name [-options] [-h for help]\n",argv);
}

void printHelp(){
     printf("\nOptions:\n-p : To print source program.\n-s : To print symbol table\n-i : To print immediate code\n-t : To print literal table\n-l : To print lst of the source file");
}

