NAME = philo
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -g -fsanitize=thread 
RM = rm -f

SRCS = main.c utils.c philo.c monitor.c init.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJS)

fclean:	clean
	@$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re