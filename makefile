NAME = msh
SRCS = msh.c
OBJS = $(SRCS:.c=.o)

CC = cc
IFLAGS = -Iinclude
CFLAGS = -Wall -Wextra -Werror
all: $(NAME)


$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lreadline

%.o: %.c
	$(CC) $(IFLAGS)  $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re
