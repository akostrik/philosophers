/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Makefile                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 11:56:54 by akostrik          #+#    #+#             */
/*   Updated: 2023/07/23 16:14:01 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

SRC  =	main.c utils1.c utils2.c
OBJ  = $(SRC:.c=.o)
OBJ := $(addprefix ./objs/, $(OBJ))
CC   = clang -pthread -Wall -Wextra -Werror

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

norme:
	norminette $(SRC)

.PHONY: all, clean, fclean, re, debug
