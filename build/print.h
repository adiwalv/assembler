// Author : Vikas Adiwal
#include "definitions.h"

void printSymTab(int sym_table_index) {
  int outer;
  printf(ANSI_COLOR_YELLOW "\n\n\tSym Table:" ANSI_COLOR_RESET "\n");
        printf(ANSI_COLOR_CYAN "======================================================================================================================================="ANSI_COLOR_RESET"\n");
        printf("|%-12s|%-12s|%-12s|%-12s|%-4s|%-7s|%-35s|%-10s|%-13s|%-7s|\n","Table Index","Name","Size","No of items","Type","Defined","Value","Address","Littab Entry","Section");
        printf(ANSI_COLOR_CYAN "======================================================================================================================================="ANSI_COLOR_RESET"\n");
        for (outer = 1; outer < sym_table_index; outer++) {
          printf ("|%-12d|%-12s|%-12d|%-12d|%-4c|%-7c|%-35s|%-10d|%-13d|%-7c|\n", symtable[outer].sym_table_index, symtable[outer].name, symtable[outer].size,
                  symtable[outer].no_of_items, symtable[outer].type, symtable[outer].defined,
                  symtable[outer].value, symtable[outer].address, symtable[outer].literal_table_link,symtable[outer].section);
        }
        printf(ANSI_COLOR_CYAN "======================================================================================================================================="ANSI_COLOR_RESET"\n");
}

void printSource(char *filename){
  int line_no = 1;
  char line[50];
  FILE *ip = fopen(filename,"r");
  printf(ANSI_COLOR_YELLOW"\n\n\tProgram:"ANSI_COLOR_RESET"\n");
  while ( fgets ( line, sizeof line, ip ) != NULL ){
    printf("%12d%20s",line_no, line);
    line_no++;
  }
}

void printLiteralTab(int lit_table_index){
  
  printf(ANSI_COLOR_YELLOW"\n\n\tLiteral Table:"ANSI_COLOR_RESET "\n");
             
        printf(ANSI_COLOR_CYAN"====================================================================================================================="ANSI_COLOR_RESET "\n");
        printf("|%-12s|%-40s|%-40s|%20s|\n","Table Index","Hex Value","Original Value","Symbol Table Index");
        printf(ANSI_COLOR_CYAN"====================================================================================================================="ANSI_COLOR_RESET "\n");
        for(outer = 1; outer < lit_table_index; outer++) {
          printf("|%-12d|%-40s|%-40s|%20d|\n",littab[outer].lit_table_index,littab[outer].value,littab[outer].original_value,littab[outer].sym_table_index);
      
          }
        printf(ANSI_COLOR_CYAN"====================================================================================================================="ANSI_COLOR_RESET "\n");
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
  printf(ANSI_COLOR_YELLOW"\nIntermediate Code generated for per line in the source:" ANSI_COLOR_RESET "\n\n");
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

  printf(ANSI_COLOR_BLUE"\t Source File \t\t\t\t\t\t Immediate Code"ANSI_COLOR_RESET"\n\n");
  for (int j = 0; j < i; j++) {
    printf("%d\t %s\t\t\t\t\t\t%s", j+1, source_file[j], immediate[j]);
  }
/**printf("\n\n\tImmediate Code from the file created:");
        while ( fgets ( line, sizeof line, op ) != NULL )
        printf("%s", line);**/
}



int printErrorList(char *filename, int error_table_index, int sym_table_index) {
  int outer;
  for (outer = 1; outer <= sym_table_index; outer++) {
          if (symtable[outer].defined == 'u') {
            errors[error_table_index].address = symtable[outer].address;
            errors[error_table_index].errorType = 0;
            errors[error_table_index].symTab_index = outer;
            error_table_index++;
          }
        }
  if (error_table_index > 1) {
        for (outer = 1; outer < error_table_index; outer++) {
          if (errors[outer].errorType == 0) {
            printf(ANSI_COLOR_RED"\n%s:%d: error: Symbol %s undefined"ANSI_COLOR_RESET"",filename, \
                   errors[outer].address, \
                   symtable[errors[outer].symTab_index].name);
          }
          if (errors[outer].errorType == 1) {
            printf(ANSI_COLOR_RED"\n%s:%d: error: Symbol %s redefined"ANSI_COLOR_RESET"", filename, \
                   errors[outer].address, \
                   symtable[errors[outer].symTab_index].name);
          }
          if (errors[outer].errorType == 2) {
            printf(ANSI_COLOR_RED"\n%s:%d: error: Not a valid instruction"ANSI_COLOR_RESET"", filename, \
                   errors[outer].address);
          }
        }
  } else {
    printf(ANSI_COLOR_GREEN "\nNo errors reported!" ANSI_COLOR_RESET "\n");
  }
  return error_table_index;
}

void printUsage(char *argv) {
  printf("Usage: %s file_name [-options] [-h for help]\n", argv);
}

void printHelp() {
     printf(ANSI_COLOR_BLUE "\nOptions:\n-p : To print source program.\n-s : To print symbol table\n-i : To print immediate code\n-t : To print literal table\n-l : To print lst of the source file"ANSI_COLOR_RESET "\n");
}

