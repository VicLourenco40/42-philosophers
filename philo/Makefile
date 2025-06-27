# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vde-albu <vde-albu@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/23 11:58:11 by vde-albu          #+#    #+#              #
#    Updated: 2025/06/27 19:05:04 by vde-albu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -g -pthread -I $(IDIR)
SDIR = src/
IDIR = include/
ODIR = obj/
OBJ = main.o utils.o manager.o
OBJS = $(patsubst %, $(ODIR)%, $(OBJ))

.PHONY: all clean fclean re

all: $(NAME)

clean:
	rm -fr $(ODIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJS): $(ODIR)%.o: $(SDIR)%.c
	mkdir -p $(ODIR)
	$(CC) $(CFLAGS) -o $@ -c $<
