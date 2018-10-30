// Author : Vikas Adiwal
#include "build/generate_object.h"

int valueAt(char *add[],int count,char *value) {
  for(int i = 0 ; i < count ; i++) {
    if(strcmp(add[i],value) == 0){
      return i;
    }
  }
  return -1;
}

int main(int argc, char *argv[]) {
  char *add[100];
  char *inst[100];  
  token1 = (char*)malloc(sizeof(char)*100);
  token2 = (char*)malloc(sizeof(char)*100);
  for(i=0;i<100;i++) {
    add[i]=malloc(sizeof(char)*100);
    inst[i]=malloc(sizeof(char)*100);
  }
  char pc[8];
  int count = 0;
  
  if (argc !=2) {
    printf("Specify a file.");
    exit(1);    
  }
  FILE *ip = fopen(argv[1],"r");
  if(ip != NULL) {

    while(fgets(line, sizeof line, ip)!=NULL) {
      int num_matches = sscanf(line, "%s %s", token1, token2);
      if( num_matches == 2 ){
        strcpy(add[count],token1);
        strcpy(inst[count],token2);
        count++;
      } else
        continue;
    }

    
    
  } else {
    perror(argv[1]);
    exit(0);
  }
  free(token1);
    free(token2);
}


/**


   
      if( num_matches == 2 ){
        strcpy(pc,token1);

        if(strcmp(pc,"00000000") == 0) {
          count++;
        }
        if(count == 2) {
          break;                          
        }
**/
