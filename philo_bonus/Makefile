NAME = philo_bonus
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread

SRC = main.c \
      utils.c \
      simulation.c \
      philosopher_actions.c \
      philosopher_lifecycle.c

OBJ = $(SRC:.c=.o)

HEADER = philosophers.h

DEPS = $(OBJ:.o=.d)

RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(YELLOW)Linking...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "$(GREEN)Compilation successful! Executable: $(NAME)$(RESET)"

# Compile object files
%.o: %.c $(HEADER)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Include dependency files
-include $(DEPS)

# Clean object files
clean:
	@echo "$(RED)Removing object files...$(RESET)"
	@rm -f $(OBJ) $(DEPS)

# Clean object files and executable
fclean: clean
	@echo "$(RED)Removing executable...$(RESET)"
	@rm -f $(NAME)

# Rebuild everything
re: fclean all

# Run the program with validation
run: all
	@echo "$(GREEN)Running $(NAME) with 5 philosophers...$(RESET)"
	./$(NAME) 5 800 200 200

# Phony targets
.PHONY: all clean fclean re run
