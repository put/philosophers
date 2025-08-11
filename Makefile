NAME = philo
CC = cc
SRC_DIR = source
OBJS_DIR  = objects
HEADERS := -I ./include
CFLAGS = -Wall -Wextra -Werror -g -pthread

SRCS =	$(SRC_DIR)/main.c \
		$(SRC_DIR)/ft_atoi.c \
		$(SRC_DIR)/death.c \
		$(SRC_DIR)/eating.c \
		$(SRC_DIR)/mutex_management.c \
		$(SRC_DIR)/parsing.c \
		$(SRC_DIR)/printing.c \
		$(SRC_DIR)/threads.c \
		$(SRC_DIR)/time.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJS_DIR)/%.o)

all: $(NAME)

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c
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