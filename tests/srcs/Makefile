NAMES		=	test0 \
			test1 \
			test2 \
			test3 \
			test4 \
			test5 \
			test6
SRCS		=	$(NAMES:=.c)
OBJS		=	$(NAMES:=.o)
CC		=	gcc
FLAGS		=	-Wall -Wall -Wextra
LIBS		=	-L./../.. -lft_malloc
IGNS		=	-Wno-free-nonheap-object -Wno-unused-result

all		:	$(NAMES)

$(NAMES)	:	% : %.o
			$(CC) $(CFLAGS) $< -o $@ $(LIBS)

%.o		:	%.c
			$(CC) $(CFLAGS) -c $< -o $@ $(IGNS)

clean		:
			$(RM) $(OBJS)

fclean		:	clean
			rm -rf $(NAMES)

re		:	fclean all
