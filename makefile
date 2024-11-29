NAME = minishell
SRCS =	msh.c src/tokenizer/tokenizer.c src/tokenizer/tokenizer_utils.c src/parser/parser.c \
		src/parser/analyse_ast.c src/parser/free_ast.c src/parser/debug.c \
		src/exec/exec.c src/exec/exec_utils.c src/exec/exec_error.c src/signals.c \
		src/env_utils.c  src/builtins/env.c  src/builtins/export.c  src/builtins/unset.c \
		src/builtins/builtin_utils.c src/builtins/pwd.c src/builtins/echo.c src/builtins/exit.c \
		src/expander/expand.c src/expander/quote_utils.c src/expander/expand_utils.c \
		src/heredoc.c src/utils/status.c

OBJS = $(SRCS:.c=.o)

CC = cc
IFLAGS = -Iinclude -Ilibc
CFLAGS = -g -Wall -Wextra -Werror
LDFLAGS = -lreadline -lft -Llibc
LIBFTDIR = libc
LIBFT = $(LIBFTDIR)/libft.a
all: $(NAME)


$(LIBFT):
	$(MAKE) bonus -C $(LIBFTDIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)
%.o: %.c
	$(CC) $(IFLAGS)  $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) fclean -C $(LIBFTDIR)

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C $(LIBFTDIR)

valgrind: $(NAME)
	valgrind --track-fds=yes  --suppressions=.valignores --leak-check=full --show-leak-kinds=all ./msh

re: fclean all

.PHONY: clean fclean re
