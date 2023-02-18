NOLIB		=	test0 \
			test1 \
			test2 \
			test3 \
			test4 \
			test5
WLIB		=	test6
OBJS		=	$(NOLIB:=.o) $(WLIB:=.o)
CC		=	gcc
FLAGS		=	-Werror -Wall -Wextra
LIBS		=	-L./../.. -lft_malloc
IGNS		=	-Wno-free-nonheap-object -Wno-unused-result

all		:	$(NOLIB) $(WLIB)

$(NOLIB)	:	% : %.o
			$(CC) $(CFLAGS) $< -o $@

$(WLIB)		:	% : %.o
			$(CC) $(CFLAGS) $< -o $@ $(LIBS)

%.o		:	%.c
			$(CC) $(CFLAGS) -c $< -o $@ $(IGNS)

clean		:
			$(RM) $(OBJS)

fclean		:	clean
			rm -rf $(NOLIB) $(WLIB)

re		:	fclean all
