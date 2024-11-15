NAME = msh
SRCS = msh.c src/tokenizer.c src/parser.c src/builtin.c src/expand.c src/exec.c src/signals.c \
		src/env_utils.c
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
	$(MAKE) clean -C $(LIBFTDIR)

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C $(LIBFTDIR)

valgrind: $(NAME)
	valgrind --track-fds=yes  --suppressions=.valignores --leak-check=full --show-leak-kinds=all ./msh

re: fclean all

.PHONY: clean fclean re
