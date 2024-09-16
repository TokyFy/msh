#include <stdio.h>
#include <unistd.h>

int main()
{
    if(-1 == read(10, NULL , sizeof(char)))
        perror("Shell");
    return (1);
}
