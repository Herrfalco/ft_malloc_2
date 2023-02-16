ifeq ($(HOSTTYPE),)
HOST	?=	$(shell uname -m)_$(shell uname -s)
endif

NAME	=	libft_malloc_$(HOSTTYPE).so
SYML	=	libft_malloc.so
SRCS	=	srcs/zone.c \
		srcs/big_zone.c \
		srcs/print.c \
		srcs/core.c
OBJS	=	$(SRCS:.c=.o)
CC	=	gcc
EFLAGS	=	-Wall -Wextra -Werror -fvisibility=hidden
CFLAGS	=	-fpic
LFLAGS	=	-shared

all	:	$(NAME)

$(NAME)	:	$(OBJS)
		$(CC) $^ $(EFLAGS) $(LFLAGS) -o $@ $(LIBS)
		$(RM) $(SYML)
		ln -s $(NAME) $(SYML)

%.o	:	%.c
		$(CC) $(EFLAGS) $(CFLAGS) -c $< -o $@

clean	:
		$(RM) $(OBJS)

fclean	:	clean
		rm -rf $(NAME) $(SYML)

re	:	fclean all
