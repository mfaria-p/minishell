NAME = minishell

SRCS = minishell.c

OBJS = $(SRCS:.c=.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror -lreadline

RM = rm -rf

all: $(NAME)

%.o: %.c
	$(CC) $(<) -c -o $(@) $(CFLAGS)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(CFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

#valgrind --suppressions=parser/ignore_readline --leak-check=full --show-leak-kinds=all --track-fds=yes ./a.out
