NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = philo.c utils.c init.c monitor.c routine.c supervisor.c 
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lpthread

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re