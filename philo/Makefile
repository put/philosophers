NAME = philo
CC = cc
OBJS_DIR  = objects
HEADERS := -I philosophers.h
CFLAGS = -Wall -Wextra -Werror -g -pthread

SRCS =	main.c \
		ft_atoi.c \
		death.c \
		eating.c \
		mutex_management.c \
		parsing.c \
		printing.c \
		threads.c \
		time.c

OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)

all: $(NAME)

$(OBJS_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

$(NAME): $(OBJS)
	@$(CC) $(OBJS) $(HEADERS) -o $(NAME)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all re fclean clean