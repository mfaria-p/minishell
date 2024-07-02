NAME = minishell

SRCS_PARSER =	destroy_tree.c	\
				expand.c		\
				lexer.c			\
				lexer_utils.c	\
				parser.c

SRCS_EXEC =		built_ins.c			\
				built_ins2.c		\
				built_ins3.c		\
				error_handling.c	\
				exec_utils.c		\
				exec_utils2.c		\
				exec_utils3.c		\
				execution.c			\
				execution2.c		\
				execution3.c

SRCS = minishell.c $(addprefix parser/, $(SRCS_PARSER)) $(addprefix exec/, $(SRCS_EXEC))

OBJS = $(SRCS:.c=.o)

CC = cc

# CFLAGS = -Wall -Wextra -Werror -lreadline
CFLAGS = -lreadline -I.

RM = rm -rf

all: $(NAME)

%.o: %.c
	$(CC) $< -c -o $@ $(CFLAGS)

$(NAME): $(OBJS)
	$(CC) $(OBJS) parser/libft.a -o $(NAME) $(CFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

#valgrind --suppressions=parser/ignore_readline --leak-check=full --show-leak-kinds=all --track-fds=yes ./a.out
