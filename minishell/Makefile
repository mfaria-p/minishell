NAME = minishell

SRCS_PARSER =	destroy_tree.c	\
				check_tree.c	\
				expand.c		\
				lexer.c			\
				lexer_utils.c	\
				lexer_redir.c	\
				lexer_cmd.c		\
				lexer_pipe.c	\
				parser.c

SRCS_EXEC =		ft_exit.c			\
				ft_echo.c			\
				ft_pwd.c			\
				ft_cd.c				\
				ft_cd2.c 			\
				ft_unset.c			\
				export_env.c 		\
				export_env2.c		\
				error_handling.c	\
				error_handling2.c	\
				exec_utils.c		\
				exec_utils2.c		\
				exec_utils3.c		\
				exec_utils4.c		\
				execution.c			\
				execution2.c		\
				execution3.c		\
				export_env3.c

SRCS = minishell.c signal.c $(addprefix parser/, $(SRCS_PARSER)) $(addprefix exec/, $(SRCS_EXEC))

OBJS = $(SRCS:.c=.o)

CC = cc

# CFLAGS = -Wall -Wextra -Werror -I. #-lreadline
CFLAGS = -lreadline -I. -g #-fsanitize=address -static-libasan

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

vgdb:
	valgrind --suppressions=parser/ignore_readline --leak-check=full --show-leak-kinds=all --track-fds=yes -q --vgdb-error=0 ./$(NAME)

valgrind:
	valgrind --suppressions=parser/ignore_readline --leak-check=full --show-leak-kinds=all --track-fds=yes ./minishell


.PHONY: all clean fclean re build vgdb valgrind

# WHAT HAPPENS IF COMMAND IS A FILE BUT NOT A EXECUTABLE BINARY
