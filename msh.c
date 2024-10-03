#include <msh.h>

int	main(const int argc, char **argv)
{
	t_token	*token;
	char	*buff;
	char	*line;

	(void)(argc);
	(void)(argv);

	while(1)
	{
		line = readline("> ");
		buff = line;
		add_history(buff);
		while ((token = get_token(&buff)))
			print_token(token);
		free(line);
	}
}
