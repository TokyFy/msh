NAME = msh
SRCS = msh.c src/tokenizer.c
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

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re
