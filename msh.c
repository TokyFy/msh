#include <msh.h>

int main(int argc , char **argv)
{
	(void)(argc);
	(void)(argv);
	char *buff;
	while(42)
	{
		buff = readline("> ");
		add_history(buff);
		printf("%s\n" , buff);
		free(buff);
	}	

	return (1);
}
