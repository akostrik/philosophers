# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/20 21:17:38 by akostrik          #+#    #+#              #
#    Updated: 2023/07/23 16:07:21 by akostrik         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC  =	main.c utils1.c utils2.c
OBJ  = $(SRC:.c=.o)
OBJ := $(addprefix ./objs/, $(OBJ))
CC   = clang -Wall -Wextra -Werror -pthread 

all: philo

philo : $(OBJ)
	$(CC) $(OBJ) -Iphilo -o philo

./objs/%.o: ./src/%.c
	@mkdir -p ./objs/ 
	$(CC) -Iphilo -o $@ -c $<

clean:
#	rm -f $(OBJ)
	@rm -rf ./objs/

fclean: clean
	rm -f philo

re: fclean all

# git:
# 	git add *
# 	git commit -m "philo"
# 	git push

norme:
	norminette ./src

.PHONY: all, clean, fclean, re, debug