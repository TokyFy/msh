#include <msh.h>

int main(int argc , char **argv)
{
	(void)(argc);
	(void)(argv);
	char *buff;
	buff = readline("> ");
	add_history(buff);
	printf("%s\n" , buff);
	free(buff);

	return (1);
}
