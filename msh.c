#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argv , char **argv)
{
	int fd = open("txt" , O_RDWR);
	dup2(fd , STDOUT_FILENO);
	printf("Hello world");
	return (1);
}
