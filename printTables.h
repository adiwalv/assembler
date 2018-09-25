#include "definitions.h"

void printSource(FILE* ip){
  int line_no = 1;
  char line[50];
  printf("\n\n\tProgram:\n");
  while ( fgets ( line, sizeof line, ip ) != NULL ){
    printf("%12d%20s",line_no, line);
    line_no++;
  }
}

void printLiteralTab(int lit_table_index){
  int outer;
  printf("\n\n\tLiteral Table:\n");
             
        printf("==========================================================================\n");
        printf("%12s%40s%20s\n","Table Index","Value","Symbol Table Index");
        
        printf("==========================================================================\n");
        for(outer = 1; outer < lit_table_index; outer++) {
          printf("%12d%40s%20d\n",littab[outer].lit_table_index,littab[outer].value,littab[outer].sym_table_index);
      
          }
}

void printImmediateCode(FILE* op){
  char line[50];
  printf("\n\n\tImmediate Code from the file created:");
        while ( fgets ( line, sizeof line, op ) != NULL )
          printf("%s", line);

}

void printErrorList(int error_table_index, int sym_table_index){
  int outer;
  for(outer = 1; outer < sym_table_index; outer++) {
          if(symtable[outer].defined == 'u') {
            errors[error_table_index].address = symtable[outer].address;
            errors[error_table_index].errorType = 0;
            errors[error_table_index].symTab_index = outer;
            error_table_index++;
          }
        }
        printf("\n\tErrors:");
        for(outer = 1; outer < error_table_index; outer++) {
          printf("\nLine %d : %s %s",errors[outer].address,symtable[errors[outer].symTab_index].name,errorTypes[errors[outer].errorType]);
          
        }
}

