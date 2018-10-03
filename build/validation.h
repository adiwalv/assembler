
int validateInstruction(char *instruction) {
  size = sizeof(valid_instructions)/10;
  int flag = -1;
  for (int i = 0 ; i < 4; i++) {
    if (strcmp(valid_instructions[i], instruction) == 0)
      flag = 1;
  }
  if (flag == 1) {
    //  printf("%s-->", instruction);
    if (strcmp(instruction, "mov") == 0) {
      
    }
  }
  return flag;
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
