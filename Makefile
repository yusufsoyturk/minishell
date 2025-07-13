NAME = minishell

CC = cc -g

RM = rm -f

CFLAGS = -Wall -Wextra -Werror -I.

LDFLAGS = -lreadline -lncurses

BUILT_IN =	built_in/built.c \
			built_in/built_utils.c \
			built_in/built_func.c \
			built_in/built_export.c \
			built_in/built_export_func.c \
			built_in/built_cd.c \
			built_in/built_exit.c

UTILS = utils/utils.c \
		utils/free.c \
		utils/free_2.c \
		utils/init.c \
		utils/signal.c \
		utils/heredoc_signal.c

LEXER = lexer/tokenize.c \
		lexer/tokenize_utils.c \
		lexer/tokenize_quotes.c \
		lexer/tokenize_func.c \
		lexer/tokenize_control.c

PARSE = parse/parse_utils.c \
		parse/parser.c \
		parse/parser_redir.c

EXECUTE = 	execute/execute.c \
			execute/execute_redir.c \
			execute/execute_redir_utils.c \
			execute/execute_utils.c \
			execute/execute_get_path.c \
			execute/execute_envp.c \
			execute/execute_expand.c \
			execute/execute_child.c \
			execute/child_utils.c 

EXPANDER = 	expander/env_var.c \
			expander/remove_quotes.c \
			expander/expand_change.c \
			expander/expand_env_var.c \
			expander/expand_pre.c \
			expander/handle_dollar.c \
			expander/handle_dollar_env.c \
			expander/expand_quotes.c

SRCS =	$(BUILT_IN) \
		$(PARSE) \
		$(UTILS) \
		$(LEXER) \
		$(EXPANDER) \
		$(EXECUTE) \
		main.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	make -C ./libft -s
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) libft/libft.a $(LDFLAGS) 

clean:
	make clean -C libft
	$(RM) $(OBJS)

fclean: clean
	make fclean -C libft
	$(RM) $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re bonus

