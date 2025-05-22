NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRC = philo.c utils.c parse.c

OBJ = $(SRC:.c=.o)

HEADER = philo_bonus.h

DEPS = $(OBJ:.o=.d)

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c $(HEADER)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean
