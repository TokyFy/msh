#include <msh.h>
#include <string.h>

#define WHITESPACE " \t\r\n\v"
#define SYMBOL "<|>"

typedef enum e_type
{
	WORD,
	REDIR,
	PIPE,
	HEREDOC,
}

typedef struct s_token
{
	e_type type;
	char *value;
} t_token;

t_token*	get_token(char **buff)
{
	char	*str;
	char	*token_end;
	char	ret = 'a';

	str = *buff;
	while (*str && strchr(WHITESPACE, *str))
		str++;
	token_end = str;
	if (strchr(SYMBOL, *str))
	{
		if (strncmp(str, ">>", 2) || strncmp(str, "<<", 2))
		{
			token_end = str + 2;
			ret = 'r';
		}
		else
		{
			token_end = str + 1;
			ret = *str;
		}
	}
	else
		while (*token_end && !strchr(WHITESPACE, *token_end) && !strchr(SYMBOL,
				*token_end))
			token_end++;
	*token = strndup(str, token_end - str);
	*buff = token_end;
	return (ret);
}

int	main(int argc, char **argv)
{
	char	*token;
	char	*buff;

	(void)(argc);
	(void)(argv);
	buff = strdup("ls -a");

	printf("%c\n" , get_token(&buff , &token));
	printf("buff : %s \ntoken : %s\n", buff, token);
	return (1);
}
