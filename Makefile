SRC_NAME =	main.c utils.c utils2.c philo_action.c
SRC = $(addprefix ./src/,$(SRC_NAME))
OBJ = $(SRC_NAME:.c=.o)
OBJ := $(addprefix ./objs/, $(OBJ))
CC = clang -pthread -Wall -Wextra -Werror

all: philo

philo : $(OBJ)
	@$(CC) $(OBJ) -o philo

./objs/%.o: ./src/%.c
	@mkdir ./objs/ 2> /dev/null || true
	@$(CC) -o $@ -c $<

clean:
	@rm -f $(OBJ)
	@rmdir ./objs/ 2> /dev/null || true

fclean: clean
	@rm -f philo

re: fclean all

git:
	git add *
	git commit -m "philo"
	git push

norme:
	norminette $(SRC)

.PHONY: all, clean, fclean, re, debug