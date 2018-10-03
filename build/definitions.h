// Author : Vikas Adiwal

#ifndef BUILD_DEFINITIONS_H_
#define BUILD_DEFINITIONS_H_

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>


struct symtab {
  int sym_table_index;
  char name[150];
  int size;
  int no_of_items;
  char defined;
  char type;
  char value[100];
  int address;
  int literal_table_link;
}symtable[100];


struct literaltab {
  int lit_table_index;
  char value[100];
  int sym_table_index;
}littab[100];

struct errorTable {
  int address;
  int errorType;
  int symTab_index;
}errors[100];


char* convertStringToHex(char* str);
char* extract_quoted_string(char *substring, char *token1);
int checkEntry(char *s, int sym_table_index);
int registerTable(char *s);
void fetchSection(char* section, FILE **fp, char *token, int *address);
int populateLiteralTable(int sym_table_index, int lit_table_index);
void printSource(char *filename);
void printSymTab(int sym_table_index);
void printLiteralTab(int lit_table_index);
void printImmediateCode(FILE* op, char *filename);
void printErrorList(char* filename, int error_table_index, int sym_table_index);
char* makeLittleEndian(char *str);

char line[150];
int ch, outer, address = 0, sym_table_index = 1, count = 0, \
  error_table_index = 1, lit_table_index = 1, check, check2,    \
  entry1, entry2, size;
int op1, op2;
char immediate_output[] = "immediate.i";
char *token = NULL, *token1 = NULL, *token2 = NULL, *token3 = NULL, \
  *strH = NULL;
int16_t i;
FILE *ip, *op;
int16_t a, b;

char valid_instructions[][10] = {"mov", "add", "sub", "mul", "jmp", "dec", "inc", "call"};

#endif  //  BUILD_DEFINITIONS_H_
