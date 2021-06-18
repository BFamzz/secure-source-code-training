#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char name[20];
    printf("Enter your name:\n");
    gets(name);
    printf("Your name is %s\n", name);
    return EXIT_SUCCESS;
}