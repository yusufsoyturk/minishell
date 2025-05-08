NAME = minishell

CC = cc -g

RM = rm -f

CFLAGS = -Wall -Wextra -Werror -I.

LDFLAGS = -lreadline -lncurses

LIBFT = libft.a
LIBFT_PATH = "libft"

BUILT_IN = built_in/built.c \
			built_in/built_utils.c 

UTILS = utils/utils.c \
		utils/free.c

LEXER = lexer/tokenize.c \
		lexer/tokenize_utils.c \
		lexer/tokenize_quotes.c \
		lexer/tokenize_func.c 

PARSE = parse/parse_utils.c \
		parse/parser.c 

SRCS = $(BUILT_IN) \
		$(PARSE) \
		$(UTILS) \
		$(LEXER) \
		main.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(LIBFT):
	@make -C $(LIBFT_PATH)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(LIBFT_PATH) -lft $(LDFLAGS)

clean:
	make clean -C libft
	$(RM) $(OBJS)

fclean: clean
	make fclean -C libft
	$(RM) $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re bonus

