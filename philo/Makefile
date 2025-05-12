NAME = philo
SRC = helper_f.c philosophers.c check_error.c organization_p_f.c loop_philo.c
CFLAGS = -Wall -Wextra -Werror -pthread -g -fsanitize=thread 
OBJ = $(SRC:.c=.o)

all : $(NAME)

$(NAME): $(OBJ)
	cc $(CFLAGS) $(OBJ)  -o $(NAME) 
%.o:%.c philo.h
	cc $(CFLAGS) -c $< -o $@
clean:
	rm -rf $(OBJ)
fclean : clean
	rm -rf $(NAME)
re : fclean all
