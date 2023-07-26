SRCS = main.c launcher.c utils.c 
OBJS = $(addprefix ./.build/, $(SRCS))
OBJS := $(OBJS:%.c=%.o)


all	: philo

philo : ${OBJS}
	cc $(OBJS) -o philo -pthread -Wall -Werror -Wextra

./.build/%.o : ./src/%.c
	mkdir -p ./.build
	cc -c $< -o $@ -I ./src -Wall -Werror -Wextra

clean :
	rm -rf ${OBJS}

fclean : clean
	rm -f philo

re : fclean all


.PHONY : all clean fclean re