CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread  -fsanitize=thread
NAME = philo

SRC = main.c handle_erreur.c initialization.c parsing.c philoso.c threads.c utils.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re