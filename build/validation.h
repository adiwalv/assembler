#include "print.h"

void insertIntoSystab(int sym_table_index, char *name, int size, \
                      int no_of_items, char defined, char type, \
                      char *value, int address, \
                      int literal_table_link) {
  symtable[sym_table_index].sym_table_index = sym_table_index;
  strcpy(symtable[sym_table_index].name, name);
  symtable[sym_table_index].size = size;
  symtable[sym_table_index].no_of_items = no_of_items;
  symtable[sym_table_index].defined = defined ;
  symtable[sym_table_index].type = type;
  strcpy(symtable[sym_table_index].value, value);
  symtable[sym_table_index].address = address;
  symtable[sym_table_index].literal_table_link = literal_table_link;
}

void insertIntoLiteral(int *lit_table_index, char *value, \
                       char *original_value, int sym_table_index) {
  littab[*lit_table_index].lit_table_index = *lit_table_index;
  strcpy(littab[*lit_table_index].value, value);
  strcpy(littab[*lit_table_index].original_value, original_value);
  littab[*lit_table_index].sym_table_index = sym_table_index;
  (*lit_table_index)++;
}

void insertIntoError(int address, int errorType, int symTab_index, int *error_table_index) {
  errors[*error_table_index].address = address;
  errors[*error_table_index].errorType = errorType;
  errors[*error_table_index].symTab_index = symTab_index;
  (*error_table_index)++;
}


char * strlwr(char * s) {
  char *t = s;
  if (!s)
    return 0;
  int i = 0;
  while ( *t != '\0' ) {
    if (*t >= 'A' && *t <= 'Z') {
      *t = *t + ('a' - 'A');
    }
    t++;
  }
  return s;
}


void sort_instructions() {
  char temp[20];
  for (int i = 0; i < instruction_set_size - 1 ; i++)
  {
    for (int j = i + 1; j < instruction_set_size; j++)
    {
      if (strcmp(valid_instructions[i], valid_instructions[j]) > 0) 
      {
        strcpy(temp, valid_instructions[i]);
        strcpy(valid_instructions[i], valid_instructions[j]);
        strcpy(valid_instructions[j], temp);
      }
    }
  }
}


int findInstructions(char *instruction) {
  int low,high,mid;
  low=0;
  high= instruction_set_size - 1;
  while(low<=high)
  {
    mid=(low+high)/2;
    if (strcmp(instruction,valid_instructions[mid])==0)
    {
      //  printf("\nIndex of %s is %d",valid_instructions[mid],mid);
      return mid;
    }
    else if(strcmp(instruction,valid_instructions[mid])>0)
    {
      high=high;
      low=mid+1;
    }
    else
    {
      low=low;
      high=mid-1;
    }
  }
  return -1;
}


int validateMnemonic(char *instruction) {
  sort_instructions();
  int flag = findInstructions(instruction);
  return flag;
}


int validateInstructions(char *filename) {
  token = (char*)malloc(sizeof(char) * 100);
  token1 = (char*)malloc(sizeof(char) * 100);
  token2 = (char*)malloc(sizeof(char) * 100);
  FILE *fp = fopen(immediate_output,"r");
  FILE *op = fopen(filename,"r");
  char *f,*g,*h;
  address = 1;
  while(fgets(line, sizeof line, op) != NULL) {
    token = strtok(line,"\n\t\r ");
        if(strcmp(token,"section") == 0) {
            token = strtok(NULL,"\n\t\r ");
            if(strcmp(token,".text") == 0) {
              break;
            }	
          }
          address++;
  }
  while(fgets(line, sizeof line, op) != NULL) {
    token = strtok(line,"\n\t\r ");
        if(strcmp(token,"global") == 0) {
           address++;	
        }
        if(strcmp(token,"extern") == 0) {
           address++;	
        }
  }
  //printf("%d--->",address);
  address+=1;
  while(fgets(line, sizeof line, fp) != NULL) {
    token = strtok(line,"\n\t\r ");
    token1 = strtok(NULL," ");
    token2 = strtok(NULL,",");
    if (token2 == NULL && token1 == NULL) {
       insertIntoError(address, 2, -1, &error_table_index);
       continue;
    }
    if (token1 != NULL) {
      if (strcmp(token,"add") == 0) {
        f = strstr(token1,"Symtab");
        g = strstr(token2,"Symtab");
        h = strstr(token1,"Littab");
        if (f&&g) {
          insertIntoError(address, 2, -1, &error_table_index);
        }
        if (h) {
          insertIntoError(address, 2, -1, &error_table_index);
        }
      }
      if (strcmp(token,"mov") == 0) {
       f = strstr(token1,"Symtab");
       g = strstr(token2,"Symtab");
       h = strstr(token1,"Littab");
       if (f&&g) {
         insertIntoError(address, 2, -1, &error_table_index);
       }
       if (h) {
         insertIntoError(address, 2, -1, &error_table_index);
       }
      }
      if (strcmp(token,"mul") == 0) {
        f = strstr(token1,"Symtab");
        g = strstr(token2,"Symtab");
        h = strstr(token1,"Littab");
        if (f&&g) {
          insertIntoError(address, 2, -1, &error_table_index);
        }
        if (h) {
          insertIntoError(address, 2, -1, &error_table_index);
        }
      }
      if (strcmp(token,"sub") == 0) {
        f = strstr(token1,"Symtab");
        g = strstr(token2,"Symtab");
        h = strstr(token1,"Littab");
        if (f&&g) {
          insertIntoError(address, 2, -1, &error_table_index);
        }
        if (h) {
          insertIntoError(address, 2, -1, &error_table_index);
        }
      }
    }
    if (token2 == NULL) {
      if (strcmp(token,"jmp") == 0) {
         f = strstr(token1,"Littab");
        if (f) {
          insertIntoError(address, 2, -1, &error_table_index);
        }
      }
      if (strcmp(token,"dec") == 0) {
        f = strstr(token1,"Littab");
        if (f) {
          insertIntoError(address, 2, -1, &error_table_index);
        }
      }
      if (strcmp(token,"inc") == 0) {
        f = strstr(token1,"Littab");
        if (f) {
          insertIntoError(address, 2, -1, &error_table_index);
        }
      }
    }
    address++;
  }
}
