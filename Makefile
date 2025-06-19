CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread 
NAME = philo

OBJDIR = ofiles
SRC = main.c handle_erreur.c initialization.c parsing.c philoso.c threads.c utils.c
OBJ = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))

all: $(OBJDIR) $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJDIR)/%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re