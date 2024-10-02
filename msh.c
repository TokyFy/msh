#include <msh.h>

int	main(int argc, char **argv)
{
	t_token	*token;
	char	*buff;

	(void)(argc);
	(void)(argv);
	buff = readline("> ");

    while ((token = get_token(&buff)))
    {
	    print_token(token);
    }
    print_token(token);
}
