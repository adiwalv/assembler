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
  if(flag != -1) {
    if(strcmp(instruction,"add") == 0) {
      printf("Hello1");
    }
    if(strcmp(instruction,"call") == 0) {
      printf("Hello2");
    }
    if(strcmp(instruction,"dec") == 0) {
      printf("Hello3");
    }
    if(strcmp(instruction,"inc") == 0) {
      printf("Hello4");
    }
    if(strcmp(instruction,"jmp") == 0) {
      printf("Hello5");
    }
    if(strcmp(instruction,"mov") == 0) {
      printf("Hello6");
    }
    if(strcmp(instruction,"mul") == 0) {
      printf("Hello7");
    }
    if(strcmp(instruction,"sub") == 0) {
      printf("Hello8");
    }
  }
  return flag;
}
