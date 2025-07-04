NAME = philo
CC = cc
SRC_DIR = source
OBJS_DIR  = objects
HEADERS := -I ./include

CFLAGS = -Wall -Wextra -Werror -O3 -flto
LDFLAGS = -lpthread -flto

SRCS =	$(SRC_DIR)/main.c \
		$(SRC_DIR)/ft_atoi.c \
		$(SRC_DIR)/funcs1.c \
		$(SRC_DIR)/funcs2.c \
		$(SRC_DIR)/funcs3.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJS_DIR)/%.o)

all: $(NAME)

# Compile .c files to .o
$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

# Link .o files to final binary
$(NAME): $(OBJS)
	@$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all re fclean clean
