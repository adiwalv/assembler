#include "symbol_literal_error.h"


void getReg(int ret1,char **instruct) {
  if(ret1 == 0){
          strcpy(*instruct,"B8");
        }
        if(ret1 == 1){
          strcpy(*instruct,"B9");
        }
        if(ret1 == 2){
          strcpy(*instruct,"BA");
        }
        if(ret1 == 3){
          strcpy(*instruct,"BB");
        }
        if(ret1 == 4){
          strcpy(*instruct,"BC");
        }
        if(ret1 == 5){
          strcpy(*instruct,"BD");
        }
        if(ret1 == 6){
          strcpy(*instruct,"BE");
        }
        if(ret1 == 7){
          strcpy(*instruct,"BF");
        }
}


char *textFromHexString(char *hex,char *result)
{
  char text[20]={0};
  int tc=0;
  
  for(int k=0;k<strlen(hex);k++)
  {
    if(k%2!=0)
    {
      char temp[3];
      sprintf(temp,"%c%c",hex[k-1],hex[k]);
      int number = (int)strtol(temp, NULL, 16);
      text[tc]=(char)number;
      
      tc++;   
      
    }
  }  
  strcpy(result,text);
  return result;
}

char *getDotFilename(char* mystr,char *ext) {
    char *retstr;
    char *lastdot;
    if (mystr == NULL)
         return NULL;
    if ((retstr = malloc (strlen (mystr) + 1)) == NULL)
        return NULL;
    strcpy (retstr, mystr);
    lastdot = strrchr (retstr, '.');
    if (lastdot != NULL)
        *lastdot = '\0';
    strcat(retstr,ext);
    return retstr;
}


void generateObjectFile(char *filename){
  FILE *op = fopen(getDotFilename(filename,".o"),"w");
  FILE *ip = fopen(immediate_output,"r");
  char *hex = (char*)malloc(sizeof(char)*8);
  char *value = (char*)malloc(sizeof(char)*10);
  char *opcode = (char*)malloc(sizeof(char)*2);
  char *mod = (char*)malloc(sizeof(char)*2);
  char *instruct = (char*)malloc(sizeof(char)*12);
  int address = 0;
  memset(hex,0,8);
  memset(value,0,10);
  char *type1,*type2;
  
  int i;
  int c;
  long ret1,ret2;
  char *s;
  fprintf(op,"\n");
  for(i = 1; symtable[i].section != 'B'; i++) {
    c = 0;
    sprintf(hex,"%08X",(unsigned int)symtable[i].address); 
    fprintf(op,"%s  %s\n",hex, littab[symtable[i].literal_table_link].value);
  }

  fprintf(op,"\n");
  for(; symtable[i].section != 'T'; i++) {
    sprintf(hex,"%08X",(unsigned int)symtable[i].address);
    sprintf(value,"%08X",(unsigned int)symtable[i].size);
    fprintf(op,"%s  <res%s>\n", hex, value);
  }
  
  fprintf(op,"\n\n");
  
  while(fgets(line, sizeof line, ip) != NULL) {

    memset(instruct,0,12);
    memset(hex,0,8);
    memset(value,0,10);
 
    token = strtok(line,"\n\t\r ");
    if(strcmp(token,"int") == 0){
      token1 = strtok(NULL,"\n\t\r ,");
      extractNumber2(&token1,&type1,&ret1);
      if (strcmp(type1,"Littab") == 0) {
        strcpy(instruct,"78");
        strcat(instruct,littab[ret1].value);
        sprintf(hex,"%08X",address);
        fprintf(op,"%s %s\n",hex,instruct);
        address+=2;
      }
    }
    if(strcmp(token,"mov") == 0){
      token1 = strtok(NULL,"\n\t\r ,");
      token2 = strtok(NULL,"\n\t\r ,");
      extractNumber(&token1,&token2,&ret1,&ret2,&type1,&type2);
      if (strcmp(type1,"Regist") == 0 && strcmp(type2,"Regist") == 0) {
        sprintf(mod,"%s",modrm[ret1][ret2]);
        sprintf(hex,"%08X",address);
        strcpy(instruct,"89");
        strcat(instruct,mod);
        fprintf(op,"%s %s\n",hex,instruct);
        address+=2;
      }
      if (strcmp(type1,"Regist") == 0 && strcmp(type2,"Littab") == 0) {
        getReg(ret1,&instruct);
        strcat(instruct,littab[ret2].value);
        sprintf(hex,"%08X",address);
        fprintf(op,"%s %s\n",hex,instruct);
        address+=5;
      }
      if (strcmp(type1,"Regist") == 0 && strcmp(type2,"SymTab") == 0) {
        getReg(ret1,&instruct);
        sprintf(hex,"%08X",address);
        sprintf(value,"[%08X]",symtable[ret2].address);
        strcat(instruct,value);
        fprintf(op,"%s %s\n",hex,instruct);
        address+=5;
      }
    }
    if(strcmp(token,"add") == 0){
      token1 = strtok(NULL,"\n\t\r ,");
      token2 = strtok(NULL,"\n\t\r ,");
      extractNumber(&token1,&token2,&ret1,&ret2,&type1,&type2);
      if (strcmp(type1,"Regist") == 0 && strcmp(type2,"SymTab") == 0) {
        if (ret1 == 0) {
          strcpy(instruct,"05");
          sprintf(hex,"%08X",address);
          sprintf(value,"[%08X]",symtable[ret2].address);
          strcat(instruct,value);
          fprintf(op,"%s %s\n",hex,instruct);
          address+=5;
        } else {
           strcpy(instruct,"81");
           strcat(instruct,modrm[ret1][0]);
           sprintf(hex,"%08X",address);
           sprintf(value,"[%08X]",symtable[ret2].address);
           strcat(instruct,value);
           fprintf(op,"%s %s\n",hex,instruct);
           address+=5;
        }
      }
      if (strcmp(type1,"Regist") == 0 && strcmp(type2,"Regist") == 0) {
        sprintf(mod,"%s",modrm[ret1][ret2]);
        sprintf(hex,"%08X",address);
        strcpy(instruct,"01");
        strcat(instruct,mod);
        fprintf(op,"%s %s\n",hex,instruct);
        //printf("\\helloasdasd");
        address+=2;
      }
       if (strcmp(type1,"Regist") == 0 && strcmp(type2,"Littab") == 0) {
         if(ret1 == 0) {
           strcpy(instruct,"05");
           strcat(instruct,littab[ret2].value);
           sprintf(hex,"%08X",address);
           fprintf(op,"%s %s\n",hex,instruct);
           address+=5;
         } else {
           strcpy(instruct,"81");
           strcat(instruct,modrm[ret1][0]);
           strcat(instruct,littab[ret2].value);
           sprintf(hex,"%08X",address);
           fprintf(op,"%s %s\n",hex,instruct);
           address+=5;
         }
       }
    }
    if(strcmp(token,"sub") == 0){
      token1 = strtok(NULL,"\n\t\r ,");
      token2 = strtok(NULL,"\n\t\r ,");
      extractNumber(&token1,&token2,&ret1,&ret2,&type1,&type2); 
      if (strcmp(type1,"Regist") == 0 && strcmp(type2,"Regist") == 0) {
        sprintf(mod,"%s",modrm[ret1][ret2]);
        sprintf(hex,"%08X",address);
        strcpy(instruct,"29");
        strcat(instruct,mod);
        fprintf(op,"%s %s\n",hex,instruct);
        address+=2;
      }
      if (strcmp(type1,"Regist") == 0 && strcmp(type2,"Littab") == 0) {
        if(ret1 == 0) {
          strcpy(instruct,"2D");
          strcat(instruct,littab[ret2].value);
          sprintf(hex,"%08X",address);
          fprintf(op,"%s %s\n",hex,instruct);
          address+=5;
        } else {
          strcpy(instruct,"83");
          strcat(instruct,modrm[ret1][0]);
          strcat(instruct,littab[ret2].value);
          sprintf(hex,"%08X",address);
          fprintf(op,"%s %s\n",hex,instruct);
          address+=5;
        }
      }
       if (strcmp(type1,"Regist") == 0 && strcmp(type2,"SymTab") == 0) {
          if (ret1 == 0) {
          strcpy(instruct,"2D");
          sprintf(hex,"%08X",address);
          sprintf(value,"[%08X]",symtable[ret2].address);
          strcat(instruct,value);
          fprintf(op,"%s %s\n",hex,instruct);
          address+=5;
        } else {
           strcpy(instruct,"83");
           strcat(instruct,modrm[ret1][0]);
           sprintf(hex,"%08X",address);
           sprintf(value,"[%08X]",symtable[ret2].address);
           strcat(instruct,value);
           fprintf(op,"%s %s\n",hex,instruct);
           address+=5;
        }
       }
    }
    if(strcmp(token,"mul") == 0){
      token1 = strtok(NULL,"\n\t\r ,");
      strcpy(instruct,"F7");
      if(strcmp(token1,"Regist0") == 0) {
        strcat(instruct,"E0");
      }
      if(strcmp(token1,"Regist1") == 0) {
        strcat(instruct,"E1");
      }
      if(strcmp(token1,"Regist2") == 0) {
        strcat(instruct,"E2");
      }
      if(strcmp(token1,"Regist3") == 0) {
        strcat(instruct,"E3");
      }
      if(strcmp(token1,"Regist4") == 0) {
        strcat(instruct,"E4");
      }
      if(strcmp(token1,"Regist5") == 0) {
        strcat(instruct,"E5");
      }
      if(strcmp(token1,"Regist6") == 0) {
        strcat(instruct,"E6");
      }
      if(strcmp(token1,"Regist7") == 0) {
        strcat(instruct,"E7");
      }
       sprintf(hex,"%08X",address);
       fprintf(op,"%s %s\n",hex,instruct);
    }
    address+= 2;
  }
  fclose(op);
  fclose(ip);
}

void generateLST(char *filename) {
  char *dot_filename,*lst_filename;
  dot_filename = getDotFilename(filename,".o");
  lst_filename = getDotFilename(filename,".lst");
  char *command = (char*)malloc(sizeof(char)*100);
  strcpy(command,"pr -m -t");
  strcat(command," ");
  strcat(command,dot_filename);
  strcat(command," ");
  strcat(command,filename);
  strcat(command,">");
  strcat(command,lst_filename);
  int status = system(command);
  if(!status) {
    printf(ANSI_COLOR_GREEN "LST created!!!" ANSI_COLOR_RESET "\n");
  }
  //status = remove(dot_filename);
}
