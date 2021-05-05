NAME = philo_one

SRCS = srcs/parse_args.c	\
	   srcs/utils.c			\
	   srcs/philo_routine.c	\
	   srcs/main.c
OBJS = $(SRCS:.c=.o)

CC = clang
CFLAGS = -Wall -Wextra -Werror -I./includes/ -g3

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -lpthread -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
