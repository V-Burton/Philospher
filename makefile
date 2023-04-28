######### Name and Folder #########
NAME 	:= philosopher

FILES 	:= main.c \
			utils.c

PATH_LIBFT	:= libft/

######### Compileur and various options ########

CC    =    cc

CFLAGS    =    -Werror -Wall -Wextra -g3

AR    =		ar

ARFLAGS =    rcs

SANITY    =    -fsanitize=address

RM 		:= 	rm -rf

########## Rules and dependency ###########

HFILES    =    philosopher.h\

ODIR    =    ./OBJS/

HDIR    =    ./

SRCS    =    $(FILES)

OBJS    =    $(addprefix $(ODIR), $(FILES:.c=.o))

all    :     mklib $(ODIR) $(NAME)

$(NAME)    :$(OBJS) ${PATH_LIBFT}libft.a
			$(CC) $(CFLAGS) -o $(PATH_LIBFT)libft.a

$(ODIR)    :
			mkdir $(ODIR)

$(ODIR)%.o   :    %.c $(HFILES)
				$(CC) $(CFLAGS) -o $@ -c $<

mklib:
			make -j -C ${PATH_LIBFT}
			make -j -C ${PATH_PRINTF}

clean    :
		-rm -rf $(OBJS)
		-rm -rf $(ODIR)
		make clean -C ${PATH_LIBFT}
		make clean -C ${PATH_PRINTF}

fclean    :    clean
		-rm $(NAME)
		make fclean -C ${PATH_LIBFT}
		make fclean -C ${PATH_PRINTF}

re    :
	make fclean
	make all

clearm    :
	clear
	make

.PHONY    :    all clean fclean re