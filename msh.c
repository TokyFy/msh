#include <msh.h>
#include <string.h>

#define WHITESPACE " \t\r\n\v"
#define SYMBOL "<|>"

typedef enum e_type
{
	WORD,
	REDIR_I,
	REDIR_O,
	APPEND,
	PIPE,
	HEREDOC,
}

typedef struct s_token
{
	e_type type;
	char *value;
} t_token;

t_token*	get_world(char **buff)
{
	(void)(buff);
	return NULL;
}

t_token*	get_operator(char **buff)
{
	(void)(buff);
	return NULL;
}

t_token*	get_token(char **buff)
{
	char	*str;
	str = *buff;
	while (*str && strchr(WHITESPACE, *str))
		str++;
	if (strchr(SYMBOL, *str))
	{
		return get_operator(buff);
	}

	return (get_world(buff));
}

int	main(int argc, char **argv)
{
	char	*token;
	char	*buff;

	(void)(argc);
	(void)(argv);
	buff = strdup("ls -a");
	return (1);
}
