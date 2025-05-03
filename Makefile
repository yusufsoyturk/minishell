NAME = minishell

CC = cc -g
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -I.
LDFLAGS = -lreadline -lncurses

SRCS =	src/minishell.c \
		src/pars/parser.c \
		src/pars/pars_utils.c \
		src/built_in/built.c \
		src/built_in/built_utils.c \
		src/utils/utils.c \
		libft/libft.a \

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): libft/libft.a $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(LDFLAGS)

libft/libft.a:
	make -C libft

clean:
	make clean -C libft
	$(RM) $(OBJS)

fclean: clean
	make fclean -C libft
	$(RM) $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re bonus

