CC = cc
CFLAGS = -Werror -Wall -Wextra -pthread
SRC = main.c parsing.c initialization.c	
OBJ = $(SRC:.c=.o)
NAME = philo

all: $(NAME)

$(NAME): philo.h $(OBJ)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: re clean fclean