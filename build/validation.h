#include "print.h"

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
  while(fgets(line, sizeof line, fp) != NULL) {
    token = strtok(line,"\n\t\r ");
    token1 = strtok(NULL," ");
    token2 = strtok(NULL,",");
    if (token1 != NULL) {
      if (strcmp(token,"add") == 0) {
       
      }
      if (strcmp(token,"mov") == 0) {
       
      }
      if (strcmp(token,"mul") == 0) {
       
      }
      if (strcmp(token,"sub") == 0) {
       
      }
    }
    address++;
  }
}
