// Author : Vikas Adiwal
#include "build/generate_object.h"

char eax[32],ecx[32],edx[32],ebx[32],esp[32],ebp[32],esi[32],edi[32];
int flag[3]; //carry zero sign
//memset(flag,0,3);


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
    int c,j=0;
    char value[8];
    
    for(i = 1; strcmp(add[i],"00000000") != 0; i++);
    for(c=1;c<count;c++) {
      if(inst[c][0] == 'B') {  //mov
        switch(inst[c][1]) {
          case '8':
            if(inst[c][2] == '[') {
               j = 0;
              while(inst[c][j+3]!=']') {
                value[j] = inst[c][j+3];
                j++;
              }
              value[8] = '\0';
              strcpy(eax,inst[valueAt(add,count,value)]);
              //printf("Eax = %s\n",eax);
            } else {
              j = 0;
              while(inst[c][j+2]!='\0'){
                value[j] = inst[c][j+2];
                j++;
              }
              value[8]= '\0';
              strcpy(eax,value);
            }
            break;
          case '9':
            if(inst[c][2] == '[') {
               j = 0;
              while(inst[c][j+3]!=']') {
                value[j] = inst[c][j+3];
                j++;
              }
              value[8] = '\0';
              strcpy(ecx,inst[valueAt(add,count,value)]);
            } else {
              j = 0;
              while(inst[c][j+2]!='\0'){
                value[j] = inst[c][j+2];
                j++;
              }
              value[8]= '\0';
              strcpy(ecx,value);
            }
            break;
          case 'A':
            if(inst[c][2] == '[') {
              j = 0;
              while(inst[c][j+3]!=']') {
                value[j] = inst[c][j+3];
                j++;
              }
              value[8] = '\0';
              strcpy(edx,inst[valueAt(add,count,value)]);
            } else {
              j = 0;
              while(inst[c][j+2]!='\0'){
                value[j] = inst[c][j+2];
                j++;
              }
              value[8]= '\0';
              strcpy(edx,value);
            }
            break;
          case 'B':
            if(inst[c][2] == '[') {
              j = 0;
              while(inst[c][j+3]!=']') {
                value[j] = inst[c][j+3];
                j++;
              }
              value[8] = '\0';
              strcpy(ebx,inst[valueAt(add,count,value)]);
            } else {
              j = 0;
              while(inst[c][j+2]!='\0'){
                value[j] = inst[c][j+2];
                j++;
              }
              value[8]= '\0';
              strcpy(ebx,value);
            }
            break;
          case 'C':
            if(inst[c][2] == '[') {
              j = 0;
              while(inst[c][j+3]!=']') {
                value[j] = inst[c][j+3];
                j++;
              }
              value[8] = '\0';
              strcpy(esp,inst[valueAt(add,count,value)]);
            } else {
              j = 0;
              while(inst[c][j+2]!='\0'){
                value[j] = inst[c][j+2];
                j++;
              }
              value[8]= '\0';
              strcpy(esp,value);
            }
            break;
          case 'D':
            if(inst[c][2] == '[') {
              j = 0;
              while(inst[c][j+3]!=']') {
                value[j] = inst[c][j+3];
                j++;
              }
              value[8] = '\0';
              strcpy(ebp,inst[valueAt(add,count,value)]);
            } else {
              j = 0;
              while(inst[c][j+2]!='\0'){
                value[j] = inst[c][j+2];
                j++;
              }
              value[8]= '\0';
              strcpy(ebp,value);
            }
            break;
          case 'E':
            if(inst[c][2] == '[') {
              j = 0;
              while(inst[c][j+3]!=']') {
                value[j] = inst[c][j+3];
                j++;
              }
              value[8] = '\0';
              strcpy(esi,inst[valueAt(add,count,value)]);
            } else {
              j = 0;
              while(inst[c][j+2]!='\0'){
                value[j] = inst[c][j+2];
                j++;
              }
              value[8]= '\0';
              strcpy(esi,value);
            }
            break;
          case 'F':
            if(inst[c][2] == '[') {
              j = 0;
              while(inst[c][j+3]!=']') {
                value[j] = inst[c][j+3];
                j++;
              }
              value[8] = '\0';
              strcpy(edi,inst[valueAt(add,count,value)]);
            } else {
              j = 0;
              while(inst[c][j+2]!='\0'){
                value[j] = inst[c][j+2];
                j++;
              }
              value[8]= '\0';
              strcpy(edi,value);
            } 
            break;
        }
      }
      if (inst[c][0] == '8') {
        switch(inst[c][1]){
          case '1':
            value[0]=inst[c][2];
            value[1]=inst[c][3];
            value[2]='\0';
            // printf("%s",value);
            break;
        }
      }
      if (inst[c][0] == '0'){
        switch(inst[c][1]){
          case '5':
            if(inst[c][2] == '[') {
              j = 0;
              while(inst[c][j+3]!=']') {
                value[j] = inst[c][j+3];
                j++;
              }
              value[8] = '\0';
              strcpy(value,inst[valueAt(add,count,value)]);
              // printf("%s",value);
            }}
      }
    }
    
    printf("\nValue of Registers: \n");
    printf("\neax = %s",eax);
    printf("\necx = %s",ecx);
    printf("\nedx = %s",edx);
    printf("\nebx = %s",ebx);
    printf("\nesp = %s",esp);
    printf("\nebp = %s",ebp);
    printf("\nesi = %s",esi);
    printf("\nedi = %s",edi);
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
