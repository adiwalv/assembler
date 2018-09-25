#include<stdio.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    int ch;
    while ((ch = getopt(argc, argv, "ab")) != -1)
    {
        if (ch == 'a')
            printf("Got A\n");
        else if (ch == 'b')
            printf("Got B\n");
        else
            printf("Got confused\n");
    }

    if (optind != argc-1) 
        printf("You forgot to enter the filename\n");
    else
        printf("File: %s\n", argv[optind]);
}
