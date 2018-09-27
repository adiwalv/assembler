// Author : Vikas Adiwal
#include "definitions.h"

void printSymTab(int sym_table_index) {
  int outer;
          printf("\n\n\tSym Table:\n");
        printf("=======================================================================================================================================\n");
        printf("%12s%12s%12s%12s%12s%12s%30s%10s%18s\n","Table Index","Name","Size","No of items","Type","Defined","Value","Address","Littab Entry");
        printf("=======================================================================================================================================\n");
        for (outer = 1; outer < sym_table_index; outer++) {
          printf ("%12d%12s%12d%12d%12c%12c%30s%10d%18d\n", symtable[outer].sym_table_index, symtable[outer].name, symtable[outer].size,
                  symtable[outer].no_of_items, symtable[outer].type, symtable[outer].defined,
                  symtable[outer].value, symtable[outer].address, symtable[outer].literal_table_link);
        }
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
             
        printf("==========================================================================\n");
        printf("%12s%40s%20s\n","Table Index","Value","Symbol Table Index");
        
        printf("==========================================================================\n");
        for(outer = 1; outer < lit_table_index; outer++) {
          printf("%12d%40s%20d\n",littab[outer].lit_table_index,littab[outer].value,littab[outer].sym_table_index);
      
          }
}

void printImmediateCode(FILE* op){
  
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

void printUsage(char *argv){
   printf("Usage: %s file_name [-options] [-h for help]\n",argv);
}

void printHelp(){
     printf("\nOptions:\n-p : To print source program.\n-s : To print symbol table\n-i : To print immediate code\n-t : To print literal table\n-l : To print lst of the source file");
}

