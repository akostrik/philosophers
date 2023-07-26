SRCS = error.c main.c init.c utils.c launcher.c
OBJS = $(addprefix ./.build/, $(SRCS))
OBJS := $(OBJS:%.c=%.o)


all	: philo

philo : ${OBJS}
	cc $(OBJS) -o philo -pthread -Wall -Werror -Wextra

./.build/%.o : ./src/%.c
	mkdir -p ./.build
	cc -c $< -o $@ -g3 -I ./src -Wall -Werror -Wextra

clean :
	rm -rf ${OBJS}

fclean : clean
	rm -f philo

re : fclean all

.PHONY : all clean fclean re